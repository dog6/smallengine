#pragma once

namespace sg::Core {

class Entity;

// Base class for all components.
class Component {
public:
    virtual ~Component() = default;
    virtual void Update(Entity& owner) {}
};

} // namespace sg::Core