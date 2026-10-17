// #include  "core/smath.hpp"
#include "core/smath.hpp"

vec2f::vec2f()
{
    this->x = 0.0f;
    this->y = 0.0f;
}

vec2f::vec2f(float _x, float _y)
{
    this->x = _x;
    this->y = _y;
}

vec2::vec2()
{
    this->x = 0;
    this->y = 0;
}

vec2::vec2(int _x, int _y)
{
    this->x = _x;
    this->y = _y;
}

uvec2::uvec2()
{
    this->x = 0;
    this->y = 0;
}

uvec2::uvec2(unsigned int _x, unsigned int _y) {
    this->x = _x;
    this->y = _y;
}
