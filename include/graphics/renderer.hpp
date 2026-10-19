#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "core/entity.hpp"

// Renders entities to the sf::RenderWindow (Game::gameWindow)

using namespace std;

// SFML renderer

namespace sg::Graphics {

class Renderer {
    public:

        void Render(sf::RenderWindow* window, const std::vector<std::unique_ptr<sg::Core::Entity>>& ents);

};

}