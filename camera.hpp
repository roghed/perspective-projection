#pragma once
#include <armadillo>

class Polygon;
class PolygonProjection;

class Camera
{
public:

    Camera();

    arma::uvec2 getImageDimensions() const;
    void        setImageDimensions(const arma::uvec2& dimensions);

    arma::vec3 getPosition() const;
    void       setPosition(const arma::vec3& position_vector);

    arma::vec3 getDirection() const;
    void       setDirection(const arma::vec3& direction_vector);

    double getNearClippingDistance() const;
    void   setNearClippingDistance(double distance);

    void   pitch(double angle);
    void   yaw(double angle);
    void   roll(double angle);

    double getFOV() const;
    void   setFOV(double angle);

    // move the camera by a vector relative to its own position and orientation:
    // x - left-right movement, perpendicular to the view direction and up 
    //     vector
    // y - up-down movement, along the up vector axis
    // z - forward-backward movement, along the view direction axis
    void move(const arma::vec3& relative_movement);

    arma::vec2        project(const arma::vec3& point) const;
    PolygonProjection project(const Polygon&) const;

private:

    arma::vec3 rotate(const arma::vec3& vector, const arma::vec3& axis, 
                      double angle);
    
    void update();

    arma::uvec2 imageDims_;
    arma::vec3  position_;            // b
    arma::vec3  projPlane_;           // a
    arma::vec3  up_;                  // Z
    double      horizontalViewAngle_; // h
    double      verticalViewAngle_;   // v

    arma::vec3  screenCenter_; // C
    arma::vec3  hScreenVec_;   // u
    arma::vec3  vScreenVec_;   // w
};
