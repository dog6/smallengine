#include "scripting/ecs_lua_bindings.hpp"
#include "ecs/entity.hpp"
#include "ecs/component.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/components/physics_component.hpp"
#include "ecs/components/drawable_component.hpp"

using namespace sg::Scripting;
using namespace sg::ECS::Components;
using namespace sg::ECS;

static sol::object WrapComponent(sol::this_state s, const std::string& name, Component* comp)
{
    if (!comp) return sol::nil;
    if (name == "PhysicsComponent")  return sol::make_object(s, static_cast<PhysicsComponent*>(comp));
    if (name == "DrawableComponent") return sol::make_object(s, static_cast<DrawableComponent*>(comp));
    return sol::make_object(s, comp);
}

static void RegisterEntity(sol::state& lua)
{
    lua.new_usertype<Entity>("Entity",
        // "name", &Entity::name,
        "AddComponent", [](Entity& self, const std::string& name, sol::this_state s) {
            // No AddComponentByName() method exists
            // We need a way to implement more lua scripts
            return WrapComponent(s, name, self.AddComponentByName(name));
        },
        "GetComponent", [](Entity& self, const std::string& name, sol::this_state s) {
            return WrapComponent(s, name, self.GetComponentByName(name));
        }
    );
}

static void RegisterEntityManager(sol::state& lua){
    
    lua.new_usertype<Entity>("Entman", 
        "MakeEntity", &EntityManager::MakeEntity,
        "RemoveEntity", &EntityManager::RemoveEntity
    );


}


void ECSLuaBindings::Register(sol::state &lua)
{

    RegisterEntity(lua);

}