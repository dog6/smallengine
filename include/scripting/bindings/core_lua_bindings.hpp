#pragma once

#include <sol/sol.hpp>
#include "scripting/lua_bindings.hpp"

namespace sg::Scripting::Bindings {

    class CoreLuaBindings : public LuaBindings {

            void RegisterVec2(sol::state& lua);
            void RegisterUVec2(sol::state& lua);
            void RegisterVec2f(sol::state& lua);
            void RegisterMathFunctions(sol::state& lua);
            void RegisterGameWindow(sol::state& lua);

        public:
            void Register(sol::state& lua) override;

    };

};