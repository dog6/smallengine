#pragma once

#include <memory>
#include "ecs/component.hpp"
#include <SFML/Graphics.hpp>

namespace sg::ECS::Components {

    class DrawableComponent : public Component {
    public:
        explicit DrawableComponent(std::unique_ptr<sf::Drawable> d);

        sf::Drawable* GetDrawable() { return drawable.get(); }
        sf::Transformable* GetTransformable() { return transformable; }

        void Update(Entity& owner) override;

    private:
        std::unique_ptr<sf::Drawable> drawable;
        sf::Transformable* transformable = nullptr;  // non-owning, can be null
    };

}