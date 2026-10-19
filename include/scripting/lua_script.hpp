#pragma once

#include <sol/sol.hpp>

namespace sg::Scripting {
    struct LuaScript {
        sol::state lua;
        std::string filepath;
    };
};