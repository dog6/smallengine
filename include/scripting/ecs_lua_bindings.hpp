#pragma once

#include <sol/sol.hpp>
#include "scripting/lua_bindings.hpp"

namespace sg::Scripting::Bindings {

    class ECSLuaBindings : public LuaBindings {

        public:
            void Register(sol::state& lua) override;

    };

};