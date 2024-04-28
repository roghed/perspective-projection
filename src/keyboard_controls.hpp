#pragma once
#include <armadillo>

class KeyboardControlsManager
{
public:

    KeyboardControlsManager(double movement_speed = 0.1, 
                            double roll_speed = 0.01);

    double getMovementSpeed() const;
    void   setMovementSpeed(double distance_per_frame);

    arma::vec3 requestedMovement() const;
    double     requestedRoll() const;

private:

    double movementSpeed_;
    double rollSpeed_;
};
