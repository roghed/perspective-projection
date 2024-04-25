#include "object.hpp"
#include "obj_file_parser.hpp"
#include "polygon.hpp"
#include <SFML/Graphics/Color.hpp>
#include <armadillo>
#include <filesystem>
#include <cstddef>
#include <vector>

Object::Object(const std::filesystem::path &obj_file_path)
{
    WavefrontObjFileParser obj_parser(obj_file_path);
    if (!obj_parser.parse())
    {
        obj_parser.printErrors();
    }
    *this = obj_parser.object();
}

std::size_t Object::nPolygons() const
{
    return polygons_.size();
}

arma::vec3 Object::getOrigin() const
{
    return origin_;
}

void Object::setOrigin(const arma::vec3 &pos)
{
    for (auto& poly : polygons_)
    {
        for (auto i = 0u; i < poly.nVertices(); ++i)
        {
            auto v = poly.getVertex(i);
            poly.setVertex(i, v + origin_ - pos);
        }
    }
}

void Object::setColor(const sf::Color &color)
{
    for (auto& p : polygons_)
    {
        p.setColor(color);
    }
}

const Polygon &Object::getPolygon(std::size_t index) const
{
    return polygons_.at(index);
}

void Object::setPolygon(std::size_t index, const Polygon &polygon)
{
    polygons_.at(index) = polygon;
}

void Object::addPolygon(const Polygon& polygon)
{
    polygons_.emplace_back(polygon);
}
