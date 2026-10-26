#pragma once

#include <memory>
#include "ecs/component.hpp"
#include "physics/physicsbody.hpp"
#include "core/smath.hpp"

namespace sg::ECS::Components {
    // Owns a physics body.
    class PhysicsComponent : public Component {
    public:
        explicit PhysicsComponent(std::unique_ptr<sg::Physics::PhysicsBody> b);

        PhysicsComponent(bool isStatic, b2Vec2 position, b2WorldId world, const b2Polygon& polygon)
                : body(std::make_unique<sg::Physics::PhysicsBody>(isStatic, position, world, polygon)) {}

        PhysicsComponent(bool isStatic, b2Vec2 position, b2WorldId world, const b2Circle& circle)
                : body(std::make_unique<sg::Physics::PhysicsBody>(isStatic, position, world, circle)) {}

        sg::Physics::PhysicsBody& GetBody() { return *body; }
        void Update();
        void SetPosition(sg::Core::vec2f pos);
        sg::Core::vec2f GetPosition();

    private:
        std::unique_ptr<sg::Physics::PhysicsBody> body;
    };
};