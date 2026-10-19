#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

// contains conversion helpers

namespace sg::Core {

    // 24" @ 1080p = 3622 PPM
    // 24" @ 4K    = 7227 PPM
    // 27" @ 1440p = 4291 PPM
    // 27" @ 1080p = 3228 PPM
    // 27" @ 4K    = 6424 PPM

    constexpr float PIXELS_PER_METER = 42; // pixels per meter 
    
    // SFML (Pixels) -> Box2D (Meters)
    inline b2Vec2 pixelsToMeters(const sf::Vector2f& sfVec) {
        return { sfVec.x / PIXELS_PER_METER, sfVec.y / PIXELS_PER_METER };
    }

    // Box2D (Meters) -> SFML (Pixels)
    inline sf::Vector2f metersToPixels(const b2Vec2& b2Vec) {
        return sf::Vector2f(b2Vec.x * PIXELS_PER_METER, b2Vec.y * PIXELS_PER_METER);
    }

}