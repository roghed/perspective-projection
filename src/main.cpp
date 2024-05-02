#include "scene.hpp"
#include "object.hpp"
#include "polygon.hpp"
#include "camera.hpp"
#include "keyboard_controls.hpp"
#include "mouse_controls.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <random>
#include <cmath>
#include <string>

sf::Color randomColor()
{
    static std::mt19937 rng(0);
    
    sf::Color clr;
    clr.r = rng();
    clr.g = rng();
    clr.b = rng();
    clr.a = 255;
    
    return clr;
}

int main() 
{
    constexpr unsigned int WIDTH = 1280, HEIGHT = 720;
    constexpr double MOUSE_SENSITIVITY = 0.000025;
    constexpr double MOVEMENT_SPEED = 0.02;
    constexpr double ROLL_SPEED = 0.001;
    constexpr double ZOOM_SENSITIVITY = 0.01;
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "perspective-projection");
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    
    Scene scene;
    {
        auto tetrahedron = Object("scene/tetrahedron.obj");
        for (int i = 0; i < tetrahedron.nPolygons(); ++i)
        {
            auto poly = tetrahedron.getPolygon(i);
            poly.setColor(randomColor());
            tetrahedron.setPolygon(i, poly);
        }
        tetrahedron.setOrigin({0, -5, 0});
        scene.addObject(tetrahedron);
    }

    {
        auto cube = Object("scene/cube.obj");
        for (int i = 0; i < cube.nPolygons(); ++i)
        {
            auto poly = cube.getPolygon(i);
            poly.setColor(randomColor());
            cube.setPolygon(i, poly);
        }
        cube.setOrigin({0, 0, 0});
        scene.addObject(cube);
    }

    {
        auto cube = Object("scene/cube.obj");
        for (int i = 0; i < cube.nPolygons(); ++i)
        {
            auto poly = cube.getPolygon(i);
            poly.setColor(randomColor());
            cube.setPolygon(i, poly);
        }
        cube.setOrigin({0, 2, 0});
        scene.addObject(cube);
    }

    {
        auto cube = Object("scene/cube.obj");
        for (int i = 0; i < cube.nPolygons(); ++i)
        {
            auto poly = cube.getPolygon(i);
            poly.setColor(randomColor());
            cube.setPolygon(i, poly);
        }
        cube.setOrigin({2, 0, 0});
        scene.addObject(cube);
    }

    {
        auto cube = Object("scene/cube.obj");
        for (int i = 0; i < cube.nPolygons(); ++i)
        {
            auto poly = cube.getPolygon(i);
            poly.setColor(randomColor());
            cube.setPolygon(i, poly);
        }
        cube.setOrigin({2, 2, 0});
        scene.addObject(cube);
    }

    {
        auto cyclic_occlusion = Object("scene/cyclic_occlusion.obj");
        for (int i = 0; i < cyclic_occlusion.nPolygons(); ++i)
        {
            auto poly = cyclic_occlusion.getPolygon(i);
            poly.setColor(randomColor());
            cyclic_occlusion.setPolygon(i, poly);
        }
        cyclic_occlusion.setOrigin({0, 4, 0});
        scene.addObject(cyclic_occlusion);
    }

    scene.camera().setPosition({-3, 0, 0});
    scene.camera().setNearClippingDistance(0.05);

    MouseControlsManager mouse_controls(window, MOUSE_SENSITIVITY, ZOOM_SENSITIVITY);
    mouse_controls.setMouseCapture(true);

    KeyboardControlsManager key_controls(MOVEMENT_SPEED, ROLL_SPEED);

    sf::Font noto_serif_font;
    noto_serif_font.loadFromFile("data/NotoSerifCJK-Regular.ttc");
    sf::Text fov_indicator("", noto_serif_font, 16);
    fov_indicator.setPosition(sf::Vector2f{10, 10});

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
                if (event.key.code == sf::Keyboard::Key::B)
                {
                    scene.setColorPolygonsByDrawingOrder(!scene.colorPolygonsByDrawingOrder());
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
        int fov_rounded = std::round(scene.camera().getFOV());
        std::wstring fov_string = std::to_wstring(fov_rounded);
        fov_indicator.setString(L"FOV: " + fov_string + L"°");
        window.draw(fov_indicator);

        window.display();
    }
}
