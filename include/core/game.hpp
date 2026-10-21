#pragma once

#include <SFML/Graphics.hpp>
#include "core/settings.hpp"
#include "physics/box2dworld.hpp"
#include "graphics/renderer.hpp"


namespace sg {
    class Game {

        private:
            inline static Core::Settings settings;
            sf::RenderWindow gameWindow;
            sg::Graphics::Renderer renderer;
            sg::Physics::Box2DWorld gameWorld = sg::Physics::Box2DWorld(b2Vec2{0, 9.81f});
            inline static Game* Instance = nullptr;

            // Called every frame
            void Run(); // game loop

            // Called on first frame
            void Start();

            // Called after game initializes
            void Load();

            // Called each frame before render takes place
            void Update();

        public:

            // Default constructors
            Game();
            ~Game() {}

            static Game* GetInstance() { return Game::Instance; }

            // Called first
            void Init();
    };

}