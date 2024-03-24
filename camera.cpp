#include "camera.hpp"
#include "polygon.hpp"
#include "polygon_projection.hpp"
#include <armadillo>
#include <cmath>

Camera::Camera() :
    imageDims_{800, 600},
    position_{0, 0, 0},
    projPlane_{1, 0, 0},
    up_{0, 0, 1},
    horizontalViewAngle_{75.0 / 180 * M_PI}
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
    double proj_distance = getProjectionDistance();
    projPlane_ = direction_vector;
    setProjectionDistance(proj_distance);
}

double Camera::getProjectionDistance() const
{
    return arma::norm(projPlane_);
}

void Camera::setProjectionDistance(double distance)
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
    return horizontalViewAngle_ / M_PI * 180;
}

void Camera::setFOV(double angle)
{
    horizontalViewAngle_ = angle / 180 * M_PI;
}

arma::vec2 Camera::project(const arma::vec3& point) const
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

PolygonProjection Camera::project(const Polygon& polygon) const
{
    PolygonProjection projection(polygon.nVertices());
    projection.setColor(polygon.getColor());
    
    for (unsigned int i = 0; i < polygon.nVertices(); ++i)
    {
        auto p = polygon.getVertex(i);
        auto p_proj = project(p);
        projection.setVertex(i, sf::Vector2f{(float)p_proj[0], (float)p_proj[1]});
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
