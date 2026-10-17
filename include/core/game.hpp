#pragma once

#include <SFML/Graphics.hpp>
#include "core/settings.hpp"
#include "graphics/renderer.hpp"

namespace sg {
    class Game {

        private:
            inline static Core::Settings settings;
            sf::RenderWindow gameWindow;
            Graphics::Renderer renderer;


            // Called every frame
            void Run(); // game loop

            // Called on first frame
            void Start();

            // Called after game initializes
            void Load();

        public:

            // Default constructors
            Game();
            ~Game() {}

            // Called first
            void Init();
    };

}