#pragma once

#include <memory>
#include <vector>

#include "core/entity.hpp"

namespace sg::Core {

    constexpr size_t MAX_ENTITY_COUNT = 100;

    class EntityManager {
    public:
        EntityManager();
        ~EntityManager();

        // The manager can move, but it cannot copy.
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager(EntityManager&&) = default;
        EntityManager& operator=(EntityManager&&) = default;

        // Takes ownership of the entity. Do not call delete on it.
        // Returns nullptr if the pointer is null.
        Entity* AddEntity(Entity* entity);

        bool RemoveEntity(int index);

        // Returns nullptr if the index is out of bounds.
        Entity* GetEntity(int index);

        // Calls Update() on all entities.
        void Update();

        // Read-only access for the renderer.
        const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return entities; }

    private:
        // The manager owns the entities.
        std::vector<std::unique_ptr<Entity>> entities;
    };

}