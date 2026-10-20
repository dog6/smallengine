#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace sg::Graphics {

    class ShapeFactory {
        public:
            static std::unique_ptr<sf::Drawable> MakeRectangle(sf::Vector2f size, sf::Color color);
            static std::unique_ptr<sf::Drawable> MakeCircle(float radius, sf::Color color);

    };


};