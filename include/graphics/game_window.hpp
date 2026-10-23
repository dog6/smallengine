#pragma once

#include <SFML/Graphics.hpp>
#include "graphics/renderer.hpp"
#include "core/setting_types.hpp"

struct GameWindow {
    GameWindow(const sg::Core::WindowSettings* ws);
    ~GameWindow() {}

    sf::RenderWindow window;
    sg::Graphics::Renderer renderer;

    sg::Core::uvec2 resolution;
    void Render(const std::vector<std::unique_ptr<sg::ECS::Entity>>& ents);
};