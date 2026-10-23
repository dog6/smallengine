#pragma once

#include <sol/sol.hpp>
#include "scripting/lua_bindings.hpp"

namespace sg::Scripting::Bindings {

    class GraphicsLuaBindings : public LuaBindings {

            void RegisterColor(sol::state& lua);

        public:
            void Register(sol::state& lua) override;

    };

};