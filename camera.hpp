#pragma once
#include <armadillo>

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

    double getProjectionDistance() const;
    void   setProjectionDistance(double distance);

    void   rotate(double angle);

    double getFOV() const;
    void   setFOV(double angle);

    arma::vec3 project(arma::vec3 point) const;

private:

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
