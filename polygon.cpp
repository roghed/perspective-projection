#include "polygon.hpp"

Polygon::Polygon(unsigned int n_vertices) :
    vertices_(n_vertices)
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

void Polygon::addVertex(const arma::vec &position)
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

Polygon Polygon::clip(const Polygon& polygon, const arma::vec3 &plane_normal_vec, 
                      const arma::vec3 &plane_point)
{
    Polygon clipped_polygon = {};
    clipped_polygon.setColor(polygon.getColor());

    for (auto i = 0u; i < polygon.nEdges(); ++i)
    {
        auto edge = polygon.getEdge(i);
        auto clipped_edge = clipLineSegment(edge, plane_normal_vec, plane_point);

        if (clipped_edge.has_value())
        {                        
            if (clipped_polygon.empty())
            {
                clipped_polygon.addVertex(clipped_edge->first);
            }
            
            clipped_polygon.addVertex(clipped_edge->second);
        }
    }

    return clipped_polygon;
}

std::optional<Polygon::Segment> Polygon::clipLineSegment(
    const Segment& line_segment, const arma::vec3 &plane_normal_vec, 
    const arma::vec3 &plane_point)
{
    bool start_clipped = arma::dot(plane_normal_vec, line_segment.first - plane_point) < 0;
    bool end_clipped = arma::dot(plane_normal_vec, line_segment.second - plane_point) < 0;
    
    if (start_clipped && end_clipped)
    {
        return std::nullopt;
    }

    arma::vec3 d = line_segment.first - line_segment.second;
    auto t = dot(plane_normal_vec, plane_point - line_segment.first)
             / dot(plane_normal_vec, d);
    arma::vec3 plane_intersection = line_segment.first + t * d;

    if (end_clipped)
    {
        return Segment(line_segment.first, plane_intersection);
    }
    else if (start_clipped)
    {
        return Segment(plane_intersection, line_segment.second);
    }
    else return line_segment;
}
