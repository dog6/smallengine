#pragma once

#include <string>
#include <memory>
#include <sol/sol.hpp>
#include "scripting/lua_script.hpp"

#include "scripting/ecs_lua_bindings.hpp"
#include "scripting/graphics_lua_bindings.hpp"

namespace sg::Scripting {


	class LuaManager {

	private:

		static std::vector<LuaScript> s_scripts;
		static void Load(LuaScript* script);

    	inline static sg::Scripting::Bindings::ECSLuaBindings ecs_lua_bindings;
		inline static sg::Scripting::Bindings::GraphicsLuaBindings graphics_lua_bindings;
        
        public:
        
        static std::unique_ptr<LuaScript> LoadFromPath(const char* filepath);
		static void Update(float deltaTime, LuaScript& script);
		static void BindToLua(sol::state& lua);
		static void Execute(std::string luaCode);

	};

}