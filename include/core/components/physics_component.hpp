#pragma once

#include <memory>
#include "core/component.hpp"
#include "core/physicsbody.hpp"

namespace sg::Core {
    // Owns a physics body.
    class PhysicsComponent : public Component {
    public:
        explicit PhysicsComponent(std::unique_ptr<PhysicsBody> b);
        PhysicsComponent(bool isStatic, b2Vec2 position, b2WorldId world, const b2Polygon& polygon)
                : body(std::make_unique<PhysicsBody>(isStatic, position, world, polygon)){};

        PhysicsBody& GetBody() { return *body; }
        void Update();
    private:
        std::unique_ptr<PhysicsBody> body;
    };
};