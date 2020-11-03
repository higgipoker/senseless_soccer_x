#include "game.h"
#include "../menu/button.h"

#include <iostream>
#include <memory>

namespace ss {
void Game::init() {
    std::cout << __FUNCTION__  << std::endl;
    window.setFramerateLimit (60);
    std::unique_ptr<Widget> btn1 = std::make_unique<Button>(window);
    btn1->setPosition({100,100});
    menu.addwidget(std::move(btn1));
}

void Game::run() {
    std::cout << __FUNCTION__  << std::endl;
    while (window.isOpen()) {
        gamepad.update();
        gamestate.frame();
    }

}

void Game::end() {
    std::cout << __FUNCTION__  << std::endl;
}
}// namespace ss
