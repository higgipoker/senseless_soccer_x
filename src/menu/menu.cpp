#include "menu.h"
#include "page.h"
#include "animation_flash.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
namespace ss {

Menu::Menu (sf::RenderWindow& wnd) : Gamestate (wnd) {
    // every menu has a defualt first page
    pages.push_back (std::move (std::make_unique<Page> (wnd)));
}

Menu::~Menu() {
    for (auto& anim : animations) {
        delete anim;
    }
}

void Menu::start() {
}

void Menu::end() {
}

void Menu::handle_input (const sf::Event& event) {
    switch (event.type) {
    case sf::Event::KeyPressed :
        if (event.key.code ==  sf::Keyboard::Escape) {
        }
        break;
    default:
        break;
    }
}
void Menu::update() {
    for (auto& anim : animations)
        anim->update();
}

void Menu::draw() {
    for (auto& page : pages) {
        page->draw();
    }
}

void Menu::addwidget (std::unique_ptr<Widget> w, int page) {
    if (pages.size() == 1 && pages[page]->childCount() == 0) {
        Animation* f = new AnimationFlash (*w);
        animations.push_back (f);
    }
    pages[page]->addChild (std::move (w));
}
}
