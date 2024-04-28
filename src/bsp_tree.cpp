#include "bsp_tree.hpp"
#include "polygon.hpp"
#include <armadillo>
#include <vector>
#include <stdexcept>

BSPTree::BSPTree(const std::vector<Polygon> &scene_polygons)
{
    rootNode_.polygons = scene_polygons;
    rootNode_.splitAll();
}

std::vector<const Polygon*> BSPTree::depthSortedPolygons(const arma::vec3 observer_position) const
{
    std::vector<const Polygon*> sorted_polygons;
    addPolygonsSortedByObserverPos(sorted_polygons, &rootNode_, observer_position);
    return sorted_polygons;
}

void BSPTree::addPolygonsSortedByObserverPos(
    std::vector<const Polygon*>& poly_vec, 
    const Node* node, 
    const arma::vec3& observer_pos)
{
    if (!node)
    {
        return;
    }
    
    auto plane_normal = node->polygons.front().normal();
    auto plane_point = node->polygons.front().getVertex(0);
    
    if (arma::dot(observer_pos - plane_point, plane_normal) >= 0)
    {
        addPolygonsSortedByObserverPos(poly_vec, node->back, observer_pos);
        for (const auto& p : node->polygons)
        {
            poly_vec.push_back(&p);
        }
        addPolygonsSortedByObserverPos(poly_vec, node->front, observer_pos);
    }
    else
    {
        addPolygonsSortedByObserverPos(poly_vec, node->front, observer_pos);
        for (const auto& p : node->polygons)
        {
            poly_vec.push_back(&p);
        }
        addPolygonsSortedByObserverPos(poly_vec, node->back, observer_pos);
    }
}

void BSPTree::Node::split()
{
    if (polygons.empty())
    {
        throw std::runtime_error("BSPTree::Node::split() called on empty Node");
    }

    if (front || back)
    {
        throw std::runtime_error("BSPTree::Node::split() called on Node that "
                                 "already has children");
    }

    if (polygons.size() == 1)
    {
        return;
    }

    auto polygons_at_node = std::vector<Polygon>{polygons.front()};
    front = new Node();
    back = new Node();
    
    const auto& splitter = polygons.front();
    arma::vec3 plane_normal = splitter.normal();
    arma::vec3 plane_point = splitter.getVertex(0);
    
    for (auto i = 1u; i < polygons.size(); ++i)
    {
        const auto& p = polygons[i];

        if (p.isCoplanar(splitter))
        {
            polygons_at_node.push_back(p);
        }
        else
        {
            auto front_clipping = Polygon::clip(p, plane_normal, plane_point);
            if (!front_clipping.empty())
            {
                front->polygons.push_back(front_clipping);
            }

            auto back_clipping = Polygon::clip(p, -plane_normal, plane_point);
            if (!back_clipping.empty())
            {
                back->polygons.push_back(back_clipping);
            }
        }
    }

    polygons = polygons_at_node;

    if (front->polygons.empty())
    {
        delete front;
        front = nullptr;
    }

    if (back->polygons.empty())
    {
        delete back;
        back = nullptr;
    }
}

void BSPTree::Node::splitAll()
{
    split();
    
    if (front)
    {
        front->splitAll();
    }
    
    if (back)
    {
        back->splitAll();
    }
}
