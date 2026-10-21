#pragma once

#include <sol/sol.hpp>

namespace sg::Scripting {

    class LuaBindings {

        public:
            virtual  void Register(sol::state& lua) = 0;

    };

};