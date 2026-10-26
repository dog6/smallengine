#pragma once

#include "core/app_settings.hpp"
#include "physics/physics_manager.hpp"
#include "graphics/game_window.hpp"
#include "editor/editor_manager.hpp"

namespace sg {
    class Game {

        private:
            inline static Game* instance = nullptr;
            inline static Core::AppSettings settings;
            inline static PhysicsManager physics;
            inline static GameWindow* game_window;
            static sg::Core::uvec2 screen_resolution;
            sg::Editor::EditorUIManager editorUIManager;
            sf::Clock m_frameClock;
            float m_fps = 0.0f;
            float m_physicsStepMs = 0.0f;

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


            const static PhysicsManager& GetPhysics() { return Game::physics; }
            const static GameWindow& GetWindow() { return *Game::game_window; }
            static Game* GetInstance() { return Game::instance; }
            // static b2WorldId GetB2WorldId() { return gameWorld.worldId; }
            // Called first
            void Init();
    };

}