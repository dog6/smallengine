#include "content/entity_factory.hpp"
#include "core/smath.hpp"
#include "core/b2sf.hpp"


using namespace sg::Content;
using namespace sg::Core;
using namespace sg::Graphics;


std::unique_ptr<Entity> EntityFactory::CreateCircleEntityPixels(std::string name, b2WorldId worldId, 
                                        sf::Vector2f pixel_pos, float pixel_radius, sf::Color col, bool isStatic)
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // Create and add PhysicsComponent
    b2Circle circle{};
    circle.center = {0.0f, 0.0f};
    circle.radius = pixel_radius;
    ent->AddComponent<PhysicsComponent>(isStatic, pixelsToMeters(pixel_pos), worldId, circle);

    // Create and add DrawableComponent
    auto shape = ShapeFactory::MakeCircle(pixel_radius, col);
    ent->AddComponent<DrawableComponent>(std::move(shape));

    return ent;
}

std::unique_ptr<Entity> EntityFactory::CreateCircleEntityMeters(std::string name, b2WorldId worldId,
                                                                             b2Vec2 meter_pos, float meter_radius, sf::Color col, bool isStatic)
{
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // Create and add PhysicsComponent
    b2Circle circle{};
    circle.center = {0.0f, 0.0f};
    circle.radius = meter_radius / PIXELS_PER_METER;
    ent->AddComponent<PhysicsComponent>(isStatic, meter_pos, worldId, circle);

    // Create and add DrawableComponent
    auto shape = ShapeFactory::MakeCircle(meter_radius, col);
    ent->AddComponent<DrawableComponent>(std::move(shape));

    return ent;
}

std::unique_ptr<Entity> EntityFactory::CreateRectEntityPixels(std::string name, b2WorldId worldId,
                                    sf::Vector2f pixel_pos, sf::Vector2f pixel_size, sf::Color col, bool isStatic) {

    // Create new unique_ptr<Entity>
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // Create and add PhysicsComponent
    b2Vec2 b2size = pixelsToMeters(pixel_size);
    b2Polygon box_polygon = b2MakeBox(b2size.x * 0.5f, b2size.y * 0.5f);
    ent->AddComponent<PhysicsComponent>(isStatic, pixelsToMeters(pixel_pos), worldId, box_polygon);

    // Create and add DrawableComponent
    auto rect = ShapeFactory::MakeRectangle(pixel_size, col);
    ent->AddComponent<DrawableComponent>(std::move(rect));

    return ent;
}

std::unique_ptr<Entity> EntityFactory::CreateRectEntityMeters(std::string name, b2WorldId worldId,
                                  b2Vec2 meter_pos, b2Vec2 meter_size, sf::Color col, bool isStatic) {

    // Create new unique_ptr<Entity>
    std::unique_ptr<Entity> ent = std::make_unique<Entity>(name);

    // TODO NEXT --> Implement a way to create b2Polygons inside lua and assign them to drawables
    b2Polygon poly = b2MakeBox(meter_size.x * 0.5f, meter_size.y * 0.5f);
    ent->AddComponent<PhysicsComponent>(isStatic, meter_pos, worldId, poly);

    // Create and add DrawableComponent
    sf::Vector2f sf_size = metersToPixels(meter_size);
    auto rect = ShapeFactory::MakeRectangle(sf_size, col);
    ent->AddComponent<DrawableComponent>(std::move(rect));

    return ent;
}