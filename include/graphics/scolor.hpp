#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>

namespace sg::Graphics {

    struct ColorRGB {
        uint8_t r, g, b;
        sf::Color toSFColor() { return sf::Color(r,g,b, 255); }
        ColorRGB(uint8_t _r, uint8_t _g, uint8_t _b);
        uint8_t Red() { return r; }
        uint8_t Green() { return g; }
        uint8_t Blue() { return b; }
        
    };

    struct ColorRGBA {
        uint8_t r, g, b, a;
        uint8_t Red() { return r; }
        uint8_t Green() { return g; }
        uint8_t Blue() { return b; }
        uint8_t Alpha() { return a; }
        ColorRGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);
        sf::Color toSFColor() { return sf::Color(r,g,b,a); }
        
    };

};