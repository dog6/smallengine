#pragma once

#include <memory>
#include "ecs/entity.hpp"
#include "ecs/components/physics_component.hpp"
#include "ecs/components/drawable_component.hpp"
#include "graphics/shape_factory.hpp"

#include "box2d/box2d.h"

using namespace sg::ECS;
using namespace sg::ECS::Components;

namespace sg::Content {

    class EntityFactory {


        public:
        // Create a circle entity. Radius and position are in pixels.
        static std::unique_ptr<Entity> CreateCircleEntityPixels(std::string name, b2WorldId worldId, sf::Vector2f pixel_pos,
                                        float pixel_radius, sf::Color col, bool isStatic);

        // Same as above, but position and radius are in meters.
        static std::unique_ptr<Entity> CreateCircleEntityMeters(std::string name, b2WorldId worldId, b2Vec2 meter_pos,
                                        float meter_radius, sf::Color col, bool isStatic);


        // We need somewhere to store CreateRectEntity(), and ideally change it to be more generic
        static std::unique_ptr<Entity> CreateRectEntityPixels(std::string name, b2WorldId worldId, sf::Vector2f pixel_pos,
                                        sf::Vector2f pixel_size, sf::Color col, bool isStatic);

        // Same as above, but takes in meter pos/size instead of pixel size
        static std::unique_ptr<Entity> CreateRectEntityMeters(std::string name, b2WorldId worldId, b2Vec2 meter_pos,
                                        b2Vec2 meter_size, sf::Color col, bool isStatic);


    };

};