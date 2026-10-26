#pragma once

#include <algorithm>
#include <thread>

#include "physics/box2dworld.hpp"
#include "core/task_pool.hpp"   // put TaskPool.hpp in your physics folder
#include "core/smath.hpp"

class PhysicsManager {

    public:
        // Keep the pool BEFORE the world. Static members start in the order
        // of declaration, and they end in the reverse order.
        // The pool starts first and ends last. This is correct.
        inline static sg::TaskPool pool = sg::TaskPool(
            std::clamp(std::thread::hardware_concurrency(), 2u, 8u));

        inline static sg::Physics::Box2DWorld world =
            sg::Physics::Box2DWorld(b2Vec2{0, 9.81f}, &pool);

        static void Update(float delta_time);

};