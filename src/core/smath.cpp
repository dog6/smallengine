#include "core/smath.hpp"
#include <SFML/Graphics.hpp>

using namespace sg::Core;

vec2f::vec2f()
{
    this->x = 0.0f;
    this->y = 0.0f;
}

sg::Core::vec2f::vec2f(sf::Vector2f sfv)
{
    this->x = sfv.x;
    this->y = sfv.y;
}

sg::Core::vec2f::vec2f(b2Vec2 b2v)
{
    this->x = b2v.x;
    this->y = b2v.y;
}

vec2f::vec2f(float _x, float _y)
{
    this->x = _x;
    this->y = _y;
}

sf::Vector2f sg::Core::vec2f::toSFML() { return sf::Vector2f(this->x, this->y); }
b2Vec2 sg::Core::vec2f::toBox2D() { return b2Vec2{this->x, this->y}; }

// Pixels -> Meter
vec2f sg::Core::vec2f::pixelsToMeters(vec2f &v) { return { v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER }; }

// Meter -> Pixels
vec2f sg::Core::vec2f::metersToPixels(vec2f &v)  { return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER); }


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


int sg::Core::random_int(int min, int max)
{
    static std::mt19937 engine{std::random_device{}()};
    std::uniform_int_distribution<int> dist(min, max);   
    return dist(engine);
}

float sg::Core::random_float(float min, float max)
{
    static std::mt19937 engine{std::random_device{}()};
    std::uniform_real_distribution<float> dist(min, max);   
    return dist(engine);
}
