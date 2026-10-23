#include "scripting/bindings/core_lua_bindings.hpp"
#include "core/smath.hpp"
#include "core/game.hpp"

using namespace sg::Scripting::Bindings;
using sg::Core::vec2;
using sg::Core::uvec2;
using sg::Core::vec2f;

void CoreLuaBindings::RegisterVec2(sol::state& lua)
{
    lua.new_usertype<vec2>("vec2",
        sol::constructors<vec2(), vec2(int, int)>(),
        "x", &vec2::x,
        "y", &vec2::y,
        sol::meta_function::to_string, [](const vec2& v) {
            return "vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        }
    );
}

void CoreLuaBindings::RegisterUVec2(sol::state& lua)
{
    lua.new_usertype<uvec2>("uvec2",
        sol::constructors<uvec2(), uvec2(unsigned int, unsigned int)>(),
        "x", &uvec2::x,
        "y", &uvec2::y,
        sol::meta_function::to_string, [](const uvec2& v) {
            return "uvec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        }
    );
}

void CoreLuaBindings::RegisterVec2f(sol::state& lua)
{
    lua.new_usertype<vec2f>("vec2f",
        sol::call_constructor, sol::constructors<vec2f(), vec2f(float, float)>(),
        "new",                 sol::constructors<vec2f(), vec2f(float, float)>(),
        "x", &vec2f::x,
        "y", &vec2f::y,

        // Unit conversion (static: use a dot, not a colon)
        "pixelsToMeters", [](const vec2f& v) { return vec2f{ v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER }; },
        "metersToPixels", [](const vec2f& v) { return vec2f{ v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER }; },

        // Operators
        sol::meta_function::addition,       [](const vec2f& a, const vec2f& b) { return vec2f{ a.x + b.x, a.y + b.y }; },
        sol::meta_function::subtraction,    [](const vec2f& a, const vec2f& b) { return vec2f{ a.x - b.x, a.y - b.y }; },
        sol::meta_function::multiplication, [](const vec2f& a, float s)        { return vec2f{ a.x * s, a.y * s }; },
        sol::meta_function::equal_to,       [](const vec2f& a, const vec2f& b) { return a.x == b.x && a.y == b.y; },
        sol::meta_function::to_string,      [](const vec2f& v) {
            return "vec2f(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        }
    );
}

void CoreLuaBindings::RegisterMathFunctions(sol::state& lua)
{
    sol::table smath = lua.create_named_table("smath");

    smath.set_function("random_int",   &sg::Core::random_int);
    smath.set_function("random_float", &sg::Core::random_float);

    smath["PIXELS_PER_METER"] = PIXELS_PER_METER;
}

void CoreLuaBindings::RegisterGameWindow(sol::state &lua)
{
     lua.new_usertype<GameWindow>("GameWindow",
        // Read-only access to the resolution.
        "resolution", sol::readonly(&GameWindow::resolution),

        // Window functions
        "is_open", [](GameWindow& gw) {
            return gw.window.isOpen();
        },
        "close", [](GameWindow& gw) {
            gw.window.close();
        },
        "set_title", [](GameWindow& gw, const std::string& title) {
            gw.window.setTitle(title);
        },
        "set_vsync", [](GameWindow& gw, bool enabled) {
            gw.window.setVerticalSyncEnabled(enabled);
        },
        "set_framerate_limit", [](GameWindow& gw, unsigned int limit) {
            gw.window.setFramerateLimit(limit);
        }

     );
}

void CoreLuaBindings::Register(sol::state& lua)
{
    RegisterVec2(lua);
    RegisterUVec2(lua);
    RegisterVec2f(lua);
    RegisterMathFunctions(lua);
    RegisterGameWindow(lua);
    lua.new_usertype<b2WorldId>("b2WorldId", sol::no_constructor);
    lua["world_id"] = sg::Game::GetPhysics().world.worldId;
    lua["game_window"] = &sg::Game::GetInstance()->GetWindow();   // see the note below
    // if (auto* game = sg::Game::GetInstance()) {
    //     lua["game_world_id"] = game->GetB2WorldId();
    // }

}