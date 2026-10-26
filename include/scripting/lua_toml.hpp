#pragma once

#include <string>
#include "scripting/lua_script.hpp"

namespace sg::Scripting {

    // Describes a LuaTOML config file
    class LuaTOML {

        private: 
            int GetPathsFromTOML();

        public:
        
            LuaTOML(std::string toml_path);
            ~LuaTOML();
            std::string toml_path;
            std::vector<std::string> script_paths;

            

    };

}