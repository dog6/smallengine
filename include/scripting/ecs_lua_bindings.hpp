#pragma once

#include <sol/sol.hpp>

namespace sg::Scripting::Bindings {

    class ECSLuaBindings {

        public:
            static void Register(sol::state& lua);

    };

};