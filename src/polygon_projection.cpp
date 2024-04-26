#include "polygon_projection.hpp"

PolygonProjection::PolygonProjection(unsigned int n_vertices) :
    sf::ConvexShape(n_vertices)
{}

void PolygonProjection::setVertex(unsigned int index, const sf::Vector2f &position)
{
    sf::ConvexShape::setPoint(index, position);
}

sf::Vector2f PolygonProjection::getVertex(unsigned int index) const
{
    return sf::ConvexShape::getPoint(index);
}

sf::Color PolygonProjection::getColor() const
{
    return sf::ConvexShape::getFillColor();
}

void PolygonProjection::setColor(const sf::Color& color)
{
    sf::ConvexShape::setFillColor(color);
}
