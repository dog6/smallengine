#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

// contains conversion helpers

namespace sg::Core {

    constexpr float PPM = 10.0f; // pixels per meter  // approx. real scale with 27 inch monitor
    
    inline b2Vec2 toBox2D(const sf::Vector2f& sfVec) {
        return { sfVec.x / PPM, sfVec.y / PPM };
    }

    // Box2D (Meters) -> SFML (Pixels)
    inline sf::Vector2f toSFML(const b2Vec2& b2Vec) {
        return sf::Vector2f(b2Vec.x * PPM, b2Vec.y * PPM);
    }

}