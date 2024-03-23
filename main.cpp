#include "polygon.hpp"
#include "polygon_projection.hpp"
#include "camera.hpp"
#include <SFML/Graphics.hpp>

int main() 
{
    constexpr unsigned int WIDTH = 800, HEIGHT = 600;
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "perspective-projection");
    window.setFramerateLimit(60);
    Polygon triangle(3);
    triangle.setVertex(0, {3,   -1,  1});
    triangle.setVertex(1, {4,    1,  1});
    triangle.setVertex(2, {3.5,  0, -1});
    triangle.setColor(sf::Color::Red);
    Camera camera;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.draw(camera.project(triangle));
        window.display();
    }
}
