#pragma once

#include <memory>
#include <vector>

#include "core/list.hpp"
#include "ecs/entity.hpp"

namespace sg::ECS {

    class EntityManager {
        
        public:
            inline static List<sg::ECS::Entity> entman;
            
            // Creates new entity and stores in entman
            // Returns pointer to newly created entity
            static Entity* MakeEntity(std::string name);

            // Removes an entity from the entman via index
            // Returns new entity size_t if item removed, otherwise returns -1
            static std::size_t RemoveEntity(int index);

    };

}