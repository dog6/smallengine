#include "core/game.hpp"

#include <spdlog/spdlog.h>
// #include <spdlog/sinks/basic_file_sink.h>
// #include <spdlog/sinks/stdout_color_sinks.h> // For colored console output
#include "ecs/entity.hpp"
#include "ecs/components/drawable_component.hpp"
#include "ecs/components/physics_component.hpp"
#include <box2d/box2d.h>
#include "physics/box2dworld.hpp"
#include "core/b2sf.hpp"
#include "core/logger.hpp"

using namespace sg;
using namespace sg::Core;
using namespace sg::Physics;
using namespace sg::ECS;
using namespace sg::ECS::Components;

Game::Game()
{
    this->renderer = Graphics::Renderer();
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
        this->renderer.Render(&this->gameWindow, entman.GetAll());

 
        frameNumber++;
    }
}

// Read 'settings.toml', calls Load()
void Game::Init()
{
    spdlog::info("Initializing game..");
    // setup_spdlog();
    Logger::SetupDefaultSinks();

    // Read settings
    Settings::ReadSettings();
    ProgInfo* pi = Settings::GetProgInfo();
    spdlog::info("Printing settings..\nVersion: {}\nStarted on: {}\nAuthor: {}\n", pi->version, pi->started_on, pi->author);

    entman = List<Entity>();
    spdlog::info("Game finished initializing!");
    

    this->Load();
}

std::unique_ptr<Entity> CreateRectEntity(b2WorldId worldId, sf::Vector2f pixel_pos,
                                sf::Vector2f pixel_size, sf::Color col, bool isStatic)
                                {
    
    // Create new unique_ptr<Entity>
    auto e = std::make_unique<Entity>("Box");

    // Create and add PhysicsComponent
    b2Vec2 b2size = pixelsToMeters(pixel_size);
    b2Polygon box_polygon = b2MakeBox(b2size.x*0.5f, b2size.y*0.5f);
    e->AddComponent<PhysicsComponent>(isStatic, pixelsToMeters(pixel_pos), worldId, box_polygon);
    
    // Create and add DrawableComponent
    auto rect = std::make_unique<sf::RectangleShape>(pixel_size);
    rect->setFillColor(col);
    rect->setOrigin(pixel_size*0.5f);
    rect->setPosition(pixel_pos);
    e->AddComponent<DrawableComponent>(std::move(rect));

    return e;
}

// Creates gameWindow, calls Run()
void Game::Load()
{
    spdlog::info("Game loading..");

    // Create windows
    WindowSettings* ws = this->settings.GetWindowSettings();
    this->gameWindow = sf::RenderWindow(sf::VideoMode({ws->resolution.x, ws->resolution.y}), ws->title);
    this->gameWindow.setFramerateLimit(ws->max_framerate); // Limit frame rate

    // Load dynamic box
    for (int i = 0; i < 3; i++){
        sf::Vector2f e_size = sf::Vector2f(50, 50);
        sf::Vector2f e_pos = sf::Vector2f(500, 50*i);
        entman.Add(CreateRectEntity(this->gameWorld.worldId, e_pos, e_size, sf::Color{255,80,140}, false));
    }

    // Load ground
    sf::Vector2f ground_size = sf::Vector2f(static_cast<float>(ws->resolution.x), 32.0f);
    sf::Vector2f ground_pos = sf::Vector2f(0.0f, static_cast<float>(ws->resolution.y)-32);
    entman.Add(CreateRectEntity(this->gameWorld.worldId, ground_pos, ground_size, sf::Color{255,255,255}, true));

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

    entman.Update();

    auto b = entman.Get(1);
    auto pb = b->GetComponent<PhysicsComponent>();
    b2BodyId bid = pb->GetBody().GetBodyId();
    b2Vec2 position = b2Body_GetPosition(bid);
    b2Rot rotation = b2Body_GetRotation(bid);
    printf("\r%s - %4.2f %4.2f %4.2f", b->GetName().c_str(), position.x, position.y, b2Rot_GetAngle(rotation));

    b2World_Step(this->gameWorld.worldId, 1.0f/60.0f, 4);
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}