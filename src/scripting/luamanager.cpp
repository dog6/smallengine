#include "core/game.hpp"
#include "scripting/lua_manager.hpp"
#include <spdlog/spdlog.h>

using namespace sg::Scripting;
using namespace sg::Core;

void LuaManager::Load(LuaScript *script)
{
    spdlog::info("LuaManager::Load(LuaScript) Loading lua script '{}'", script->filepath);
    LoadFromPath(script->filepath.c_str());
}

std::unique_ptr<LuaScript> LuaManager::LoadFromPath(const char *filepath)
{
    spdlog::info("Loading lua script '{}'", filepath);
    auto script = std::make_unique<LuaScript>();
    script->filepath = filepath;

    script->lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );

    spdlog::info("Binding script '{}'..", script->filepath.c_str());
    BindToLua(script->lua);

    // Pass references idk lol
    // script->lua["game"] = Game::GetInstance();
    // script->lua["CreateRectEntity"]

    try {
        auto result = script->lua.script_file(filepath);
        if (!result.valid()) {
            sol::error err = result;
            spdlog::error("Lua script error in '{}\nERROR: {}",filepath, err.what());
            return nullptr;
        }
    }
    catch (const sol::error& e) {
        spdlog::error("Exception loading Lua script '{}': {}", filepath, e.what());
        return nullptr;
    }

    return script; // The compiler moves it. Do not write std::move here.
}

void sg::Scripting::LuaManager::Update(float deltaTime, LuaScript &script)
{
    sol::protected_function updateFunc = script.lua["OnUpdate"];
        if (updateFunc.valid()) {
            auto result = updateFunc(deltaTime);
            if (!result.valid()) {
                sol::error err = result;
                spdlog::error("Lua Update Error in '{}'\nERROR: {}", script.filepath, err.what());
            }
        }

}

void sg::Scripting::LuaManager::BindToLua(sol::state &lua)
{

    // CoreRegistrar::Register(lua);
    // GraphicsRegistrar::Register(lua);
    // PhysicsRegistrar::Register(lua);

}

void sg::Scripting::LuaManager::Execute(std::string luaCode)
{
    LuaScript cmdScript = LuaScript();
    sol::state& lua = cmdScript.lua;

    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );

    spdlog::info("Binding {} to lua", luaCode.c_str());
    BindToLua(lua);

    try {
        // Run lua code
        sol::load_result script = lua.load(luaCode);

        if (!script.valid()) {
            sol::error err = script;
            spdlog::error("Compile Error: {}", err.what());
        }
        else {
            sol::protected_function func = script;
            sol::protected_function_result result = func();

            if (!result.valid()) {
                sol::error err = result;
                spdlog::warn("Runtime Error: {}", err.what());
            }

        }
    }
    catch (const std::exception& e) {
        spdlog::error("Lua Exception: {}", e.what());
    }
}
