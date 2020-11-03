#include "menu.h"
#include "page.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
namespace ss {

Menu::Menu (sf::RenderWindow& wnd) : Gamestate (wnd) {
    // every menu has a defualt first page
    pages.push_back (std::move (std::make_unique<Page> (wnd)));
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
    for (auto& page : pages) {
        page->draw();
    }
}

void Menu::addwidget (std::unique_ptr<Widget> w, int page) {
    pages[page]->addChild (std::move (w));
}
}
