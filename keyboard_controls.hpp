#pragma once
#include <armadillo>
#include <SFML/Window/Event.hpp>

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
