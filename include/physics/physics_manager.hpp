#pragma once

#include "physics/box2dworld.hpp"
#include "core/smath.hpp"

class PhysicsManager {

    public:
        inline static sg::Physics::Box2DWorld world = sg::Physics::Box2DWorld(b2Vec2{0, 9.81f});
        static void Update(float delta_time);

};