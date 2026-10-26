#include "ecs/entity.hpp"
#include "ecs/component_type.hpp"
#include <spdlog/spdlog.h>
#include "core/smath.hpp"

using namespace sg::ECS;

Entity::Entity(std::string n) : name(std::move(n)) {}
Entity::~Entity() = default;

    Component *Entity::AddComponentByName(const std::string &type)
    {
        spdlog::info("AddComponent {} to {}", type.c_str(), this->name);
        const ComponentType* t = FindType(type);
        if (t == nullptr) {
            spdlog::warn("Failed to add component type {}", type);
            return nullptr;
        }

        return t->add(*this);
    }

    Component* Entity::GetComponentByName(const std::string& type)
    {
        const ComponentType* t = FindType(type);
        if (t == nullptr) {
            spdlog::info("GetComponentByName: unknown component type {}", type);
            return nullptr;
        }

        Component* comp = t->get(*this);

        if (!comp) {
            spdlog::warn("GetComponent '{}' failed on object {}", type, this->name);
            for (auto& c : components)
                spdlog::info("{}", typeid(&c).name());
        }

        return comp;
    }

    void sg::ECS::Entity::SetPosition(sg::Core::vec2f pos)
    {
        auto* drawableComp = this->GetComponent<sg::ECS::Components::DrawableComponent>();
        if (drawableComp != nullptr) {
            
            sf::Drawable* d = drawableComp->GetDrawable(); 
            if (auto* physComp = this->GetComponent<sg::ECS::Components::PhysicsComponent>()) {

                if (auto* tf = drawableComp->GetTransformable()) {
                    b2BodyId bid = physComp->GetBody().GetBodyId();
                    sg::Core::vec2f pos = sg::Core::vec2f(b2Body_GetPosition(bid));
                    tf->setPosition(pos.toSFML());
                    tf->setRotation(sf::radians(b2Rot_GetAngle(b2Body_GetRotation(bid))));
                }
                
            }


        }
    }

    sg::Core::vec2f sg::ECS::Entity::GetPosition()
    {
        sg::Core::vec2f result = sg::Core::vec2f(0,0);
        auto* drawableComp = this->GetComponent<sg::ECS::Components::DrawableComponent>();
        if (drawableComp != nullptr) {
            
            sf::Drawable* d = drawableComp->GetDrawable(); 
            if (auto* physComp = this->GetComponent<sg::ECS::Components::PhysicsComponent>()) {

                if (auto* tf = drawableComp->GetTransformable()) {
                    b2BodyId bid = physComp->GetBody().GetBodyId();
                    result = tf->getPosition();
                }
                
            }


        }
        return result;
    }

    void Entity::Update()
    {
        for (auto &[type, c] : components)
        {
            c->Update(*this);
        }
}