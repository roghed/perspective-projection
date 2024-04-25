#include "scene.hpp"
#include "polygon.hpp"
#include "bsp_tree.hpp"
#include "object.hpp"
#include "camera.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <armadillo>
#include <cstddef>
#include <vector>

const Object& Scene::getObject(std::size_t index) const
{
    return objects_.at(index);
}

Object &Scene::getObject(std::size_t index)
{
    treeNeedsRebuilding_ = true;
    return objects_.at(index);
}

void Scene::setObject(std::size_t index, const Object& object)
{
    treeNeedsRebuilding_ = true;
    objects_.at(index) = object;
}

void Scene::addObject(const Object& object)
{
    treeNeedsRebuilding_ = true;
    objects_.emplace_back(object);
}

void Scene::addObject(Object&& object)
{
    treeNeedsRebuilding_ = true;
    objects_.emplace_back(object);
}

void Scene::removeObject(std::size_t index)
{
    treeNeedsRebuilding_ = true;
    objects_.erase(objects_.begin() + index);
}

Camera& Scene::camera()
{
    return camera_;
}

void Scene::rebuildBSPTree() const
{
    std::vector<Polygon> all_polygons;
    for (auto& obj : objects_)
    {
        for (auto i = 0u; i < obj.nPolygons(); ++i)
        {
            all_polygons.push_back(obj.getPolygon(i));
        }
    }

    bspTree_ = BSPTree(all_polygons);
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (treeNeedsRebuilding_)
    {
        rebuildBSPTree();
        treeNeedsRebuilding_ = false;
    }
    
    // using painter's algorithm
    auto sorted_polygons = bspTree_.depthSortedPolygons(camera_.getPosition());
    for (const Polygon* p : sorted_polygons)
    {
        target.draw(camera_.project(*p), states);
    }
}
