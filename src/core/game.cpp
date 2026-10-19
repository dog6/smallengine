#include "core/game.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output
#include "core/entity.hpp"
#include "core/components/drawable_component.hpp"
#include "core/components/physics_component.hpp"
#include <box2d/box2d.h>
#include "core/box2dworld.hpp"
#include "core/box2sf.hpp"

using namespace sg;
using namespace sg::Core;

// helper function to setup spdlog sinks
void setup_spdlog(){

    // 1. Create the console sink (with color support)
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);

    // 2. Create your file sink
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/game.log", true);
    file_sink->set_level(spdlog::level::info);

    // 3. Combine both sinks into a single vector
    std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };

    // 4. Create a multi-sink logger and set it as the global logger
    auto multi_logger = std::make_shared<spdlog::logger>("global_logger", sinks.begin(), sinks.end());
    
    // Register it globally and set the default pointer
    spdlog::register_logger(multi_logger);
    spdlog::set_default_logger(multi_logger);

}

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
    setup_spdlog();

    // Read settings
    Settings::ReadSettings();
    ProgInfo* pi = Settings::GetProgInfo();
    spdlog::info("Printing settings..\nVersion: {}\nStarted on: {}\nAuthor: {}\n", pi->version, pi->started_on, pi->author);

    entman = List<Entity>();
    spdlog::info("Game finished initializing!");
    

    this->Load();


}

std::unique_ptr<Entity> CreateRectEntity(b2WorldId worldId, sf::Vector2f pos, sf::Vector2f size, sf::Color col, bool isStatic) {
    
    // Create and add DrawableComponent
    auto e = std::make_unique<Entity>("Box");
    auto rect = std::make_unique<sf::RectangleShape>(size);
    rect->setFillColor(col);
    rect->setOrigin(size * 0.5f);
    rect->setPosition(pos);
    e->AddComponent<DrawableComponent>(std::move(rect));

    // Create and add PhysicsComponent
    b2Polygon box_polygon = b2MakeBox(size.x/2, size.y/2);
    e->AddComponent<PhysicsComponent>(isStatic, toBox2D(pos), worldId, box_polygon);
    
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
    sf::Vector2f e_size = sf::Vector2f(50, 50);
    sf::Vector2f e_pos = sf::Vector2f(500, 150);
    entman.Add(CreateRectEntity(this->gameWorld.worldId, e_pos, e_size, sf::Color{255,80,140}, false));

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