#include "graphics/renderer.hpp"
#include <algorithm>

using namespace sg::Graphics;

// Renderer will need to implement some kind of sf::View

int Renderer::AddDrawable(sf::Drawable *drawable) { 
    if (drawable == nullptr) return -1;

    this->drawables.push_back(drawable); 
    return 0;
}

int Renderer::RemoveDrawable(sf::Drawable *drawable) {
    auto it = std::find(this->drawables.begin(), this->drawables.end(), drawable);
    
    if (it != this->drawables.end()) {
        this->drawables.erase(it);
        return 0;
    }
    return -1;
}


void Renderer::Render(sf::RenderWindow *window)
{
    if (window == nullptr) return; // cannot draw to null window

    window->clear(sf::Color::Black);

    // draw each drawable
    for (int i = 0; i < this->drawables.size(); i++){
    
        if (drawables[i] == nullptr) continue; // skip null drawables
        window->draw(*drawables[i]); // draw drawable

    }

    window->display();
}