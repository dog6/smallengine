#pragma once


#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace sg::Core {

// Describes anything in the game world

    class Entity {

        private:
        
            sf::Transform transform;
            sf::Drawable* drawable;

        public:


            Entity();
            ~Entity();

            void SetDrawable(sf::Drawable& _drawable);
            sf::Drawable& GetDrawable();

            // No SetTransform since Transform is set in the Entity constructor;
            sf::Transform* GetTransform();

    };


};