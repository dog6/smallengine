#include "scripting/bindings/graphics_lua_bindings.hpp"

#include "graphics/shape_factory.hpp"
#include "graphics/scolor.hpp"

using namespace sg::Scripting::Bindings;
using namespace sg::Graphics;


static std::uint8_t ClampByte(int v)
{
    return static_cast<std::uint8_t>(std::clamp(v, 0, 255));
}

void GraphicsLuaBindings::RegisterColor(sol::state& lua)
{
    lua.new_usertype<sg::Graphics::ColorRGB>("ColorRGB",
        sol::call_constructor,
        sol::factories([](int r, int g, int b) {
            return ColorRGB(ClampByte(r), ClampByte(g), ClampByte(b));
        }),
        "new", sol::factories([](int r, int g, int b) {
            return ColorRGB(ClampByte(r), ClampByte(g), ClampByte(b));
        }),
        "r", &ColorRGB::Red,
        "g", &ColorRGB::Green,
        "b", &ColorRGB::Blue
    );

    lua.new_usertype<sg::Graphics::ColorRGBA>("ColorRGBA",
        sol::call_constructor,
        sol::factories([](int r, int g, int b, int a) {
            return ColorRGBA(ClampByte(r), ClampByte(g), ClampByte(b), ClampByte(a));
        }),
        "new", sol::factories([](int r, int g, int b, int a) {
            return ColorRGBA(ClampByte(r), ClampByte(g), ClampByte(b), ClampByte(a));
        }),
        "r", &ColorRGBA::Red,
        "g", &ColorRGBA::Green,
        "b", &ColorRGBA::Blue,
        "a", &ColorRGBA::Alpha
    );
}


void GraphicsLuaBindings::Register(sol::state &lua)
{
    RegisterColor(lua);
}