#include "scripting/bindings/input_lua_bindings.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "core/game.hpp"
#include "core/smath.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

namespace sg::Scripting::Bindings {

namespace {

template <typename Device, typename Code>
void BindDigitalDevice(sol::table& table)
{
    table.set_function("IsDown", &Device::IsDown);
    table.set_function("IsUp", &Device::IsUp);

    table.set_function("AnyDown", [](sol::variadic_args codes) {
        for (auto code : codes) {
            if (Device::IsDown(code.get<Code>()))
                return true;
        }
        return false;
    });

    table.set_function("AllDown", [](sol::variadic_args codes) {
        for (auto code : codes) {
            if (Device::IsUp(code.get<Code>()))
                return false;
        }
        return true;
    });

    table.set_function("Axis", [](Code negative, Code positive, sol::optional<float> scale) {
        return Device::Axis(negative, positive, scale.value_or(1.0f));
    });
}

}

void InputLuaBindings::RegisterKeys(sol::state& lua)
{
    using Key = sf::Keyboard::Key;

#define SG_KEY(name) { #name, Key::name }
    lua.new_enum<Key>("Key", {
        SG_KEY(A), SG_KEY(B), SG_KEY(C), SG_KEY(D), SG_KEY(E), SG_KEY(F), SG_KEY(G),
        SG_KEY(H), SG_KEY(I), SG_KEY(J), SG_KEY(K), SG_KEY(L), SG_KEY(M), SG_KEY(N),
        SG_KEY(O), SG_KEY(P), SG_KEY(Q), SG_KEY(R), SG_KEY(S), SG_KEY(T), SG_KEY(U),
        SG_KEY(V), SG_KEY(W), SG_KEY(X), SG_KEY(Y), SG_KEY(Z),

        SG_KEY(Num0), SG_KEY(Num1), SG_KEY(Num2), SG_KEY(Num3), SG_KEY(Num4),
        SG_KEY(Num5), SG_KEY(Num6), SG_KEY(Num7), SG_KEY(Num8), SG_KEY(Num9),

        SG_KEY(Numpad0), SG_KEY(Numpad1), SG_KEY(Numpad2), SG_KEY(Numpad3), SG_KEY(Numpad4),
        SG_KEY(Numpad5), SG_KEY(Numpad6), SG_KEY(Numpad7), SG_KEY(Numpad8), SG_KEY(Numpad9),

        SG_KEY(F1),  SG_KEY(F2),  SG_KEY(F3),  SG_KEY(F4),  SG_KEY(F5),
        SG_KEY(F6),  SG_KEY(F7),  SG_KEY(F8),  SG_KEY(F9),  SG_KEY(F10),
        SG_KEY(F11), SG_KEY(F12), SG_KEY(F13), SG_KEY(F14), SG_KEY(F15),

        SG_KEY(Left), SG_KEY(Right), SG_KEY(Up), SG_KEY(Down),

        SG_KEY(Escape), SG_KEY(Space), SG_KEY(Enter), SG_KEY(Backspace), SG_KEY(Tab),
        SG_KEY(Insert), SG_KEY(Delete), SG_KEY(Home), SG_KEY(End),
        SG_KEY(PageUp), SG_KEY(PageDown), SG_KEY(Pause), SG_KEY(Menu),

        SG_KEY(LControl), SG_KEY(RControl), SG_KEY(LShift), SG_KEY(RShift),
        SG_KEY(LAlt), SG_KEY(RAlt), SG_KEY(LSystem), SG_KEY(RSystem),

        SG_KEY(LBracket), SG_KEY(RBracket), SG_KEY(Semicolon), SG_KEY(Comma),
        SG_KEY(Period), SG_KEY(Apostrophe), SG_KEY(Slash), SG_KEY(Backslash),
        SG_KEY(Grave), SG_KEY(Equal), SG_KEY(Hyphen),

        SG_KEY(Add), SG_KEY(Subtract), SG_KEY(Multiply), SG_KEY(Divide)
    });
#undef SG_KEY
}

void InputLuaBindings::RegisterMouseButtons(sol::state& lua)
{
    using Button = sf::Mouse::Button;

    lua.new_enum<Button>("MouseButton", {
        { "Left",   Button::Left },
        { "Right",  Button::Right },
        { "Middle", Button::Middle },
        { "Extra1", Button::Extra1 },
        { "Extra2", Button::Extra2 }
    });
}

void InputLuaBindings::RegisterKeyboard(sol::state& lua)
{
    sol::table keyboard = lua.create_named_table("Keyboard");
    BindDigitalDevice<Input::Keyboard, sf::Keyboard::Key>(keyboard);
}

void InputLuaBindings::RegisterMouse(sol::state& lua)
{
    sol::table mouse = lua.create_named_table("Mouse");
    BindDigitalDevice<Input::Mouse, sf::Mouse::Button>(mouse);

    mouse.set_function("Position", []() {
        return Input::Mouse::Position(Game::GetInstance()->GetWindow().window);
    });

    mouse.set_function("ScaledPosition", [](const Core::vec2f& scale) {
        return Input::Mouse::ScaledPosition(Game::GetInstance()->GetWindow().window, scale);
    });
}

void InputLuaBindings::Register(sol::state& lua)
{
    RegisterKeys(lua);
    RegisterMouseButtons(lua);
    RegisterKeyboard(lua);
    RegisterMouse(lua);
}

}