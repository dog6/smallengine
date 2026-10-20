#include "ecs/entity.hpp"
#include "ecs/component_type.hpp"
#include <spdlog/spdlog.h>

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


void Entity::Update()
{
    for (auto& [type, c] : components) {
        c->Update(*this);
    }
}