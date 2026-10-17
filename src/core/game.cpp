#include "core/game.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output

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

        // Poll for window and input events
        while (const std::optional event = this->gameWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->gameWindow.close(); // Close the window
            }
        }

        // Clear the screen with a black color
        this->gameWindow.clear(sf::Color::Black);

        // Draw the shape to the back buffer
        this->renderer.Render(&this->gameWindow);

        // Display the back buffer contents on screen
        this->gameWindow.display();
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

// Creates gameWindow, calls Run()
void Game::Load()
{
    spdlog::info("Game loading..");
    // Create the main rendering window (800x600 resolution)
    WindowSettings* ws = this->settings.GetWindowSettings();
    this->gameWindow = sf::RenderWindow(sf::VideoMode({ws->resolution.x, ws->resolution.y}), ws->title);
    this->gameWindow.setFramerateLimit(ws->max_framerate); // Limit frame rate

    spdlog::info("Game finished loading!");

    // load shape
    auto circle = sf::CircleShape(5);
    circle.setFillColor(sf::Color(0, 160, 240));
    circle.setPosition(sf::Vector2f(50, 50));
    this->renderer.AddDrawable(&circle);

    this->Run();
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}
