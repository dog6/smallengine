#pragma once

#include <string>
#include <memory>
#include <sol/sol.hpp>
#include "scripting/lua_script.hpp"

#include "scripting/bindings/ecs_lua_bindings.hpp"
#include "scripting/bindings/graphics_lua_bindings.hpp"
#include "scripting/bindings/core_lua_bindings.hpp"
#include "scripting/bindings/input_lua_bindings.hpp"


namespace sg::Scripting {


	class LuaManager {

	private:

		static void Load(LuaScript* script);
	
		inline static std::vector<std::unique_ptr<LuaScript>> s_scripts;
    	inline static sg::Scripting::Bindings::ECSLuaBindings ecs_lua_bindings;
		inline static sg::Scripting::Bindings::GraphicsLuaBindings graphics_lua_bindings;
		inline static sg::Scripting::Bindings::CoreLuaBindings core_lua_bindings;
		inline static sg::Scripting::Bindings::InputLuaBindings input_lua_bindings;

        
        public:
        static void LoadFromTOML(std::string config_path);
        static std::unique_ptr<LuaScript> LoadFromPath(const char* filepath);
		static void Update(float deltaTime);
		static void BindToLua(sol::state& lua);
		static void Execute(std::string luaCode);

	};

}