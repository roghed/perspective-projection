#pragma once
#include <SFML/Graphics/ConvexShape.hpp>

class PolygonProjection : public sf::ConvexShape
{
public:

    PolygonProjection(unsigned int n_vertices);

    void         setVertex(unsigned int index, const sf::Vector2f& position);
    sf::Vector2f getVertex(unsigned int index) const;

    sf::Color getColor() const;
    void      setColor(const sf::Color& color);
};
