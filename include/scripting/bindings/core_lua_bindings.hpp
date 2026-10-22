#pragma once

#include <sol/sol.hpp>
#include "scripting/lua_bindings.hpp"

namespace sg::Scripting::Bindings {

    class CoreLuaBindings : public LuaBindings {

            static void RegisterVec2(sol::state& lua);
            static void RegisterUVec2(sol::state& lua);
            static void RegisterVec2f(sol::state& lua);
            static void RegisterMathFunctions(sol::state& lua);
        public:
            void Register(sol::state& lua) override;

    };

};