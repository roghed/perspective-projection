#pragma once
#include "polygon.hpp"
#include <armadillo>
#include <vector>
#include <iterator>
#include <cstddef>

// data structure for the binary space partitioning algorithm
class BSPTree
{
public:

    // build BSP tree for the given set of scene polygons
    BSPTree(const std::vector<Polygon>& scene_polygons);

    // returns a list of pointers to polygons, sorted by depth relative to the
    // observer's position
    std::vector<const Polygon*> depthSortedPolygons(
        const arma::vec3 observer_position) const;

private:

    struct Node
    {
        std::vector<Polygon> polygons;
        Node* front = nullptr;
        Node* back  = nullptr;

        // take the first polygon at node and split all other polygons into
        // those behind or in front, and add them into corresponding child nodes
        // (creates children if needed)
        void split();

        // do split() on all nodes recursively until all polygons are split
        // essentially builds the BSP tree for all polygons at the current node
        void splitAll();
    };

    static void addPolygonsSortedByObserverPos(
        std::vector<const Polygon*>& poly_vec, 
        const Node* node,
        const arma::vec3& observer_pos);

    arma::vec3 observerPos_;
    Node rootNode_;
};
