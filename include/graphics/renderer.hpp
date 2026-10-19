#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ecs/entity.hpp"

// Renders entities to the sf::RenderWindow (Game::gameWindow)

using namespace std;

// SFML renderer

namespace sg::Graphics {

class Renderer {

        void RenderEntitites(sf::RenderWindow* window, const std::vector<std::unique_ptr<sg::ECS::Entity>>& ents);

    public:

        void Render(sf::RenderWindow* window, const std::vector<std::unique_ptr<sg::ECS::Entity>>& ents);

};

}