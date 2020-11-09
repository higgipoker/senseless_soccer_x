#pragma once
#include "gamestate.h"
#include "../menu/menu.h"
#include "../input/gamepad.h"
#include <SFML/Graphics.hpp>

namespace ss {
namespace game {
class Game {
public:
    explicit Game (sf::RenderWindow& wnd) : window (wnd) {};
    void init();
    void run();
    void end();
private:
    sf::RenderWindow& window;
    Menu menu{window};
    Gamestate& gamestate{menu};
    Gamepad gamepad;
};
}// namespace game
}// namespace ss
