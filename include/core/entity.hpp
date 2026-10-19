#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>
#include "core/component.hpp"

namespace sg::Core {

class Entity {
public:
    explicit Entity(std::string name = "");
    ~Entity();

    // An entity owns its components. Do not copy it.
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    const std::string& GetName() const { return name; }

    // Adds a component. Returns nullptr if the entity already has one of type T.
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        if (HasComponent<T>()) return nullptr;
        auto c = std::make_unique<T>(std::forward<Args>(args)...);
        T* handle = c.get();
        components.emplace_back(std::type_index(typeid(T)), std::move(c));
        return handle;
    }

    // Returns nullptr if the entity has no component of type T.
    template <typename T>
    T* GetComponent() const
    {
        for (const auto& [type, c] : components) {
            if (type == std::type_index(typeid(T))) return static_cast<T*>(c.get());
        }
        return nullptr;
    }

    template <typename T>
    bool HasComponent() const { return GetComponent<T>() != nullptr; }

    // Returns false if the entity has no component of type T.
    template <typename T>
    bool RemoveComponent()
    {
        for (auto it = components.begin(); it != components.end(); ++it) {
            if (it->first == std::type_index(typeid(T))) {
                components.erase(it);  // the component is deleted
                return true;
            }
        }
        return false;
    }

    void Update();

private:
    std::string name;
    std::vector<std::pair<std::type_index, std::unique_ptr<Component>>> components;
};

} // namespace sg::Core