#pragma once


#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace sg::Core {

// Describes anything in the game world

    class Entity {

        private:
        
            sf::Drawable* drawable;

        public:

            Entity();
            ~Entity();

            void SetDrawable(sf::Drawable& _drawable);
            sf::Drawable& GetDrawable();

    };


};