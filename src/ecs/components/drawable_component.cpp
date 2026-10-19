#include "ecs/components/drawable_component.hpp"
#include "ecs/components/physics_component.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include "ecs/entity.hpp"
#include "core/b2sf.hpp"

using namespace sg::ECS::Components;
using namespace sg::Core;

DrawableComponent::DrawableComponent(std::unique_ptr<sf::Drawable> d) : drawable(std::move(d)) {
    transformable = dynamic_cast<sf::Transformable*>(drawable.get());
}


void DrawableComponent::Update(Entity& owner)
{
    if (transformable == nullptr) return;

    auto* phys = owner.GetComponent<PhysicsComponent>();
    if (phys == nullptr) return;

    b2BodyId id = phys->GetBody().GetBodyId();
    if (!b2Body_IsValid(id)) return;

    b2Vec2 p = b2Body_GetPosition(id);
    b2Rot  r = b2Body_GetRotation(id);
    transformable->setPosition(metersToPixels(p));
    transformable->setRotation(sf::radians(b2Rot_GetAngle(r)));
}