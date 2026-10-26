#include "ecs/components/physics_component.hpp"
#include "box2d/box2d.h"

using namespace sg::ECS::Components;
using namespace sg::Physics;

PhysicsComponent::PhysicsComponent(std::unique_ptr<PhysicsBody> b)
    : body(std::move(b)) {}

void PhysicsComponent::Update()
{

}

void sg::ECS::Components::PhysicsComponent::SetPosition(sg::Core::vec2f pos)
{
    
}
