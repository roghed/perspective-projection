#pragma once
#include <armadillo>
#include <SFML/Graphics/Color.hpp>
#include <optional>

// class for 3D convex polygons
class Polygon
{
public:

    using Segment = std::pair<arma::vec3, arma::vec3>;
    using Edge = Segment;

    Polygon(unsigned int n_vertices = 0);

    unsigned int nVertices() const;
    unsigned int nEdges() const;
    bool         empty() const;

    // all vertices should lie on the same plane, else the projection will be
    // incorrect
    void       setVertex(unsigned int index, const arma::vec3& position);
    arma::vec3 getVertex(unsigned int index) const;

    Edge getEdge(unsigned int index) const;

    void addVertex(const arma::vec& position);

    sf::Color getColor() const;
    void      setColor(const sf::Color& color);

    static Polygon clip(const Polygon&, const arma::vec3& plane_normal_vec,
                        const arma::vec3& plane_point);

    bool isCoplanar(const Polygon&) const;

    arma::vec3 normal() const;

private:

    static std::optional<Segment> clipLineSegment(
        const Segment& line_segment, const arma::vec3& plane_normal_vec, 
        const arma::vec3& plane_point);

    std::vector<arma::vec3> vertices_;
    sf::Color               color_;
};
