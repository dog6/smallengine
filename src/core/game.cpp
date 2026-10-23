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
#include "core/smath.hpp"

using namespace sg;
using namespace sg::Core;
using namespace sg::Physics;
using namespace sg::ECS;
using namespace sg::ECS::Components;
using namespace sg::Graphics;
using namespace sg::Content;
using namespace sg::Scripting;

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
        game_window->Render(EntityManager::entman.GetAll());
 
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
    EntityManager::entman = List<Entity>();

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

    // TEMP CODE

    // Create person sized box
    // b2Vec2 person_ref_size = b2Vec2{0.45, 1.75};
    // b2Vec2 person_ref_pos = b2Vec2{20.0f, 20.0f};
    // EntityManager::entman.Add(EntityFactory::CreateRectEntity("Person", PhysicsManager::world.worldId, person_ref_pos, person_ref_size,
    //      sf::Color{255,80,140}, DYNAMIC_BODY, USE_METER_SCALE));
    

    LuaManager::LoadFromPath("D:/Projects/CPP/smallgame/res/scripts/demo_1.lua");
    LuaManager::LoadFromPath("D:/Projects/CPP/smallgame/res/scripts/wall_border.lua");

    
    
    // END OF TEMP CODE
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

    EntityManager::entman.Update();

    PhysicsManager::Update(1.0f/60.0f);
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}
