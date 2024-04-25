#include "scene.hpp"
#include "keyboard_controls.hpp"
#include "mouse_controls.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <random>

sf::Color randomColor()
{
    static std::mt19937 rng(0);
    
    sf::Color clr;
    clr.a = 255;
    clr.r = rng();
    clr.g = rng();
    clr.b = rng();

    return clr;
}

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
    
    Scene scene;
    {
        auto tetrahedron = Object("../scene/tetrahedron.obj");
        for (int i = 0; i < tetrahedron.nPolygons(); ++i)
        {
            auto poly = tetrahedron.getPolygon(i);
            poly.setColor(randomColor());
            tetrahedron.setPolygon(i, poly);
        }
        scene.addObject(tetrahedron);
    }
    scene.camera().setPosition({-5, 0, 0});


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
            case sf::Event::LostFocus:
                mouse_controls.setMouseCapture(false);
                break;
            case sf::Event::GainedFocus:
                mouse_controls.setMouseCapture(true);
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Key::Enter)
                {
                    scene.camera().setWireframe(!scene.camera().isWireframeEnabled());
                }
                break;
            default:
                mouse_controls.handle(event);
                break;
            }
        }

        scene.camera().move(key_controls.requestedMovement());
        scene.camera().yaw(mouse_controls.requestedYaw());
        scene.camera().pitch(mouse_controls.requestedPitch());
        scene.camera().roll(key_controls.requestedRoll());
        scene.camera().setFOV(scene.camera().getFOV() * mouse_controls.requestedZoom());
        mouse_controls.updateMousePosition();

        window.clear();
        window.draw(scene);
        window.display();
    }
}
