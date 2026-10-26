#pragma once

#include <random>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#define PIXELS_PER_METER 24 //42 // number of pixels per meter

namespace sg::Core {

    struct vec2 {
        int x;
        int y;
        vec2();
        vec2(int _x, int _y);
    };

    struct uvec2 {
        unsigned int x;
        unsigned int y;
        uvec2();
        uvec2(unsigned int _x, unsigned int _y);
    };

    struct vec2f {
        float x;
        float y;
        vec2f();
        vec2f(sf::Vector2f sfv);
        vec2f(b2Vec2 b2v);
        vec2f(float _x, float _y);
        sf::Vector2f toSFML();
        b2Vec2 toBox2D();
        static vec2f pixelsToMeters(vec2f& v);
        static vec2f metersToPixels(vec2f& v);
        vec2f operator*(float factor) const { return { x * factor, y * factor }; }
        vec2f operator*(const vec2f& factor) const { return { x * factor.x, y * factor.y }; }
        vec2f operator/(float divisor) const { return { x / divisor, y / divisor }; }
        vec2f operator/(const vec2f& divisor) const { return { x / divisor.x, y / divisor.y }; }
    };
    
    inline vec2f operator*(float factor, const vec2f& v) { return v * factor; }

    // min and max are included
    int random_int(int min, int max);

    // min is included, max is not
    float random_float(float min, float max);

    // SFML (Pixels) -> Box2D (Meters)
    inline vec2f pixelsToMeters(const sf::Vector2f& sfVec) {
        return { sfVec.x / PIXELS_PER_METER, sfVec.y / PIXELS_PER_METER };
    }

    // Box2D (Meters) -> SFML (Pixels)
    inline sf::Vector2f metersToPixels(const b2Vec2& b2Vec) {
        return sf::Vector2f(b2Vec.x * PIXELS_PER_METER, b2Vec.y * PIXELS_PER_METER);
    }

    
}
