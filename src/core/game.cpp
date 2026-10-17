#include "core/game.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output
#include "core/entity.hpp"

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
        this->renderer.Render(&this->gameWindow);

 
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
    WindowSettings* ws = Settings::GetWindowSettings();
    spdlog::info("Printing settings..\nVersion: {}\nStarted on: {}\nAuthor: {}\n", pi->version, pi->started_on, pi->author);

    spdlog::info("Game finished initializing!");
    this->Load();

}

Entity ball = Entity();
Entity padl = Entity();
Entity padr = Entity();
sf::Transformable* balltf;
sf::Transformable* padltf;
sf::Transformable* padrtf;

sf::Transform get_transform_from_drawable(sf::Drawable* drawable) {
    sf::Transformable* tfable = dynamic_cast<sf::Transformable*>(drawable);
    sf::Transform tf = tfable->getTransform();
    return tf;
}

sf::Transformable* get_transformable_from_drawable(sf::Drawable* drawable) {
    return dynamic_cast<sf::Transformable*>(drawable);
}

// Creates gameWindow, calls Run()
void Game::Load()
{
    spdlog::info("Game loading..");

    // Create windows
    WindowSettings* ws = this->settings.GetWindowSettings();
    this->gameWindow = sf::RenderWindow(sf::VideoMode({ws->resolution.x, ws->resolution.y}), ws->title);
    this->gameWindow.setFramerateLimit(ws->max_framerate); // Limit frame rate

    
    // Load drawables
    auto circle = sf::CircleShape(5);
    circle.setFillColor(sf::Color(180, 180, 180));
    circle.setPosition(sf::Vector2f(200, 50));
    
    auto paddle_L = sf::RectangleShape(sf::Vector2f{28, 300});
    paddle_L.setFillColor(sf::Color(255,255,255));
    paddle_L.setPosition(sf::Vector2f(80, 50));
    
    auto paddle_R = sf::RectangleShape(sf::Vector2f{28, 300});
    paddle_R.setFillColor(sf::Color(255,255,255));
    paddle_R.setPosition(sf::Vector2f(1172, 50));
    


    ball.SetDrawable(circle);
    padl.SetDrawable(paddle_L);
    padr.SetDrawable(paddle_R);

    // Add to renderer
    this->renderer.AddDrawable(&circle);
    this->renderer.AddDrawable(&paddle_L);
    this->renderer.AddDrawable(&paddle_R);

    // Get transforms
    balltf = get_transformable_from_drawable(&ball.GetDrawable());
    padltf = get_transformable_from_drawable(&padl.GetDrawable());
    padrtf = get_transformable_from_drawable(&padr.GetDrawable());

    spdlog::info("Game finished loading!");
    this->Run();
}

// Called every frame before renderer is called
void Game::Update()
{
    // Poll for window and input events
    while (const std::optional event = this->gameWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            this->gameWindow.close(); // Close the window
        }
    }

    // listen for keyboard input
    if (padltf != nullptr && padrtf != nullptr){
        sf::Vector2f posl = padltf->getPosition();
        sf::Vector2f posr = padrtf->getPosition();
        
        if (posl.y >= 0 && posl.y <= 360) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
                if (posl.y+1 <= 360) {
                    padltf->setPosition(sf::Vector2f(posl.x, posl.y+1));
                    padrtf->setPosition(sf::Vector2f(posr.x, posr.y+1));
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
                if (posl.y-1 >= 0) {
                    padltf->setPosition(sf::Vector2f(posl.x, posl.y-1));
                    padrtf->setPosition(sf::Vector2f(posr.x, posr.y-1));
                }
            }

        }



    }


}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}