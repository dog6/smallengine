#include "ecs/components/physics_component.hpp"
#include "box2d/box2d.h"

using namespace sg::ECS::Components;
using namespace sg::Physics;

PhysicsComponent::PhysicsComponent(std::unique_ptr<PhysicsBody> b)
    : body(std::move(b)) {}

void PhysicsComponent::Update()
{


    // auto b = b2Shape_GetBody(this->body.get()->GetBodyId());
    // this->body.get()->GetBodyId;
    // this->body.get()->

    // b2BodyId bid = body.get()->GetBodyId();
    // b2Vec2 position = b2Body_GetPosition(bid);
    // b2Rot rotation = b2Body_GetRotation(bid);
    // float angle = b2Rot_GetAngle(rotation);

    // Apply position / rotation to drawable component
    
    


    // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, b2Rot_GetAngle(rotation));
}
