#include "ecs/entity.hpp"
#include "ecs/component_type.hpp"
#include <spdlog/spdlog.h>
#include "core/smath.hpp"

using namespace sg::ECS;

Entity::Entity(std::string n) : name(std::move(n)) {}
Entity::~Entity() = default;

Component *Entity::AddComponentByName(const std::string &type)
{
    spdlog::info("AddComponent {} to {}", type.c_str(), this->name);
    const ComponentType* t = FindType(type);
    if (t == nullptr) {
        spdlog::warn("Failed to add component type {}", type);
        return nullptr;
    }

    return t->add(*this);
}

Component* Entity::GetComponentByName(const std::string& type)
{
    const ComponentType* t = FindType(type);
    if (t == nullptr) {
        spdlog::info("GetComponentByName: unknown component type {}", type);
        return nullptr;
    }

    Component* comp = t->get(*this);

    if (!comp) {
        spdlog::warn("GetComponent '{}' failed on object {}", type, this->name);
        for (auto& c : components)
            spdlog::info("{}", typeid(&c).name());
    }

    return comp;
}

void Entity::SetPosition(sg::Core::vec2f pos)
{
    auto* physComp =
        GetComponent<sg::ECS::Components::PhysicsComponent>();

    if (!physComp)
    {
        spdlog::warn(
            "No physics component attached to entity '{}'",
            GetName()
        );
        return;
    }

    b2BodyId bid = physComp->GetBody().GetBodyId();

    // Lua supplies pixels; Box2D expects meters.
    const b2Vec2 box2DPosition{
        pos.x / PIXELS_PER_METER,
        pos.y / PIXELS_PER_METER
    };

    b2Body_SetTransform(
        bid,
        box2DPosition,
        b2Body_GetRotation(bid)
    );
}

sg::Core::vec2f Entity::GetPosition()
{
    auto* physComp =
        GetComponent<sg::ECS::Components::PhysicsComponent>();

    if (!physComp)
    {
        spdlog::warn(
            "No physics component attached to entity '{}'",
            GetName()
        );
        return { 0, 0 };
    }

    b2BodyId bid = physComp->GetBody().GetBodyId();
    const b2Vec2 position = b2Body_GetPosition(bid);

    // Box2D supplies meters; Lua expects pixels.
    return {
        position.x * PIXELS_PER_METER,
        position.y * PIXELS_PER_METER
    };
}

void Entity::Update()
    {
        for (auto &[type, c] : components)
        {
            c->Update(*this);
        }
}