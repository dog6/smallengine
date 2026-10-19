#include "graphics/renderer.hpp"
#include "core/components/drawable_component.hpp"
#include "core/components/physics_component.hpp"
#include "core/box2sf.hpp"
#include <spdlog/spdlog.h>

using namespace sg::Graphics;
using namespace sg::Core;
// Renderer will need to implement some kind of sf::View


void Renderer::Render(sf::RenderWindow* window,
                      const std::vector<std::unique_ptr<sg::Core::Entity>>& ents)
{
    if (window == nullptr) {
        spdlog::error("Cannot draw to null window");
        return;
    }

    window->clear(sf::Color::Black);

    for (const auto& e : ents) {
        if (!e) continue;

        auto* drawableComp = e->GetComponent<sg::Core::DrawableComponent>();
        if (drawableComp == nullptr) continue;

        sf::Drawable* d = drawableComp->GetDrawable();   // pointer version
        if (d == nullptr) continue;

        if (auto* physComp = e->GetComponent<sg::Core::PhysicsComponent>()) {
            if (auto* tf = drawableComp->GetTransformable()) {
                b2BodyId bid = physComp->GetBody().GetBodyId();
                tf->setPosition(toSFML(b2Body_GetPosition(bid)));
                tf->setRotation(sf::radians(b2Rot_GetAngle(b2Body_GetRotation(bid))));
            }
        }

        window->draw(*d);
    }

    window->display();
}