#include "game.h"
#include "../menu/button.h"

#include <iostream>
#include <memory>

namespace ss {
namespace game {
    
std::unique_ptr<Widget> make_button (sf::RenderWindow& wnd, const MenuEvent evt,  const std::string& caption, const sf::Vector2f& pos, const sf::Color& col) {
    std::unique_ptr<Widget> btn = std::make_unique<Button> (wnd, evt, caption, col);
    btn->setPosition (pos);
    return btn;
}

void Game::init() {
    std::cout << __FUNCTION__  << std::endl;
    window.setFramerateLimit (60);

    menu.addwidget (make_button (window, MenuEvent::Button1, "BUTTON 1", {100, 100}, sf::Color::Blue));
    menu.addwidget (make_button (window, MenuEvent::Button2, "BUTTON 2", {100, 200}, sf::Color::Red));
    menu.addwidget (make_button (window, MenuEvent::Button3, "BUTTON 3", {100, 300}, sf::Color::Green));
    menu.addwidget (make_button (window, MenuEvent::Button4, "BUTTON 4", {100, 400}, sf::Color::Yellow));
    menu.addwidget (make_button (window, MenuEvent::Exit, "EXIT", {100, 400}, sf::Color::Cyan));
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

}// namespace game
}// namespace ss
