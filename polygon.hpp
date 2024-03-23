#pragma once
#include <armadillo>
#include <SFML/Graphics/Color.hpp>

class Polygon
{
public:

    Polygon(unsigned int n_vertices);

    unsigned int nVertices() const;

    void       setVertex(unsigned int index, const arma::vec3& position);
    arma::vec3 getVertex(unsigned int index) const;

    sf::Color getColor() const;
    void      setColor(const sf::Color& color);

private:

    std::vector<arma::vec3> vertices_;
    sf::Color               color_;
};
