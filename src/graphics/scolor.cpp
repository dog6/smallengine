#include "graphics/scolor.hpp"

using namespace sg::Graphics;

ColorRGB::ColorRGB(uint8_t _r, uint8_t _g, uint8_t _b)
{
    this->r = _r;
    this->g = _g;
    this->b = _b;
}

ColorRGBA::ColorRGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
{
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
}
