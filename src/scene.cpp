#include "scene.hpp"
#include "polygon.hpp"
#include "bsp_tree.hpp"
#include "object.hpp"
#include "camera.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
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

bool Scene::colorPolygonsByDrawingOrder() const
{
    return bspDebugPolygonColoring_;
}

void Scene::setColorPolygonsByDrawingOrder(bool enabled)
{
    bspDebugPolygonColoring_ = enabled;
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

sf::Color Scene::debugColorMap(std::size_t polygon_index, std::size_t n_polygons)
{    
    float value = float(polygon_index + 1) / n_polygons;
    
    sf::Color clr;
    clr.a = 255;
    
    if (value < 0.25) 
    {
        clr.r = 0;
        clr.g = static_cast<int>(255 * (4 * value));
        clr.b = 255;
    } 
    else if (value < 0.5) 
    {
        clr.r = 0;
        clr.g = 255;
        clr.b = static_cast<int>(255 * (1 - 4 * (value - 0.25)));
    } 
    else if (value < 0.75) 
    {
        clr.r = static_cast<int>(255 * (4 * (value - 0.5)));
        clr.g = 255;
        clr.b = 0;
    } 
    else 
    {
        clr.r = 255;
        clr.g = static_cast<int>(255 * (1 - 4 * (value - 0.75)));
        clr.b = 0;
    }

    return clr;
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (treeNeedsRebuilding_)
    {
        rebuildBSPTree();
        treeNeedsRebuilding_ = false;
    }
    
    auto sorted_polygons = bspTree_.depthSortedPolygons(camera_.getPosition());
    for (std::size_t i = 0; i < sorted_polygons.size(); ++i)
    {
        if (bspDebugPolygonColoring_)
        {
            Polygon p_cpy = *sorted_polygons[i];
            p_cpy.setColor(debugColorMap(i, sorted_polygons.size()));
            target.draw(camera_.project(p_cpy), states);
        }
        else
        {
            target.draw(camera_.project(*sorted_polygons[i]), states);
        }
    }
}
