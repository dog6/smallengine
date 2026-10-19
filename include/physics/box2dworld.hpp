#pragma once
#include <box2d/box2d.h>

struct Box2DWorld {
    b2WorldDef worldDef;
    b2WorldId worldId;
    Box2DWorld(b2Vec2 _gravity);
};