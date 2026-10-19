#pragma once

namespace sg::ECS {

class Entity;

// Base class for all components.
class Component {
public:
    virtual ~Component() = default;
    virtual void Update(Entity& owner) {}
};

} // namespace sg::Core