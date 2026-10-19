#include "physics/box2dworld.hpp"


// b2CreateWorld using b2DefaultWorldDef
Box2DWorld::Box2DWorld(b2Vec2 _gravity)
{
    this->worldDef = b2DefaultWorldDef();
    this->worldDef.gravity = _gravity;
    this->worldId = b2CreateWorld(&worldDef);
}
