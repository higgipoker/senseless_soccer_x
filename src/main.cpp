#include "game/Gamestate.h"
#include "menu/Menu.h"
#include "game/Match.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace ss;
using namespace game;

sf::RenderWindow window (sf::VideoMode{800, 600}, "Senseless Soccer");
Menu menu (window);
Match match (window);
Gamestate* gamestate = &menu;

void changeState (Gamestate* gamestate) {
    gamestate->end();
    switch (gamestate->nextState()) {
    case State::Main_Menu:
        gamestate = &menu;
        break;
    case State::Match:
        gamestate = &match;
        break;
    }
    gamestate->start();
}
int main() {
    std::cout << "Senseless soccer started" << std::endl;
    window.setFramerateLimit (60);
    while (window.isOpen()) {
        gamestate->frame();
        if (gamestate->stateOver()) {
            changeState (gamestate);
        }
    }
    std::cout << "Senseless Soccer finished" << std::endl;
    return 0;
}
