#include "game.h"
#include "../menu/button.h"

#include <iostream>
#include <memory>

namespace ss {
std::unique_ptr<Widget> make_button (sf::RenderWindow& wnd, const sf::Vector2f& pos, const sf::Color& col) {
    std::unique_ptr<Widget> btn = std::make_unique<Button> (wnd, col);
    btn->setPosition (pos);
    return btn;
}

void Game::init() {
    std::cout << __FUNCTION__  << std::endl;
    window.setFramerateLimit (60);

    menu.addwidget (make_button (window, {100, 100}, sf::Color::Blue));
    menu.addwidget (make_button (window, {100, 200}, sf::Color::Red));
    menu.addwidget (make_button (window, {100, 300}, sf::Color::Green));
    menu.addwidget (make_button (window, {100, 300}, sf::Color::Yellow));
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
