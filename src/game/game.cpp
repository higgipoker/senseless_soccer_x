#include "game.h"
#include <iostream>

namespace ss {
void Game::init() {
    std::cout << __FUNCTION__  << std::endl;
    window.setFramerateLimit ( 60 );
}

void Game::run() {
    std::cout << __FUNCTION__  << std::endl;
    while ( window.isOpen() ) {
        gamestate.frame();
       
        sf::Vector2f js = gamepad.getLeftAnalogDirection();
        std::cout << js.x << ", " << js.y << std::endl;
    }

}

void Game::end() {
    std::cout << __FUNCTION__  << std::endl;
}
}// namespace ss
