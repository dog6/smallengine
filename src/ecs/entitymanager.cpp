#include "ecs/entity_manager.hpp"
#include "ecs/entity.hpp"

#include <algorithm>
#include <memory>
#include <spdlog/spdlog.h>

using namespace sg::ECS;

EntityManager::EntityManager()
{
    entities.reserve(MAX_ENTITY_COUNT);
}

EntityManager::~EntityManager() = default;  // unique_ptr deletes the entities

Entity* EntityManager::AddEntity(Entity* e)  // takes ownership
{
    if (e == nullptr) {
        spdlog::error("AddEntity() called with a null pointer");
        return nullptr;
    }

    auto it = std::find_if(entities.begin(), entities.end(),
        [e](const std::unique_ptr<Entity>& p) { return p.get() == e; });
    if (it != entities.end()) {
        spdlog::warn("AddEntity() called with an entity that is already stored");
        return e;
    }

    entities.push_back(std::unique_ptr<Entity>(e));
    return e;
}

bool EntityManager::RemoveEntity(int index)
{
    if (index < 0 || index >= static_cast<int>(entities.size())) {
        spdlog::error("RemoveEntity() called with index out of bounds {}", index);
        return false;
    }

    entities.erase(entities.begin() + index);  // the entity is deleted
    return true;
}

void EntityManager::Update()
{
    for (auto& ent : entities) {
        if (ent) ent->Update();
    }
}