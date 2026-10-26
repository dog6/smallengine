#pragma once
#include <box2d/box2d.h>
#include "core/task_pool.hpp"

namespace sg::Physics {

    struct Box2DWorld {
        b2WorldDef worldDef;
        b2WorldId worldId;
        // Box2DWorld(b2Vec2 _gravity);
        Box2DWorld(b2Vec2 _gravity, TaskPool* pool);
    };

};