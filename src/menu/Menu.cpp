#include "Menu.h"
#include "Page.h"
#include "AnimationFlash.h"
#include "events.h"
#include "Button.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
namespace ss {
namespace game {

std::unique_ptr<Widget> make_button (sf::RenderWindow& wnd, const MenuEvent evt,  const std::string& caption, const sf::Vector2f& pos, const sf::Color& col) {
    std::unique_ptr<Widget> btn = std::make_unique<Button> (wnd, evt, caption, col);
    btn->setPosition (pos);
    return btn;
}

Menu::Menu (sf::RenderWindow& wnd) : Gamestate (wnd) {
    name = "menu";
    // every menu has a defualt first page
    pages.push_back (std::make_unique<Page> (wnd));

    addwidget (make_button (window, MenuEvent::Button1, "BUTTON 1", {100, 100}, sf::Color::Blue));
    addwidget (make_button (window, MenuEvent::Button2, "BUTTON 2", {100, 200}, sf::Color::Red));
    addwidget (make_button (window, MenuEvent::Button3, "BUTTON 3", {100, 300}, sf::Color::Green));
    addwidget (make_button (window, MenuEvent::Button4, "BUTTON 4", {100, 400}, sf::Color::Yellow));
    addwidget (make_button (window, MenuEvent::Exit, "EXIT", {100, 400}, sf::Color::Cyan));
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

void Menu::handle_input () {
    if (window.hasFocus()) {
        sf::Event event;
        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            switch (event.type) {
            case sf::Event::KeyPressed :
                if (event.key.code ==  sf::Keyboard::Escape) {
                    window.close();
                }
                break;
            default:
                break;
            }
        }
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
} // namesace game
} // namespace ss
