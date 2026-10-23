#include "physics/physics_manager.hpp"

void PhysicsManager::Update(float delta_time)
{
    b2World_Step(world.worldId, 1.0f/60.0f, 4);
}