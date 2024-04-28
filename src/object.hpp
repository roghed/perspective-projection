#pragma once
#include "polygon.hpp"
#include <SFML/Graphics/Color.hpp>
#include <armadillo>
#include <filesystem>
#include <vector>
#include <cstddef>

// class for 3D objects (set of polygons)
class Object
{
public:

    // creates empty object
    Object() = default;

    // loads object from an .obj (Wavefront) file
    Object(const std::filesystem::path& obj_file_path);
    
    std::size_t nPolygons() const;

    arma::vec3 getOrigin() const;
    void       setOrigin(const arma::vec3& pos);

    // sets color of every polygon of the object to `color`
    void setColor(const sf::Color& color);

    const Polygon& getPolygon(std::size_t index) const;
    void setPolygon(std::size_t index, const Polygon& polygon);
    void addPolygon(const Polygon&);

private:

    arma::vec3 origin_;
    std::vector<Polygon> polygons_;
};
