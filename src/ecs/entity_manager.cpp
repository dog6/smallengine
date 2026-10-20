#include "ecs/entity_manager.hpp"

using namespace sg::ECS;

Entity* EntityManager::MakeEntity(std::string name)
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);
    return ent.get();
}

std::size_t EntityManager::RemoveEntity(int index)
{
    if (EntityManager::entman.Remove(index)) {
        // index was valid, item removed
        return EntityManager::entman.Size();        
    }else {
        return -1;
    }
}
