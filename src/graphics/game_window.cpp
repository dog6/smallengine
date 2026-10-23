#include "graphics/game_window.hpp"
#include "ecs/entity_manager.hpp"

using namespace sg::Core;

GameWindow::GameWindow(const WindowSettings* ws)
{
    this->window = sf::RenderWindow(sf::VideoMode({ws->resolution.x, ws->resolution.y}), ws->title);
    this->window.setFramerateLimit(ws->max_framerate); // Limit frame rate
    this->resolution = ws->resolution;
}

void GameWindow::Render(const std::vector<std::unique_ptr<sg::ECS::Entity>> &ents)
{
    renderer.Render(&window, ents);
}
