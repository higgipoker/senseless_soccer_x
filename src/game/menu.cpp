#include "menu.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
namespace ss {
Menu::Menu (sf::RenderWindow& wnd) : Gamestate (wnd) {
}

void Menu::start() {
}

void Menu::end() {
}

void Menu::handle_input (const sf::Event& event) {
    switch (event.type) {
    case sf::Event::KeyPressed :
        if (event.key.code ==  sf::Keyboard::Escape) {
            std::cout << "ESC" << std::endl;
        }
        break;
    default:
        break;

    }
}
void Menu::update() {
}

void Menu::draw() {
}
}
