#include "content/entity_factory.hpp"
#include "core/smath.hpp"

using namespace sg::Content;
using namespace sg::Core;
using namespace sg::Graphics;

std::unique_ptr<Entity> sg::Content::EntityFactory::CreateCircleEntity(
    std::string name, b2WorldId worldId, vec2f pos, float radius, 
    sf::Color col, bool isStatic, bool useMeterScale)
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    b2Circle circle{};
    circle.center = {0.0f, 0.0f};

    b2Vec2 physics_pos;
    if (useMeterScale) {
        physics_pos = pos.toBox2D();
        circle.radius = radius / PIXELS_PER_METER;
    } else {
        physics_pos = vec2f::pixelsToMeters(pos).toBox2D();
        circle.radius = radius;
    }

    // Create and add PhysicsComponent
    ent->AddComponent<PhysicsComponent>(isStatic, physics_pos, worldId, circle);

    // Create and add DrawableComponent
    auto shape = ShapeFactory::MakeCircle(radius, col);
    ent->AddComponent<DrawableComponent>(std::move(shape));

    return ent;
}

std::unique_ptr<Entity> sg::Content::EntityFactory::CreateRectEntity(
    std::string name, b2WorldId worldId, vec2f pos, vec2f size, 
    sf::Color col, bool isStatic, bool useMeterScale) 
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    b2Vec2 physics_pos;
    b2Polygon box_polygon;
    vec2f drawable_size;

    if (useMeterScale) {
        physics_pos = pos.toBox2D();
        box_polygon = b2MakeBox(size.x * 0.5f, size.y * 0.5f);
        drawable_size = vec2f::metersToPixels(size);
    } else {
        physics_pos = vec2f::pixelsToMeters(pos).toBox2D();
        vec2f size_meters = vec2f::pixelsToMeters(size);
        box_polygon = b2MakeBox(size_meters.x * 0.5f, size_meters.y * 0.5f);
        drawable_size = size;
    }

    // Create and add PhysicsComponent
    ent->AddComponent<PhysicsComponent>(isStatic, physics_pos, worldId, box_polygon);

    // Create and add DrawableComponent
    auto rect = ShapeFactory::MakeRectangle(drawable_size.toSFML(), col);
    ent->AddComponent<DrawableComponent>(std::move(rect));

    return ent;
}
