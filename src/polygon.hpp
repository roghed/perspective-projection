#pragma once
#include <armadillo>
#include <SFML/Graphics/Color.hpp>
#include <optional>
#include <utility>
#include <string>
#include <vector>

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

    void addVertex(const arma::vec3& position);

    sf::Color getColor() const;
    void      setColor(const sf::Color& color);

    static Polygon clip(const Polygon&, const arma::vec3& plane_normal_vec,
                        const arma::vec3& plane_point);

    bool isCoplanar(const Polygon&) const;

    arma::vec3 normal() const;

    // for debugging only
    std::string toString() const;

private:

    static arma::vec3 linePlaneIntersection(
        const arma::vec3& line_point_a, const arma::vec3& line_point_b,
        const arma::vec3& plane_normal, const arma::vec3& plane_point);

    std::vector<arma::vec3> vertices_;
    sf::Color               color_;
};
