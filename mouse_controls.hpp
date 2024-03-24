#pragma once
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class Event;
    class Window;
}

class MouseControlsManager
{
public:

    MouseControlsManager(sf::Window& parent_window,
                         double mouse_sensitivity = 1.0, 
                         double zoom_sensitivity  = 0.1);

    double getMouseSensitivity() const;
    void   setMouseSensitivity(double value);

    double getZoomSensitivity() const;
    void   setZoomSensitivity(double percentage_per_scroll);

    bool getMouseCapture() const;
    void setMouseCapture(bool enabled);

    void handle(const sf::Event&);

    void updateMousePosition();

    double requestedYaw() const;
    double requestedPitch() const;
    double requestedZoom() const;

private:

    sf::Vector2i mousePositionDelta() const;

    sf::Window& window_;
    bool mouseCapture_;

    double mouseSensitivity_;
    double zoomSensitivity_;

    sf::Vector2i lastMousePosition_;
    int          accumulatedScrollDelta_;
};
