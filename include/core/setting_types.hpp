#pragma once
#include <string>
#include "core/smath.hpp"

namespace sg::Core {

    struct ProgInfo {
        std::string version;
        std::string author;
        std::string started_on;
    };

    struct WindowSettings {
        std::string title;
        uvec2 resolution;
        int max_framerate;
    };

};