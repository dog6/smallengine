#pragma once

#include <memory>
#include "ecs/component.hpp"
#include "physics/physicsbody.hpp"


namespace sg::ECS::Components {
    // Owns a physics body.
    class PhysicsComponent : public Component {
    public:
        explicit PhysicsComponent(std::unique_ptr<sg::Physics::PhysicsBody> b);
        PhysicsComponent(bool isStatic, b2Vec2 position, b2WorldId world, const b2Polygon& polygon)
                : body(std::make_unique<sg::Physics::PhysicsBody>(isStatic, position, world, polygon)){};

        sg::Physics::PhysicsBody& GetBody() { return *body; }
        void Update();
    private:
        std::unique_ptr<sg::Physics::PhysicsBody> body;
    };
};