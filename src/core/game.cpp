#include "core/game.hpp"

#include <spdlog/spdlog.h>
#include "ecs/entity.hpp"
#include "ecs/components/drawable_component.hpp"
#include "ecs/components/physics_component.hpp"
#include <box2d/box2d.h>
#include "physics/box2dworld.hpp"
#include "core/b2sf.hpp"
#include "core/logger.hpp"
#include "ecs/entity_manager.hpp"

#include <variant>

using namespace sg;
using namespace sg::Core;
using namespace sg::Physics;
using namespace sg::ECS;
using namespace sg::ECS::Components;

Game::Game()
{
    this->renderer = Graphics::Renderer();
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

// Avoid complexity
// Next draw circle
// We need somewhere to store CreateRectEntity(), and ideally change it to be more generic
std::unique_ptr<Entity> CreateRectEntityPixels(b2WorldId worldId, sf::Vector2f pixel_pos,
                                sf::Vector2f pixel_size, sf::Color col, bool isStatic)
                                {
    
    // Create new unique_ptr<Entity>
    std::unique_ptr<Entity> ent = std::make_unique<Entity>("Box");

    // Create and add PhysicsComponent
    b2Vec2 b2size = pixelsToMeters(pixel_size);
    b2Polygon box_polygon = b2MakeBox(b2size.x*0.5f, b2size.y*0.5f);
    ent->AddComponent<PhysicsComponent>(isStatic, pixelsToMeters(pixel_pos), worldId, box_polygon);
    
    // Create and add DrawableComponent
    std::unique_ptr<sf::RectangleShape> rect = std::make_unique<sf::RectangleShape>(pixel_size);
    rect->setFillColor(col);
    rect->setOrigin(pixel_size*0.5f);
    rect->setPosition(pixel_pos);
    ent->AddComponent<DrawableComponent>(std::move(rect));

    return ent;
}

// Same as above, but takes in meter pos/size instead of pixel size
std::unique_ptr<Entity> CreateRectEntityMeters(b2WorldId worldId, b2Vec2 meter_pos,
                                b2Vec2 meter_size, sf::Color col, bool isStatic)
                                {
    
    // Create new unique_ptr<Entity>
    std::unique_ptr<Entity> ent = std::make_unique<Entity>("Box");

    // Create and add PhysicsComponent
    // b2Vec2 b2size = metersToPixels(meter_pos);
    b2Polygon box_polygon = b2MakeBox(meter_size.x*0.5f, meter_size.y*0.5f);
    ent->AddComponent<PhysicsComponent>(isStatic, meter_pos, worldId, box_polygon);
    
    // Create and add DrawableComponent
    sf::Vector2f sf_size = metersToPixels(meter_size);
    std::unique_ptr<sf::RectangleShape> rect = std::make_unique<sf::RectangleShape>(sf_size);
    rect->setFillColor(col);
    rect->setOrigin(sf_size*0.5f);
    rect->setPosition(sf_size);
    ent->AddComponent<DrawableComponent>(std::move(rect));

    return ent;
}


// Creates gameWindow, calls Run()
void Game::Load()
{
    spdlog::info("Game loading..");

    // Create windows
    WindowSettings* ws = this->settings.GetWindowSettings();
    this->gameWindow = sf::RenderWindow(sf::VideoMode({ws->resolution.x, ws->resolution.y}), ws->title);
    this->gameWindow.setFramerateLimit(ws->max_framerate); // Limit frame rate

    // TEMP CODE

    // Create person sized box
    b2Vec2 person_ref_size = b2Vec2{0.45, 1.75};
    b2Vec2 person_ref_pos = b2Vec2{20.0f, 20.0f};
    EntityManager::entman.Add(CreateRectEntityMeters(this->gameWorld.worldId, person_ref_pos, person_ref_size, sf::Color{255,80,140}, false));
    
    
    // Load dynamic box
    for (int i = 0; i < 500; i++){
        b2Vec2 ent_size = b2Vec2{1, 1};
        b2Vec2 ent_pos = b2Vec2{10.0f, 50.0f-(i*2)};
        EntityManager::entman.Add(CreateRectEntityMeters(this->gameWorld.worldId, ent_pos, ent_size, sf::Color{130,255,180}, false));
    }

    // Load ground
    sf::Vector2f ground_size = sf::Vector2f(static_cast<float>(ws->resolution.x), 32.0f);
    sf::Vector2f ground_pos = sf::Vector2f(ground_size.x * 0.5f, static_cast<float>(ws->resolution.y)-ground_size.y * 0.5f);
    EntityManager::entman.Add(CreateRectEntityPixels(this->gameWorld.worldId, ground_pos, ground_size, sf::Color{255,255,255}, true));

    // TODO: Create Circle
    // END OF TEMP CODE


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

    b2World_Step(this->gameWorld.worldId, 1.0f/60.0f, 4);
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}