#pragma once

#include <sol/sol.hpp>

namespace sg::Scripting {

    class LuaBindings {

        public:
            static void Register(sol::state& lua);

    };

};