#include "core/game.hpp"

#include <spdlog/spdlog.h>
#include "ecs/entity.hpp"
#include "ecs/components/drawable_component.hpp"
#include "ecs/components/physics_component.hpp"
#include <box2d/box2d.h>
#include "physics/box2dworld.hpp"
#include "core/logger.hpp"
#include "ecs/entity_manager.hpp"
#include "graphics/shape_factory.hpp"
#include "content/entity_factory.hpp"
#include "scripting/lua_manager.hpp"
#include "scripting/lua_toml.hpp"
#include "core/smath.hpp"

using namespace sg;
using namespace sg::Core;
using namespace sg::Physics;
using namespace sg::ECS;
using namespace sg::ECS::Components;
using namespace sg::Graphics;
using namespace sg::Content;
using namespace sg::Scripting;

#define DELTA_TIME 1.0f/60.0f

Game::Game()
{
    Game::instance = this;
}

void Game::Run()
{
    spdlog::info("Starting game loop..");

    long long unsigned int frameNumber = 0;
    
    // Main application loop
    while (this->game_window->window.isOpen()) {

        if (frameNumber == 0){
            this->Start();
        }

        // Update game logic
        this->Update();

        // Draw all drawables added to renderer
        game_window->Render(EntityManager::entman);
        // game_window->Render(EntityManager::entman)
 
        frameNumber++;
    }
}

// Read 'settings.toml', calls Load()
void Game::Init()
{
    // Setup file & console logging w/ spdlog
    Logger::SetupDefaultSinks(); 

    spdlog::info("Initializing game..");

    // Read settings
    AppSettings::ReadSettings();
    ProgInfo* pi = AppSettings::GetProgInfo();
    spdlog::info("Printing settings..\nVersion: {}\nStarted on: {}\nAuthor: {}\n", pi->version, pi->started_on, pi->author);

    // Create list to store entitites
    // EntityManager::entman = List<Entity>();
    EntityManager::entman = std::vector<std::unique_ptr<sg::ECS::Entity>>();

    spdlog::info("Game finished initializing!");

    this->Load();
}


// Creates gameWindow, calls Run()
void Game::Load()
{
    spdlog::info("Game loading..");

    // Create window
    WindowSettings* ws = this->settings.GetWindowSettings();
    Game::game_window = new GameWindow(ws);

    // Load multiple lua scripts specified inside a .TOML file
    LuaManager::LoadFromTOML("D:/Projects/CPP/smallgame/res/scripts/configs/demo1.toml");
    
    game_window->renderer.clearBufferColor = sf::Color(32,32,32);

    spdlog::info("Game finished loading!");


    this->Run();

}

// Called every frame before renderer is called
void Game::Update()
{
    // Poll for window and input eventss
    while (const std::optional event = game_window->window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game_window->window.close(); // Close the window
        }
    }

    // EntityManager::entman.Update();
    EntityManager::Update();
    LuaManager::Update(DELTA_TIME);
    PhysicsManager::Update(DELTA_TIME);
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}
