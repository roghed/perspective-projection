#include "mouse_controls.hpp"
#include <SFML/Window/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>

MouseControlsManager::MouseControlsManager(sf::Window& parent_window,
                                           double mouse_sensitivity, 
                                           double zoom_sensitivity) :
    window_(parent_window),
    mouseSensitivity_(mouse_sensitivity),
    zoomSensitivity_(zoom_sensitivity)
{
    updateMousePosition();
}

double MouseControlsManager::getMouseSensitivity() const
{
    return mouseSensitivity_;
}

void MouseControlsManager::setMouseSensitivity(double value)
{
    mouseSensitivity_ = value;
}

double MouseControlsManager::getZoomSensitivity() const
{
    return zoomSensitivity_;
}

void MouseControlsManager::setZoomSensitivity(double percentage_per_scroll)
{
    zoomSensitivity_ = percentage_per_scroll;
}

bool MouseControlsManager::getMouseCapture() const
{
    return mouseCapture_;
}

void MouseControlsManager::setMouseCapture(bool enabled)
{
    mouseCapture_ = enabled;
}

void MouseControlsManager::handle(const sf::Event& event)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        accumulatedScrollDelta_ += event.mouseWheelScroll.delta;
    }
}

void MouseControlsManager::updateMousePosition()
{
    accumulatedScrollDelta_ = 0;
    
    if (mouseCapture_)
    {
        sf::Vector2i window_center = window_.getPosition()
                                     + sf::Vector2i(window_.getSize()) / 2;
        sf::Mouse::setPosition(window_center);
    }
    
    lastMousePosition_ = sf::Mouse::getPosition();
}

double MouseControlsManager::requestedYaw() const
{
    return -mousePositionDelta().x * mouseSensitivity_; 
}

double MouseControlsManager::requestedPitch() const
{
    return -mousePositionDelta().y * mouseSensitivity_;
}

double MouseControlsManager::requestedZoom() const
{
    double zoom_multiplier = 1.0 + zoomSensitivity_;

    if (accumulatedScrollDelta_ < 0)
    {
        return std::pow(zoom_multiplier, -accumulatedScrollDelta_);
    }
    else if (accumulatedScrollDelta_ > 0)
    {
        return std::pow(1.0 / zoom_multiplier, accumulatedScrollDelta_);
    }

    return 1.0;
}

sf::Vector2i MouseControlsManager::mousePositionDelta() const
{
    return sf::Mouse::getPosition() - lastMousePosition_;
}
