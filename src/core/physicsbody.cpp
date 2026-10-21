#include "physics/physicsbody.hpp"

#include <spdlog/spdlog.h>

using namespace sg::Physics;

void PhysicsBody::CreateBody(bool isStatic, b2Vec2 position, b2WorldId world)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;
    bodyDef.position = position;
    bodyId = b2CreateBody(world, &bodyDef);
}


PhysicsBody::PhysicsBody(bool isStatic, b2Vec2 position, b2WorldId world, const b2Circle& circle)
{
    CreateBody(isStatic, position, world);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreateCircleShape(bodyId, &shapeDef, &circle);
}

PhysicsBody::PhysicsBody(bool isStatic, b2Vec2 position, b2WorldId world, const b2Polygon& polygon)
{
    CreateBody(isStatic, position, world);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
}



// PhysicsBody::PhysicsBody(bool isStatic, b2Vec2 pos, b2WorldId worldId, b2Polygon& polygon)
// {
//     this->is_static = isStatic;
//     this->bodyDef = b2DefaultBodyDef();
//     this->bodyDef.position = pos;
//     this->bodyDef.type = is_static ? b2_staticBody : b2_dynamicBody;
//     this->bodyId = b2CreateBody(worldId, &this->bodyDef);
//     this->bodyPoly = polygon;
//     this->bodyShapeDef = b2DefaultShapeDef();
//     this->worldId = worldId;

//     // Set properties for dynamic body
//     if (is_static == false){
//         this->bodyShapeDef.density = 1.0f;
//         this->bodyShapeDef.material.friction = 0.3f;
//     }

//     spdlog::info("Creating polygon shape");
//     b2CreatePolygonShape(this->bodyId, &this->bodyShapeDef, &this->bodyPoly);
// }