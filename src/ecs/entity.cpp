#include "ecs/entity.hpp"

using namespace sg::ECS;

Entity::Entity(std::string n) : name(std::move(n)) {}
Entity::~Entity() = default;

void Entity::Update()
{
    for (auto& [type, c] : components) {
        c->Update(*this);
    }
}