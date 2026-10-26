#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowBase.hpp>

#include "core/smath.hpp"
#include "input/digital_device.hpp"

namespace sg::Input {

class Mouse : public DigitalDevice<Mouse, sf::Mouse::Button> {
public:
    static Core::vec2f Position(const sf::WindowBase& window)
    {
        return Core::vec2f(sf::Vector2f(sf::Mouse::getPosition(window)));
    }

    static Core::vec2f ScaledPosition(const sf::WindowBase& window, const Core::vec2f& scale)
    {
        return Position(window) * scale;
    }

private:
    friend DigitalDevice;

    static bool Query(sf::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(button);
    }
};

}