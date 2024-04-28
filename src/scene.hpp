#pragma once
#include "object.hpp"
#include "camera.hpp"
#include "bsp_tree.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <cstddef>

namespace sf
{
    class RenderTarget;
    class Color;
}

class Scene : public sf::Drawable
{
public:

    Scene() = default;

    const Object& getObject(std::size_t index) const;
    Object&       getObject(std::size_t index);
    void          setObject(std::size_t index, const Object&);
    void          addObject(const Object&);
    void          addObject(Object&&);
    void          removeObject(std::size_t index);

    Camera& camera();

    bool colorPolygonsByDrawingOrder() const;
    void setColorPolygonsByDrawingOrder(bool enabled);

    void rebuildBSPTree() const;

private:

    static sf::Color debugColorMap(std::size_t polygon_index, std::size_t n_polygons);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<Object> objects_;
    Camera              camera_;
    mutable BSPTree     bspTree_;
    mutable bool        treeNeedsRebuilding_ = false;
    bool                bspDebugPolygonColoring_ = false;
};
