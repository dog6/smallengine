#include "scripting/bindings/ecs_lua_bindings.hpp"
#include "ecs/entity.hpp"
#include "ecs/component.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/components/physics_component.hpp"
#include "ecs/components/drawable_component.hpp"
#include "content/entity_factory.hpp"
#include <sol/sol.hpp>
#include "graphics/scolor.hpp"

using namespace sg::Scripting;
using namespace sg::Scripting::Bindings;

using namespace sg::ECS::Components;
using namespace sg::ECS;

using namespace sg::Graphics;
using namespace sg::Content;

// Helper function
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
        "AddComponent", [](Entity& self, const std::string& name, sol::this_state s) {
            return WrapComponent(s, name, self.AddComponentByName(name));
        },
        "GetComponent", [](Entity& self, const std::string& name, sol::this_state s) {
            return WrapComponent(s, name, self.GetComponentByName(name));
        },
        "SetPosition", &Entity::SetPosition,
        "GetPosition", &Entity::GetPosition
    );
}


static void RegisterEntityManager(sol::state& lua)
{
    lua.new_usertype<EntityManager>("Entman",
        sol::no_constructor,
        "MakeEntity",   &EntityManager::MakeEntity,
        "RemoveEntity", &EntityManager::RemoveEntity,

        "AddCircle", sol::overload(
            [](std::string name, b2WorldId w, vec2f pos, float radius,
               ColorRGB col, bool isStatic, bool useMeters) -> Entity*
            {
                auto& stored = EntityManager::AddEntity(
                    EntityFactory::CreateCircleEntity(
                        std::move(name), w, pos, radius,
                        col.toSFColor(), isStatic, useMeters
                    )
                );

                return stored.get();
            },
            [](std::string name, b2WorldId w, vec2f pos, float radius,
               ColorRGBA col, bool isStatic, bool useMeters) -> Entity*
            {
                auto& stored = EntityManager::AddEntity(
                    EntityFactory::CreateCircleEntity(
                        std::move(name), w, pos, radius,
                        col.toSFColor(), isStatic, useMeters
                    )
                );

                return stored.get();
            }
        ),

        "AddRect", sol::overload(
            [](std::string name, b2WorldId w, vec2f pos, vec2f size,
               ColorRGB col, bool isStatic, bool useMeters) -> Entity*
            {
                auto& stored = EntityManager::AddEntity(
                    EntityFactory::CreateRectEntity(
                        std::move(name), w, pos, size,
                        col.toSFColor(), isStatic, useMeters
                    )
                );

                return stored.get();
            },
            [](std::string name, b2WorldId w, vec2f pos, vec2f size,
               ColorRGBA col, bool isStatic, bool useMeters) -> Entity*
            {
                auto& stored = EntityManager::AddEntity(
                    EntityFactory::CreateRectEntity(
                        std::move(name), w, pos, size,
                        col.toSFColor(), isStatic, useMeters
                    )
                );

                return stored.get();
            }
        )
    );
}


static void RegisterEntityFactory(sol::state& lua)
{
    using sg::Content::EntityFactory;

    lua.new_usertype<EntityFactory>("EntityFactory",
        sol::no_constructor,

        "CreateCircleEntity", sol::overload(
            [](std::string name, b2WorldId w, vec2f pos, float radius,
               ColorRGB col, bool isStatic, bool useMeters) {
                return EntityFactory::CreateCircleEntity(
                    std::move(name), w, pos, radius, col.toSFColor(), isStatic, useMeters);
            },
            [](std::string name, b2WorldId w, vec2f pos, float radius,
               ColorRGBA col, bool isStatic, bool useMeters) {
                return EntityFactory::CreateCircleEntity(
                    std::move(name), w, pos, radius, col.toSFColor(), isStatic, useMeters);
            }
        ),

        "CreateRectEntity", sol::overload(
            [](std::string name, b2WorldId w, vec2f pos, vec2f size,
               ColorRGB col, bool isStatic, bool useMeters) {
                return EntityFactory::CreateRectEntity(
                    std::move(name), w, pos, size, col.toSFColor(), isStatic, useMeters);
            },
            [](std::string name, b2WorldId w, vec2f pos, vec2f size,
               ColorRGBA col, bool isStatic, bool useMeters) {
                return EntityFactory::CreateRectEntity(
                    std::move(name), w, pos, size, col.toSFColor(), isStatic, useMeters);
            }
        )
    );
}


// static void Register
void ECSLuaBindings::Register(sol::state &lua)
{

    RegisterEntity(lua);
    RegisterEntityManager(lua);
    RegisterEntityFactory(lua);
    lua["entman"] = &EntityManager::entman;

}