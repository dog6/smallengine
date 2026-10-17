#pragma once

#include <vector>
#include "sfml/Graphics/Drawable.hpp"
#include "sfml/Graphics/RenderWindow.hpp"

// Renders entities to the sf::RenderWindow (Game::gameWindow)

using namespace std;

// using namespace sg::Core;

// SFML renderer

namespace sg::Graphics {

class Renderer {

    private:

        std::vector<sf::Drawable*> drawables;

    public:

        int AddDrawable(sf::Drawable* drawable);
        int RemoveDrawable(sf::Drawable* drawable);

        void Render(sf::RenderWindow* window);

};

}