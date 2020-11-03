#pragma once
#include "gamestate.h"
#include "../menu/menu.h"
#include "../input/gamepad.h"
#include <SFML/Graphics.hpp>

namespace ss {
class Game{
public:
   void init();
   void run();
   void end();
private:
    sf::RenderWindow window{sf::VideoMode(800, 600), "Senseless Soccer"};
    Menu menu{window};
    Gamestate &gamestate{menu};
    Gamepad gamepad;
};    
}// namespace ss
