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
#include "graphics/shape_factory.hpp"
#include <variant>

#include "core/smath.hpp"

using namespace sg;
using namespace sg::Core;
using namespace sg::Physics;
using namespace sg::ECS;
using namespace sg::ECS::Components;
using namespace sg::Graphics;

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


// Create a circle entity. Radius and position are in pixels.
std::unique_ptr<Entity> CreateCircleEntityPixels(std::string name, b2WorldId worldId, sf::Vector2f pixel_pos,
                                float pixel_radius, sf::Color col, bool isStatic)
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // Create and add PhysicsComponent
    b2Circle circle{};
    circle.center = {0.0f, 0.0f};
    circle.radius = pixel_radius;
    ent->AddComponent<PhysicsComponent>(isStatic, pixelsToMeters(pixel_pos), worldId, circle);

    // Create and add DrawableComponent
    auto shape = ShapeFactory::MakeCircle(pixel_radius, col);
    ent->AddComponent<DrawableComponent>(std::move(shape));

    return ent;
}

// Same as above, but position and radius are in meters.
std::unique_ptr<Entity> CreateCircleEntityMeters(std::string name, b2WorldId worldId, b2Vec2 meter_pos,
                                float meter_radius, sf::Color col, bool isStatic)
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // Create and add PhysicsComponent
    b2Circle circle{};
    circle.center = {0.0f, 0.0f};
    circle.radius = meter_radius/PIXELS_PER_METER;
    ent->AddComponent<PhysicsComponent>(isStatic, meter_pos, worldId, circle);

    // Create and add DrawableComponent
    auto shape = ShapeFactory::MakeCircle(meter_radius, col);
    ent->AddComponent<DrawableComponent>(std::move(shape));

    return ent;
}


// We need somewhere to store CreateRectEntity(), and ideally change it to be more generic
std::unique_ptr<Entity> CreateRectEntityPixels(std::string name, b2WorldId worldId, sf::Vector2f pixel_pos,
                                sf::Vector2f pixel_size, sf::Color col, bool isStatic)
                                {
    
    // Create new unique_ptr<Entity>
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // Create and add PhysicsComponent
    b2Vec2 b2size = pixelsToMeters(pixel_size);
    b2Polygon box_polygon = b2MakeBox(b2size.x*0.5f, b2size.y*0.5f);
    ent->AddComponent<PhysicsComponent>(isStatic, pixelsToMeters(pixel_pos), worldId, box_polygon);
    
    // Create and add DrawableComponent
    auto rect = ShapeFactory::MakeRectangle(pixel_size, col);
    ent->AddComponent<DrawableComponent>(std::move(rect));

    return ent;
}

// Same as above, but takes in meter pos/size instead of pixel size
std::unique_ptr<Entity> CreateRectEntityMeters(std::string name, b2WorldId worldId, b2Vec2 meter_pos,
                                b2Vec2 meter_size, sf::Color col, bool isStatic)
                                {
    
    // Create new unique_ptr<Entity>
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // TODO NEXT --> Implement a way to create b2Polygons inside lua and assign them to drawables
    b2Polygon poly = b2MakeBox(meter_size.x*0.5f, meter_size.y*0.5f);
    ent->AddComponent<PhysicsComponent>(isStatic, meter_pos, worldId, poly);
    
    // Create and add DrawableComponent
    sf::Vector2f sf_size = metersToPixels(meter_size);
    auto rect = ShapeFactory::MakeRectangle(sf_size, col);
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
    EntityManager::entman.Add(CreateRectEntityMeters("Person", this->gameWorld.worldId, person_ref_pos, person_ref_size, sf::Color{255,80,140}, false));
    

    
    // Load dynamic box
    for (int i = 0; i < 300; i++){
        uint8_t r = static_cast<uint8_t>(random_int(60, 255));
        uint8_t g = static_cast<uint8_t>(random_int(60, 255));
        uint8_t b = static_cast<uint8_t>(random_int(60, 255));

        // Load meter sized boxes
        b2Vec2 b2_size = b2Vec2{1, 1};
        b2Vec2 b2_pos = b2Vec2{10.0f, 50.0f-(i*2)};
        EntityManager::entman.Add(CreateRectEntityMeters("Dynamic Box", this->gameWorld.worldId, b2_pos, b2_size, sf::Color{r,g,b}, false));

        // Load pixel sized boxes
        sf::Vector2f ent_size = sf::Vector2f(10, 10);
        sf::Vector2f ent_pos = sf::Vector2f(250.0f, 50.0f-(i*12));

        EntityManager::entman.Add(CreateRectEntityPixels("Dynamic Box", this->gameWorld.worldId, ent_pos, ent_size, sf::Color{r,g,b}, false));
          
        b2Vec2 circle_pos = b2Vec2{15, 50.0f-(i*3)};
        float circle_radius = 15; // 2m in radius
        EntityManager::entman.Add(
            CreateCircleEntityMeters("Dynamic Circle", this->gameWorld.worldId, circle_pos, circle_radius, sf::Color(r,g,b), false)
        );
    }


    // Load ground
    sf::Vector2f ground_size = sf::Vector2f(static_cast<float>(ws->resolution.x), 32.0f);
    sf::Vector2f ground_pos = sf::Vector2f(ground_size.x * 0.5f, static_cast<float>(ws->resolution.y)-ground_size.y * 0.5f);
    EntityManager::entman.Add(CreateRectEntityPixels("Ground", this->gameWorld.worldId, ground_pos, ground_size, sf::Color{255,255,255}, true));

    // Wall thickness in pixels
    const float wall_thickness = 32.0f;
    float res_x = static_cast<float>(ws->resolution.x);
    float res_y = static_cast<float>(ws->resolution.y);

    // Both walls have the full height of the window
    sf::Vector2f wall_size = sf::Vector2f(wall_thickness, res_y);

    // Left wall: the center is half a thickness from the left edge
    sf::Vector2f left_pos = sf::Vector2f(wall_thickness * 0.5f, res_y * 0.5f);
    EntityManager::entman.Add(CreateRectEntityPixels("LeftWall", this->gameWorld.worldId, left_pos, wall_size, sf::Color{255,255,255}, true));

    // Right wall: the center is half a thickness from the right edge
    sf::Vector2f right_pos = sf::Vector2f(res_x - wall_thickness * 0.5f, res_y * 0.5f);
    EntityManager::entman.Add(CreateRectEntityPixels("RightWall", this->gameWorld.worldId, right_pos, wall_size, sf::Color{255,255,255}, true));


    // TODO: Create Circle
    
    renderer.clearBufferColor = sf::Color(32,32,32);

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