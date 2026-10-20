#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "ecs/component.hpp"
#include "ecs/components/drawable_component.hpp"
#include "ecs/components/physics_component.hpp"
#include "ecs/entity.hpp"

namespace sg::ECS {

    // One row for each component type that Lua and the scene loader can use by name.
    struct ComponentType {
        const char* name;
        Component* (*add)(Entity&);   // returns nullptr if the type needs data
        Component* (*get)(Entity&);
    };

    template<typename T>
    ComponentType MakeType(const char* name)
    {
        return {
            name,
            [](Entity& entity) -> Component* {
                if constexpr (std::is_default_constructible_v<T>) {
                    return entity.AddComponent<T>();
                } else {
                    return nullptr;
                }
            },
            [](Entity& entity) -> Component* { return entity.GetComponent<T>(); }
        };
    }

    inline const std::vector<ComponentType>& GetTypes()
    {
        static const std::vector<ComponentType> types = {
            MakeType<Components::PhysicsComponent>("PhysicsComponent"),
            MakeType<Components::DrawableComponent>("DrawableComponent"),
        };
        return types;
    }

    inline const ComponentType* FindType(const std::string& name)
    {
        for (const ComponentType& type : GetTypes()) {
            if (name == type.name) return &type;
        }
        return nullptr;
    }

}