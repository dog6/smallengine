#pragma once

#include <SFML/Graphics.hpp>
#include "core/settings.hpp"

class Game {

    private:
        inline static Settings settings;
        sf::RenderWindow gameWindow;
        
        // Called every frame
        void Run(); // game loop

        // Called on first frame
        void Start();
        
        // Called after game initializes
        void Load();
    public:

        // Default constructors
        Game() {}
        ~Game() {}

        // Called first
        void Init();



};
