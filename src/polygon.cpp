#include "polygon.hpp"
#include <SFML/Graphics/Color.hpp>
#include <armadillo>
#include <stdexcept>
#include <vector>
#include <optional>
#include <sstream>
#include <string>

Polygon::Polygon(unsigned int n_vertices) :
    vertices_(n_vertices),
    color_(sf::Color::White)
{}

unsigned int Polygon::nVertices() const
{
    return vertices_.size();
}

unsigned int Polygon::nEdges() const
{
    return vertices_.size();
}

bool Polygon::empty() const
{
    return vertices_.empty();
}

void Polygon::setVertex(unsigned int index, const arma::vec3 &position)
{
    vertices_.at(index) = position;
}

arma::vec3 Polygon::getVertex(unsigned int index) const
{
    return vertices_.at(index);
}

Polygon::Edge Polygon::getEdge(unsigned int index) const
{
    if (index < nVertices() - 1)
    {
        return Edge(getVertex(index), getVertex(index + 1));
    }
    else if (index == nVertices() - 1)
    {
        return Edge(getVertex(index), getVertex(0));
    }
    else
    {
        throw std::out_of_range("Edge index out of bounds");
    }
}

void Polygon::addVertex(const arma::vec3 &position)
{
    vertices_.emplace_back(position);
}

sf::Color Polygon::getColor() const
{
    return color_;
}

void Polygon::setColor(const sf::Color &color)
{
    color_ = color;
}

Polygon Polygon::clip(const Polygon& polygon, const arma::vec3 &plane_normal, 
                      const arma::vec3 &plane_point)
{
    Polygon clipped_polygon = {};
    clipped_polygon.setColor(polygon.getColor());

    for (auto i = 0u; i < polygon.nEdges(); ++i)
    {
        auto e = polygon.getEdge(i);
        double edge_start_dot = arma::dot(plane_normal, e.first - plane_point);
        double edge_end_dot = arma::dot(plane_normal, e.second - plane_point);
        bool is_start_inside = edge_start_dot > 1e-6;
        bool is_end_inside = edge_end_dot > 1e-6;

        if (is_start_inside)
        {
            clipped_polygon.addVertex(e.first);
        }

        if (is_start_inside != is_end_inside)
        {
            auto intersection_point = linePlaneIntersection(
                    e.first, e.second, plane_normal, plane_point);
            clipped_polygon.addVertex(intersection_point);            
        }
    }

    return clipped_polygon;
}

bool Polygon::isCoplanar(const Polygon& other) const
{
    arma::vec3 v = getVertex(1) - getVertex(0);
    arma::vec3 u = getVertex(2) - getVertex(0);
    arma::vec3 n = arma::cross(v, u);

    for (auto i = 0u; i < 3; ++i)
    {
        auto x = other.getVertex(i);
        arma::vec3 w = x - getVertex(0);
        if (arma::dot(w, n) != 0)
        {
            return false;
        }
    }

    return true;
}

arma::vec3 Polygon::normal() const
{
    return arma::normalise(arma::cross(getVertex(0) - getVertex(1), 
                                       getVertex(0) - getVertex(2)));
}

std::string Polygon::toString() const
{
    std::stringstream ss;
    
    ss << "[";
    for (int i = 0; i < nVertices(); ++i)
    {
        auto v = getVertex(i);
        ss << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
        if (i != nVertices() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]";

    return ss.str();
}

arma::vec3 Polygon::linePlaneIntersection(const arma::vec3 &line_point_a,
                                          const arma::vec3 &line_point_b,
                                          const arma::vec3 &plane_normal,
                                          const arma::vec3 &plane_point)
{
    auto line_direction = line_point_b - line_point_a;    
    auto t = arma::dot(plane_point - line_point_a, plane_normal)
             / arma::dot(line_direction, plane_normal);    
    return line_point_a + t * line_direction;
}  
