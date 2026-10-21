#pragma once

#include <memory>
#include "ecs/entity.hpp"
#include "ecs/components/physics_component.hpp"
#include "ecs/components/drawable_component.hpp"
#include "graphics/shape_factory.hpp"

#include "core/smath.hpp"
#include "box2d/box2d.h"

using namespace sg::Core;
using namespace sg::ECS;
using namespace sg::ECS::Components;

#define USE_METER_SCALE true
#define USE_PIXEL_SCALE false
#define STATIC_BODY true
#define DYNAMIC_BODY false

namespace sg::Content {

    class EntityFactory {


        public:
        static std::unique_ptr<Entity> CreateCircleEntity(std::string name, b2WorldId worldId, vec2f pos,
                                        float radius, sf::Color col, bool isStatic, bool useMeterScale);

        static std::unique_ptr<Entity> CreateRectEntity(std::string name, b2WorldId worldId, vec2f pos,
                                        vec2f size, sf::Color col, bool isStatic, bool useMeterScale);




    };

};