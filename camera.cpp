#include "camera.hpp"
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

void Camera::rotate(double angle)
{
    double rad_angle = angle / 180 * M_PI;
    arma::vec3 k = arma::normalise(projPlane_);
    arma::vec3 v = arma::normalise(arma::cross(arma::cross(k, up_), k));
    arma::vec3 v_rotated = 
            v * std::cos(rad_angle) + arma::cross(k, v) * std::sin(rad_angle)
            + k * (k * v) * (1.0 - std::cos(rad_angle));
    up_ = v_rotated;
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

arma::vec3 Camera::project(arma::vec3 point) const
{

    double alpha = arma::dot(projPlane_, projPlane_)
                   / arma::dot(projPlane_, point - position_);
    arma::vec3 c = alpha * (point - position_) - projPlane_;
    arma::vec2 point_projection = {
        dot(c, hScreenVec_) / dot(hScreenVec_, hScreenVec_), 
        dot(c, vScreenVec_) / dot(vScreenVec_, vScreenVec_)};

    return point_projection;
}

void Camera::update()
{
    arma::vec3 U = arma::cross(projPlane_, up_);
    arma::vec3 W = arma::cross(U, projPlane_);
    screenCenter_ = projPlane_ + position_;
    hScreenVec_ = 
            arma::normalise(U) * arma::norm(projPlane_)
            * std::tan(horizontalViewAngle_);
    vScreenVec_ = 
            arma::normalise(W) * arma::norm(hScreenVec_)
            * imageDims_[1] / imageDims_[0];
}
