#include "polygon.hpp"
#include "polygon_projection.hpp"
#include "camera.hpp"
#include "keyboard_controls.hpp"
#include "mouse_controls.hpp"
#include "bsp_tree.hpp"
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
    
    std::vector<Polygon> scene;

    Polygon triangle1(3);
    triangle1.setVertex(0, {3,   -1,  1});
    triangle1.setVertex(1, {4,    1,  1});
    triangle1.setVertex(2, {3.5,  0, -1});
    triangle1.setColor(sf::Color::Red);
    scene.push_back(triangle1);

    Polygon triangle2(3);
    triangle2.setVertex(0, {5,   -1,  1});
    triangle2.setVertex(1, {6,    1,  1});
    triangle2.setVertex(2, {5.5,  0, -1});
    triangle2.setColor(sf::Color::Blue);
    scene.push_back(triangle2);

    Polygon tetrahedron1(3);
    tetrahedron1.setVertex(0, {1, 1, 1});
    tetrahedron1.setVertex(1, {-1, -1, 1});
    tetrahedron1.setVertex(2, {-1, 1, -1});
    scene.push_back(tetrahedron1);

    Polygon tetrahedron2(3);
    tetrahedron2.setVertex(0, {1, 1, 1});
    tetrahedron2.setVertex(1, {-1, -1, 1});
    tetrahedron2.setVertex(2, {1, -1, -1});
    tetrahedron2.setColor(sf::Color::Green);
    scene.push_back(tetrahedron2);

    Polygon tetrahedron3(3);
    tetrahedron3.setVertex(0, {1, 1, 1});
    tetrahedron3.setVertex(1, {1, -1, -1});
    tetrahedron3.setVertex(2, {-1, 1, -1});
    tetrahedron3.setColor(sf::Color::Yellow);
    scene.push_back(tetrahedron3);

    Polygon tetrahedron4(3);
    tetrahedron4.setVertex(0, {-1, -1, 1});
    tetrahedron4.setVertex(1, {1, -1, -1});
    tetrahedron4.setVertex(2, {-1, 1, -1});
    tetrahedron4.setColor(sf::Color::Magenta);
    scene.push_back(tetrahedron4);

    BSPTree bsp_tree(scene);
    
    Camera camera;
    camera.setWireframe(true);

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
                    camera.setWireframe(!camera.isWireframeEnabled());
                }
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

        auto sorted_polygons = bsp_tree.depthSortedPolygons(camera.getPosition());
        for (auto poly_ptr : sorted_polygons)
        {
            window.draw(camera.project(*poly_ptr));
        }

        window.display();
    }
}
