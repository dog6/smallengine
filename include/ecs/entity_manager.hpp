#pragma once

#include <memory>
// #include <vector>
#include <unordered_map>

#include "ecs/entity.hpp"
#include <bits/stdc++.h>

namespace sg::ECS {

    class EntityManager {
            
        public:
        
            inline static std::vector<std::unique_ptr<sg::ECS::Entity>> entman;
            // inline static std::unordered_map<std::hash<std::unique_ptr<sg::ECS::Entity>>, std::unique_ptr<sg::ECS::Entity>> entman;


            // Creates new entity and stores in entman
            // Returns pointer to newly created entity
            static Entity* MakeEntity(std::string name);

            // Removes an entity from the entman via index
            // Returns new entity size_t if item removed, otherwise returns -1
            static std::size_t RemoveEntity(int index);
            
            static std::unique_ptr<Entity>& AddEntity(std::unique_ptr<Entity> ent);
            
            static Entity* GetEntity(int entity_id);
            static bool HasEntity(int entity_id);

            static void Update();

    };

}