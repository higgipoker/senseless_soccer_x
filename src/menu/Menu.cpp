#include "Menu.hpp"
#include "Page.hpp"
#include "AnimationFlash.hpp"
#include "events.hpp"
#include "Button.hpp"
#include "PageMain.hpp"

#include <SFML/Window/Event.hpp>
#include <iostream>
namespace ss {
static const int RETURN_NOTHING = 0;
static const int RETURN_EXIT = 1;
static const int RETURN_MATCH = 2;

Menu::Menu (sf::RenderWindow& wnd, Controller& controller) : window (wnd), gamepad (controller), mainpage (wnd, this), calibratepage (wnd, this) {
    active_page = &mainpage;

    active_page->setAlpha (0);
    page_animation.changeTarget (active_page);
    page_animation.start();

    active_animation.changeTarget (active_page->getActiveWidget());
    active_animation.start();
}

Menu::~Menu() {
}

int Menu::run() {
    while (!exit) {
        // input
        if (window.hasFocus()) {
            update_mouse();
            read_keyboard();
            read_gamepad();
            read_mouse();
        }

        // update
        active_page->update();
        if (active_page->getActiveWidget() != active_widget) {
            active_widget = active_page->getActiveWidget();
            active_animation.stop();
            active_animation.changeTarget (active_widget);
            active_animation.start();
        }

        if (!page_animation.finished()) {
            page_animation.update();
        }
        active_animation.update();


        // draw
        window.clear (sf::Color::Black);
        active_page->draw();
        window.display();
    }
    return return_code;
}

void Menu::changePage (const Page_ID id) {
    active_page->onHide();
    switch (id) {
    case Page_ID::Main:
        last_page = nullptr;
        active_page = &mainpage;
        break;
    case Page_ID::Calibrate:
        last_page = &mainpage;
        active_page = &calibratepage;
        break;
    }
    active_page->onShow();
    active_animation.stop();
    active_animation.changeTarget (active_page->getActiveWidget());
    active_animation.start();
}

void Menu::update_mouse() {
    sf::Vector2i m = sf::Mouse::getPosition (window);
    mouse_position = window.mapPixelToCoords (m);
    if (mouse_position != last_mouse_position) {
        mouse_moved = true;
        mouse_mode = true;
        last_mouse_position = mouse_position;
    } else {
        mouse_moved = false;
    }
    mouse_down = sf::Mouse::isButtonPressed (sf::Mouse::Left);
    if (mouse_down) {
        if (!mouse_pressed) {
            mouse_pressed = true;
            mouse_mode = true;
        }
        mouse_down = true;
    } else {
        mouse_down = false;
        mouse_pressed = false;
        wait_for_mouse_up = false;
    }
}

void Menu::read_keyboard() {
    sf::Event event;
    while (window.pollEvent (event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        switch (event.type) {
        case sf::Event::KeyPressed :
            mouse_mode = false;
            if (event.key.code == sf::Keyboard::Escape) {
                return_code = RETURN_EXIT;
                exit = true;
            } else if (event.key.code == sf::Keyboard::W) {
                active_page->up();
            } else if (event.key.code == sf::Keyboard::S)  {
                active_page->down();
            } else if (event.key.code == sf::Keyboard::A) {
                active_page->left();
            } else if (event.key.code == sf::Keyboard::D) {
                active_page->right();
            } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                press_button();
            }
            break;
        default:
            break;
        }
    }
}

void Menu::read_mouse() {
    if (!wait_for_mouse_up) {
        if (mouse_mode && mouse_pressed && active_widget->has_mouse) {
            wait_for_mouse_up = true;
            press_button();
        }
    }
}

void Menu::read_gamepad() {
    gamepad.update();
    if (/*!gamepad_enabled ||*/ wait_for_gamepad()) return;

    if (gamepad.up()) {
        mouse_mode = false;
        active_page->up();
        gamepad_wait = GAMEPAD_WAIT_TIME;
    } else if (gamepad.down()) {
        mouse_mode = false;
        active_page->down();
        gamepad_wait = GAMEPAD_WAIT_TIME;
    } else if (gamepad.left()) {
        mouse_mode = false;
        active_page->left();
        gamepad_wait = GAMEPAD_WAIT_TIME;
    } else if (gamepad.right()) {
        mouse_mode = false;
        active_page->right();
        gamepad_wait = GAMEPAD_WAIT_TIME;
    }
    if (gamepad.state.buttons[0].evt == ButtonEvent::Pressed) {
        if (!waiting_for_js_button_up) {
            waiting_for_js_button_up = true;
            mouse_mode = false;
            fire_pressed = false;
            press_button();
        }
    } else {
        waiting_for_js_button_up = false;
    }
}

void Menu::press_button() {
    if (active_widget) {
        active_widget->onPressed();
        active_page->handleButtonPress (active_widget);
    }
}
} // namespace ss
