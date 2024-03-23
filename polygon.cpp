#include "polygon.hpp"

Polygon::Polygon(unsigned int n_vertices) :
    vertices_(n_vertices)
{}

unsigned int Polygon::nVertices() const
{
    return vertices_.size();
}

void Polygon::setVertex(unsigned int index, const arma::vec3 &position)
{
    vertices_.at(index) = position;
}

arma::vec3 Polygon::getVertex(unsigned int index) const
{
    return vertices_.at(index);
}

sf::Color Polygon::getColor() const
{
    return color_;
}

void Polygon::setColor(const sf::Color &color)
{
    color_ = color;
}
