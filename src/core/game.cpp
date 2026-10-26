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

#include "input/mouse.hpp"
#include "input/keyboard.hpp"


#include <imgui.h>
#include <imgui-sfml.h>
#include <chrono>

using namespace sg;
using namespace sg::Core;
using namespace sg::Physics;
using namespace sg::Input;
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

        this->Update();

        game_window->Render(EntityManager::entman);
        
        // Draw editor windows after the game scene.
        editorUIManager.Draw();
        ImGui::SFML::Render(game_window->window);
        
        game_window->Display();
        // window->display();

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

    if (!ImGui::SFML::Init(game_window->window))
    {
        spdlog::error("Failed to initialize ImGui-SFML");
        return;
    }


    // Load multiple lua scripts specified inside a .TOML file
    LuaManager::LoadFromTOML("D:/Projects/CPP/smallgame/res/scripts/configs/demo1.toml");
    
    game_window->renderer.clearBufferColor = sf::Color(32,32,32);

    spdlog::info("Game finished loading!");


    this->Run();

}

// Called every frame before renderer is called
void Game::Update()
{
    using Key = sf::Keyboard::Key;
    using Button = sf::Mouse::Button;

    // Measure real elapsed frame time.
    const sf::Time frameDelta = m_frameClock.restart();
    const float deltaSeconds = frameDelta.asSeconds();

    if (deltaSeconds > 0.0f)
    {
        const float instantFPS = 1.0f / deltaSeconds;

        // Smooth the FPS reading so it doesn't jump around as much.
        m_fps = (m_fps == 0.0f)
            ? instantFPS
            : m_fps * 0.9f + instantFPS * 0.1f;
    }

    Keyboard::BeginFrame();
    Mouse::BeginFrame();

    while (const std::optional event =
        game_window->window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(game_window->window, *event);

        if (event->is<sf::Event::Closed>())
        {
            game_window->window.close();
        }
        else if (const auto* e =
            event->getIf<sf::Event::KeyPressed>())
        {
            Keyboard::OnPressed(e->code);
        }
        else if (const auto* e =
            event->getIf<sf::Event::KeyReleased>())
        {
            Keyboard::OnReleased(e->code);
        }
        else if (const auto* e =
            event->getIf<sf::Event::MouseButtonPressed>())
        {
            Mouse::OnPressed(e->button);
        }
        else if (const auto* e =
            event->getIf<sf::Event::MouseButtonReleased>())
        {
            Mouse::OnReleased(e->button);
        }
        else if (event->is<sf::Event::FocusLost>())
        {
            Keyboard::ResetEventState();
            Mouse::ResetEventState();
        }
    }

    EntityManager::Update();
    LuaManager::Update(DELTA_TIME);

    // Measure only the physics update.
    const auto physicsStart = std::chrono::steady_clock::now();

    PhysicsManager::Update(DELTA_TIME);

    const auto physicsEnd = std::chrono::steady_clock::now();

    m_physicsStepMs =
        std::chrono::duration<float, std::milli>(
            physicsEnd - physicsStart
        ).count();

    // Send live values to the performance window.
    sg::Editor::PerformanceStats stats{};
    stats.fps = m_fps;
    stats.entityCount = EntityManager::entman.size();
    stats.physicsStepMs = m_physicsStepMs;

    editorUIManager.SetPerformanceStats(stats);

    // ImGui receives real frame time, not the fixed simulation timestep.
    ImGui::SFML::Update(game_window->window, frameDelta);
}

// Called on first game loop
void Game::Start()
{
    spdlog::info("Started game loop!");
}
