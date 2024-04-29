#include "camera.hpp"
#include "polygon.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <armadillo>
#include <vector>

#define _USE_MATH_DEFINES // for M_PI
#include <cmath>

Camera::Camera() :
    imageDims_{800, 600},
    position_{0, 0, 0},
    projPlane_{0.01, 0, 0},
    up_{0, 0, 1},
    horizontalViewAngle_{90.0 / 360 * M_PI},
    wireframeEnabled_(false)
{
    update();
}

arma::uvec2 Camera::getImageDimensions() const
{
    return imageDims_;
}

void Camera::setImageDimensions(const arma::uvec2 &dimensions)
{
    imageDims_ = dimensions;
    update();
}

arma::vec3 Camera::getPosition() const
{
    return position_;
}

void Camera::setPosition(const arma::vec3 &position_vector)
{
    position_ = position_vector;
    update();
}

arma::vec3 Camera::getDirection() const
{
    return arma::normalise(projPlane_);
}

void Camera::setDirection(const arma::vec3 &direction_vector)
{
    double proj_distance = getNearClippingDistance();
    projPlane_ = direction_vector;
    setNearClippingDistance(proj_distance);
}

double Camera::getNearClippingDistance() const
{
    return arma::norm(projPlane_);
}

void Camera::setNearClippingDistance(double distance)
{
    projPlane_ = arma::normalise(projPlane_) * distance;
    update();
}

void Camera::pitch(double angle)
{
    arma::vec3 U = arma::cross(projPlane_, up_);
    up_ = rotate(up_, U, angle);
    projPlane_ = rotate(projPlane_, U, angle);
    update();
}

void Camera::yaw(double angle)
{
    projPlane_ = rotate(projPlane_, up_, angle);
    update();
}

void Camera::roll(double angle)
{
    up_ = rotate(up_, projPlane_, angle);
    update();
}

double Camera::getFOV() const
{
    return horizontalViewAngle_ / M_PI * 360;
}

void Camera::setFOV(double angle)
{
    horizontalViewAngle_ = angle / 360 * M_PI;
}

void Camera::move(const arma::vec3 &relative_movement)
{
    arma::vec3 lr_axis = arma::normalise(arma::cross(projPlane_, up_));
    arma::vec3 ud_axis = arma::normalise(up_);
    arma::vec3 fb_axis = arma::normalise(projPlane_);
    arma::vec3 absolute_movement = {0, 0, 0};

    absolute_movement += relative_movement[0] * lr_axis;
    absolute_movement += relative_movement[1] * ud_axis;
    absolute_movement += relative_movement[2] * fb_axis;

    setPosition(getPosition() + absolute_movement);
}

void Camera::setWireframe(bool enabled)
{
    wireframeEnabled_ = enabled;
}

bool Camera::isWireframeEnabled() const
{
    return wireframeEnabled_;
}

arma::vec2 Camera::project(const arma::vec3 &point) const
{
    double alpha = arma::dot(projPlane_, projPlane_)
                   / arma::dot(projPlane_, point - position_);
    arma::vec3 c = alpha * (point - position_) - projPlane_;
    arma::vec2 point_projection = {
        (imageDims_[0] / 2.0) * (1.0 + dot(c, hScreenVec_) / dot(hScreenVec_, hScreenVec_)), 
        (imageDims_[1] / 2.0) * (1.0 - dot(c, vScreenVec_) / dot(vScreenVec_, vScreenVec_))
    };

    return point_projection;
}

sf::VertexArray Camera::project(const Polygon& polygon) const
{
    auto clipped_poly = Polygon::clip(polygon, projPlane_, screenCenter_);
    if (clipped_poly.empty())
    {
        return {};
    }

    
    sf::VertexArray projection;
    projection.resize(clipped_poly.nVertices());
    auto poly_color = polygon.getColor();

    for (int i = 0; i < projection.getVertexCount(); ++i)
    {
        auto vertex_projection = project(clipped_poly.getVertex(i));
        
        projection[i].color = poly_color;
        projection[i].position.x = vertex_projection[0];
        projection[i].position.y = vertex_projection[1];
    }

    if (wireframeEnabled_)
    {
        projection.append(projection[0]);
        projection.setPrimitiveType(sf::LineStrip);
    }
    else
    {
        projection.setPrimitiveType(sf::TriangleFan);
    }

    return projection;
}

arma::vec3 Camera::rotate(const arma::vec3 &vector, const arma::vec3 &axis, double angle)
{
    angle = angle / M_PI * 180;
    arma::vec3 k = arma::normalise(axis);
    return vector * std::cos(angle) + arma::cross(k, vector) * std::sin(angle)
           + k * dot(k, vector) * (1.0 - std::cos(angle));
}

void Camera::update()
{
    arma::vec3 U = arma::cross(projPlane_, up_);
    arma::vec3 W = arma::cross(U, projPlane_);
    up_ = arma::normalise(W); // make up vector (Z) be always perpendicular to
                              // projection plane vector (a)
    screenCenter_ = projPlane_ + position_;
    hScreenVec_ = 
            arma::normalise(U) * arma::norm(projPlane_)
            * std::tan(horizontalViewAngle_);
    vScreenVec_ = 
            arma::normalise(W) * arma::norm(hScreenVec_)
            * imageDims_[1] / imageDims_[0];
}
