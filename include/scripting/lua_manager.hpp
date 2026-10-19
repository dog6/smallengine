#pragma once

#include <string>
#include <memory>
#include <sol/sol.hpp>
#include "scripting/lua_script.hpp"

// #ifdef NG_DEVELOPER_MODE
// #include "AVGNG/Editor/ConsoleView.hpp"
// #endif

namespace sg::Scripting {


	class LuaManager {

	private:

		static std::vector<LuaScript> s_scripts;
		static void Load(LuaScript* script);
        
        public:
        
        static std::unique_ptr<LuaScript> LoadFromPath(const char* filepath);
		static void Update(float deltaTime, LuaScript& script);
		static void BindToLua(sol::state& lua);
		static void Execute(std::string luaCode);

	};

}