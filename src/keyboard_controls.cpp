#include "keyboard_controls.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <armadillo>

KeyboardControlsManager::KeyboardControlsManager(double movement_speed,
                                                 double roll_speed) :
    movementSpeed_(movement_speed),
    rollSpeed_(roll_speed)
{}

double KeyboardControlsManager::getMovementSpeed() const
{
    return movementSpeed_;
}

void KeyboardControlsManager::setMovementSpeed(double distance_per_frame)
{
    movementSpeed_ = distance_per_frame;
}

arma::vec3 KeyboardControlsManager::requestedMovement() const
{
    arma::vec3 rel_movement_vec = {0, 0, 0};
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        rel_movement_vec[2] += 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        rel_movement_vec[2] -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        rel_movement_vec[0] -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        rel_movement_vec[0] += 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        rel_movement_vec[1] -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        rel_movement_vec[1] += 1;
    }

    return arma::normalise(rel_movement_vec) * movementSpeed_;
}

double KeyboardControlsManager::requestedRoll() const
{
    double roll_angle = 0.0;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        roll_angle += rollSpeed_;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        roll_angle -= rollSpeed_;
    }

    return roll_angle;
}
