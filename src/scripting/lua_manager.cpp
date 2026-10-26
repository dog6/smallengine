#include "core/game.hpp"
#include <spdlog/spdlog.h>
#include "scripting/lua_manager.hpp"
#include "scripting/lua_toml.hpp"

using namespace sg::Scripting;
using namespace sg::Scripting::Bindings;
using namespace sg::Core;

void LuaManager::Load(LuaScript *script)
{
    spdlog::info("LuaManager::Load(LuaScript) Loading lua script '{}'", script->filepath);
    LoadFromPath(script->filepath.c_str());
}

void sg::Scripting::LuaManager::LoadFromTOML(std::string config_path)
{
    if (config_path.empty())  {
        spdlog::info("LuaTOML filepath passed is empty");
        return;
    }

    spdlog::info("Loading scripts from LuaTOML '{}'..", config_path);
    // Load lua scripts into s_scripts member using provided TOML path
    LuaTOML lt = LuaTOML(config_path);

    for (std::string path : lt.script_paths) {
        LoadFromPath(path.c_str());
        spdlog::info("LuaManager loaded script '{}' from LuaTOML '{}'", path, config_path);
    }

}


std::unique_ptr<LuaScript> LuaManager::LoadFromPath(const char* filepath)
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

    spdlog::info("Binding script '{}'..", filepath);
    BindToLua(script->lua);

    try {
        auto result = script->lua.script_file(filepath);

        if (!result.valid()) {
            sol::error err = result;
            spdlog::error(
                "Lua script error in '{}': {}",
                filepath,
                err.what()
            );
            return nullptr;
        }
    }
    catch (const sol::error& e) {
        spdlog::error(
            "Exception loading Lua script '{}': {}",
            filepath,
            e.what()
        );
        return nullptr;
    }

    // Transfer ownership to the manager only after successful loading.
    s_scripts.push_back(std::move(script));

    // The manager now owns the script.
    return nullptr;
}


void LuaManager::Update(float deltaTime)
{
    if (s_scripts.empty()) {
        spdlog::warn("LuaManager::Update: No scripts loaded");
        return;
    }

    for (auto& script : s_scripts) {
        auto* s = script.get();

        sol::object obj = s->lua["update"];

        if (!obj.valid() || obj.get_type() == sol::type::nil) {
            sol::object upper = s->lua["Update"];

            // spdlog::warn(
            //     "No global 'update' in '{}'. Uppercase 'Update' exists: {}",
            //     s->filepath,
            //     upper.valid() && upper.get_type() == sol::type::function
            // );
            continue;
        }

        if (obj.get_type() != sol::type::function) {
            spdlog::error(
                "'update' exists but is not a function in '{}'",
                s->filepath
            );
            continue;
        }

        sol::protected_function updateFunc =
            obj.as<sol::protected_function>();

        auto result = updateFunc(deltaTime);

        if (!result.valid()) {
            sol::error err = result;
            spdlog::error(
                "Lua Update Error in '{}': {}",
                s->filepath,
                err.what()
            );
        }
    }
}

void LuaManager::BindToLua(sol::state &lua)
{
    // Add script to lua manager
    // Bind script to different engine modules
    ecs_lua_bindings.Register(lua);         // entity-component system - entity factory, entity manager, etc
    graphics_lua_bindings.Register(lua);    // graphics system - color, shape factory, etc
    core_lua_bindings.Register(lua);        // core systems - math, types, etc 
    input_lua_bindings.Register(lua);       // user input - keyboard, mouse, etc
}

void LuaManager::Execute(std::string luaCode)
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
