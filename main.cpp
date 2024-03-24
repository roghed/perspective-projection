#include "polygon.hpp"
#include "polygon_projection.hpp"
#include "camera.hpp"
#include "keyboard_controls.hpp"
#include "mouse_controls.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() 
{
    constexpr unsigned int WIDTH = 800, HEIGHT = 600;
    constexpr double MOUSE_SENSITIVITY = 0.000025;
    constexpr double MOVEMENT_SPEED = 0.1;
    constexpr double ROLL_SPEED = 0.001;
    constexpr double ZOOM_SENSITIVITY = 0.01;
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "perspective-projection");
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    
    Polygon triangle(3);
    triangle.setVertex(0, {3,   -1,  1});
    triangle.setVertex(1, {4,    1,  1});
    triangle.setVertex(2, {3.5,  0, -1});
    triangle.setColor(sf::Color::Red);
    
    Camera camera;

    MouseControlsManager mouse_controls(window, MOUSE_SENSITIVITY, ZOOM_SENSITIVITY);
    mouse_controls.setMouseCapture(true);

    KeyboardControlsManager key_controls(MOVEMENT_SPEED, ROLL_SPEED);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                mouse_controls.handle(event);
                break;
            }
        }

        camera.move(key_controls.requestedMovement());
        camera.yaw(mouse_controls.requestedYaw());
        camera.pitch(mouse_controls.requestedPitch());
        camera.roll(key_controls.requestedRoll());
        camera.setFOV(camera.getFOV() * mouse_controls.requestedZoom());
        mouse_controls.updateMousePosition();

        window.clear();
        window.draw(camera.project(triangle));
        window.display();
    }
}
