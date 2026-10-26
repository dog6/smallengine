#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "input/digital_device.hpp"

namespace sg::Input {

class Keyboard : public DigitalDevice<Keyboard, sf::Keyboard::Key> {
    friend DigitalDevice;

    static bool Query(sf::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }
};

}