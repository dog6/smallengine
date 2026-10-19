#include "graphics/renderer.hpp"
#include "core/components/drawable_component.hpp"
#include "core/components/physics_component.hpp"
#include "core/b2sf.hpp"
#include <spdlog/spdlog.h>

using namespace sg::Graphics;
using namespace sg::Core;
// Renderer will need to implement some kind of sf::View

void sg::Graphics::Renderer::RenderEntitites(sf::RenderWindow *window, const std::vector<std::unique_ptr<sg::Core::Entity>> &ents)
{

    for (const auto& e : ents) {
        if (!e) continue;

        // Get sg::Core::DrawableComponent from Entity
        auto* drawableComp = e->GetComponent<sg::Core::DrawableComponent>();
        if (drawableComp == nullptr) continue;

        // Get sf::Drawable from DrawableComponent
        sf::Drawable* d = drawableComp->GetDrawable(); 
        if (d == nullptr) continue;

        // Sync PhysicsComponent (Box2D) position/rotation with DrawableComponent (SFML) position/rotation
        if (auto* physComp = e->GetComponent<sg::Core::PhysicsComponent>()) {
            if (auto* tf = drawableComp->GetTransformable()) {
                b2BodyId bid = physComp->GetBody().GetBodyId();
                tf->setPosition(metersToPixels(b2Body_GetPosition(bid)));
                tf->setRotation(sf::radians(b2Rot_GetAngle(b2Body_GetRotation(bid))));
            }
        }

        // Draw to window
        window->draw(*d);
    }

}

void Renderer::Render(sf::RenderWindow *window,
                      const std::vector<std::unique_ptr<sg::Core::Entity>> &ents)
{
    if (window == nullptr) {
        spdlog::error("Cannot draw to null window");
        return;
    }

    // Clear last frame
    window->clear(sf::Color::Black);

    // Render Entities
    if (ents.size() > 0) RenderEntitites(window, ents);

    // Display window
    window->display();

}