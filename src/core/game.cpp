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
    Game::Instance = this;
}

void Game::Run()
{
    spdlog::info("Starting game loop..");

    long long unsigned int frameNumber = 0;
    
    // Main application loop
    while (this->gameWindow.isOpen()) {

        if (frameNumber == 0){
            this->Start();
        }

        // Update game logic
        this->Update();

        // Draw all drawables added to renderer
        this->renderer.Render(&this->gameWindow, EntityManager::entman.GetAll());

 
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
    Settings::ReadSettings();
    ProgInfo* pi = Settings::GetProgInfo();
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

    // Create windows
    WindowSettings* ws = this->settings.GetWindowSettings();

    this->gameWindow = sf::RenderWindow(sf::VideoMode({ws->resolution.x, ws->resolution.y}), ws->title);
    this->gameWindow.setFramerateLimit(ws->max_framerate); // Limit frame rate
    float res_x = static_cast<float>(ws->resolution.x);
    float res_y = static_cast<float>(ws->resolution.y);

    // TEMP CODE

    // Create person sized box
    b2Vec2 person_ref_size = b2Vec2{0.45, 1.75};
    b2Vec2 person_ref_pos = b2Vec2{20.0f, 20.0f};
    EntityManager::entman.Add(EntityFactory::CreateRectEntity("Person", this->gameWorld.worldId, person_ref_pos, person_ref_size,
         sf::Color{255,80,140}, DYNAMIC_BODY, USE_METER_SCALE));
    

    LuaManager::LoadFromPath("D:/Projects/CPP/smallgame/res/scripts/demo_1.lua");


    // Wall dimensions
    const float wall_thickness = 32.0f;
    sf::Vector2f wall_size = sf::Vector2f(wall_thickness, res_y); // walls as tall as window
    sf::Vector2f left_pos = sf::Vector2f(wall_thickness * 0.5f, res_y * 0.5f);
    sf::Vector2f right_pos = sf::Vector2f(res_x - wall_thickness * 0.5f, res_y * 0.5f);


    // // Load ground
    sf::Vector2f ground_size = sf::Vector2f(static_cast<float>(ws->resolution.x), 32.0f);
    sf::Vector2f ground_pos = sf::Vector2f(ground_size.x * 0.5f, static_cast<float>(ws->resolution.y)-ground_size.y * 0.5f);
    EntityManager::entman.Add(EntityFactory::CreateRectEntity("Ground", this->gameWorld.worldId, ground_pos, ground_size, sf::Color{255,255,255}, STATIC_BODY, USE_PIXEL_SCALE));

    // Left wall
    EntityManager::entman.Add(EntityFactory::CreateRectEntity("LeftWall", this->gameWorld.worldId, left_pos, wall_size, 
            sf::Color{255,255,255}, STATIC_BODY, USE_PIXEL_SCALE));

    // Right wall
    EntityManager::entman.Add(EntityFactory::CreateRectEntity("RightWall", this->gameWorld.worldId, right_pos, wall_size, 
            sf::Color{255,255,255}, STATIC_BODY, USE_PIXEL_SCALE));


    // TODO: Create Circle
    
    
    // END OF TEMP CODE
    
    renderer.clearBufferColor = sf::Color(32,32,32);

    spdlog::info("Game finished loading!");


    this->Run();

}

// Called every frame before renderer is called
void Game::Update()
{
    // Poll for window and input eventss
    while (const std::optional event = this->gameWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            this->gameWindow.close(); // Close the window
        }
    }

    EntityManager::entman.Update();

    // Debug physics logging
    // auto b = EntityManager::entman.Get(1);
    // auto pb = b->GetComponent<PhysicsComponent>();
    // b2BodyId bid = pb->GetBody().GetBodyId();
    // b2Vec2 position = b2Body_GetPosition(bid);
    // b2Rot rotation = b2Body_GetRotation(bid);
    // printf("\r%s - %4.2f %4.2f %4.2f", b->GetName().c_str(), position.x, position.y, b2Rot_GetAngle(rotation));
    b2World_Step(gameWorld.worldId, 1.0f/60.0f, 4);
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}
