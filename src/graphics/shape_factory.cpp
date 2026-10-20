#include "graphics/shape_factory.hpp"

using namespace sg::Graphics;

std::unique_ptr<sf::Drawable> ShapeFactory::MakeRectangle(sf::Vector2f size, sf::Color color)
{
    std::unique_ptr<sf::RectangleShape> rect = std::make_unique<sf::RectangleShape>(size);
    rect->setFillColor(color);
    rect->setOrigin(size*0.5f);
    rect->setPosition(size);
    return rect;
}

std::unique_ptr<sf::Drawable> ShapeFactory::MakeCircle(float radius, sf::Color color)
{
    std::unique_ptr<sf::CircleShape> circle = std::make_unique<sf::CircleShape>(radius);
    circle->setFillColor(color);
    sf::Vector2f size = circle.get()->getScale();
    circle->setOrigin(size*0.5f);
    circle->setPosition(size);
    return circle;
}
