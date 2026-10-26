#pragma once

#include <sol/sol.hpp>
#include "scripting/lua_bindings.hpp"

namespace sg::Scripting::Bindings {

    class InputLuaBindings : public LuaBindings {

            void RegisterKeys(sol::state& lua);
            void RegisterMouseButtons(sol::state& lua);
            void RegisterKeyboard(sol::state& lua);
            void RegisterMouse(sol::state& lua);

        public:
            void Register(sol::state& lua) override;

    };

}