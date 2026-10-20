#pragma once

#include <memory>
#include <vector>

#include "core/list.hpp"
#include "ecs/entity.hpp"

namespace sg::ECS {

    class EntityManager {
        
        public:
            inline static List<sg::ECS::Entity> entman;
            

    };

}