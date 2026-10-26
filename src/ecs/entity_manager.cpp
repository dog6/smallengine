#include "ecs/entity_manager.hpp"
#include <spdlog/spdlog.h>
#include "core/smath.hpp"
#include <algorithm>

using namespace sg::ECS;

Entity* EntityManager::MakeEntity(std::string name)
{
    spdlog::info("Making entity {}", name);
    
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);
    return ent.get();
}

std::size_t EntityManager::RemoveEntity(int index)
{
    spdlog::info("Removing entity at index {}", index);
    
    auto it = entman.erase(entman.begin()+index);
    return  sizeof(it)/sizeof(Entity);
}

std::unique_ptr<Entity>& EntityManager::AddEntity(std::unique_ptr<Entity> ent)
{
    spdlog::info("Adding entity {}", ent.get()->GetName());
    ent->SetId(entman.size()+1);
    return EntityManager::entman.emplace_back(std::move(ent));
}

Entity *sg::ECS::EntityManager::GetEntity(int entity_id)
{
    if (entity_id < entman.size() && entity_id >= 0) {

        auto is_equal = [entity_id](const std::unique_ptr<sg::ECS::Entity>& ent) { 
            return ent->GetId() == entity_id; 
        };

        // find_if entity_id == iterator entity id
        std::vector<std::unique_ptr<sg::ECS::Entity>>::iterator it = std::find_if(entman.begin(), entman.end(), is_equal);

        if (it != entman.end()){
            return it->get();
        }

    }
    return nullptr;
    // return entman[entity_id];
}

bool sg::ECS::EntityManager::HasEntity(int entity_id)
{

    // lambda predicate
    auto is_equal = [entity_id](const std::unique_ptr<sg::ECS::Entity>& ent) { 
        return ent->GetId() == entity_id; 
    };

    // find_if entity_id == iterator entity id
    std::vector<std::unique_ptr<sg::ECS::Entity>>::iterator it = std::find_if(entman.begin(), entman.end(), is_equal);

    return it != entman.end();

}

void EntityManager::Update()
{
    for(auto& e : entman) {
        auto ent = e.get();
        // need function to find entity by ID
        ent->Update();
    }
}
