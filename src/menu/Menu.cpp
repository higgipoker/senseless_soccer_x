#include "Menu.hpp"
#include "Page.hpp"
#include "AnimationFlash.hpp"
#include "events.hpp"
#include "Button.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>
namespace ss {

std::unique_ptr<Widget> make_button (sf::RenderWindow& wnd, const MenuEvent evt,  const std::string& caption, const sf::Vector2f& pos, const sf::Color& col) {
    std::unique_ptr<Widget> btn = std::make_unique<Button> (wnd, evt, caption, pos, col);
    return btn;
}

Menu::Menu (sf::RenderWindow& wnd, Gamepad& controller) : window (wnd), gamepad (controller) {
    gamepad.attachListener (*this);
    // every menu has a defualt first page
    pages.push_back (std::make_unique<Page> (wnd));
    active_page = pages[0].get();

    sf::Color color_std {0, 120, 10};
    sf::Color color_ext {55, 0, 0};
    float btn_x = window.getSize().x / 2 - 500 / 2;
    Widget* button1 = addwidget (make_button (window, MenuEvent::Friendly, "FRIENDLY ", {btn_x, 250}, color_std));
    Widget* button2 = addwidget (make_button (window, MenuEvent::Button2, "LEAGUE", {btn_x, 320}, color_std));
    Widget* button3 = addwidget (make_button (window, MenuEvent::Button3, "CUP", {btn_x, 390}, color_std));
    Widget* button4 = addwidget (make_button (window, MenuEvent::Button4, "CAREER", {btn_x, 460}, color_std));
    Widget* button5 = addwidget (make_button (window, MenuEvent::Exit,    "EXIT", {btn_x, 530}, color_ext));

    button1->neighbours.below = button2;
    button2->neighbours.above = button1;

    button2->neighbours.below = button3;
    button3->neighbours.above = button2;

    button3->neighbours.below = button4;
    button4->neighbours.above = button3;

    button4->neighbours.below = button5;
    button5->neighbours.above = button4;

    active_page->setAlpha (0);
    page_animation.changeTarget (active_page);
    page_animation.start();
}

Menu::~Menu() {
    gamepad.detatchListener (*this);
}

void Menu::waitForNoKey(){
    sf::Event event;
    while (window.pollEvent (event));
}

MenuEvent Menu::frame() {
    
    update_mouse();
    if (active_page) {
        active_widget = active_page->getActiveWidget();

        // input
        if (window.hasFocus()) {
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

        // render
        window.clear (sf::Color::Black);
        if (active_page) {
            active_page->draw();
        }
        window.display();
    }
    MenuEvent ret = return_code;
    return_code = MenuEvent::None;
    return ret;
}

Widget* Menu::addwidget (std::unique_ptr<Widget> w, int page) {
    Widget* widget =  pages[page]->addChild (std::move (w));
    pages[page]->setContext (this);
    widget->setContext (this);
    if (pages.size() == 1 && pages[page]->childCount() == 1) {
        active_animation.changeTarget (widget);
        active_animation.start();
    }
    return widget;
}

void Menu::onInputEvent (const InputEvent in_event, const std::vector<int>& in_params) {
    if (in_event == InputEvent::FireDown) {
        fire_pressed = true;
    }
}

void Menu::update_mouse() {
    sf::Vector2i m = sf::Mouse::getPosition (window);
    mouse_position = window.mapPixelToCoords(m);   
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
                return_code = MenuEvent::Exit;
            } else if (event.key.code == sf::Keyboard::W) {
                active_page->up();
            } else if (event.key.code == sf::Keyboard::S)  {
                active_page->down();
            } else if (event.key.code == sf::Keyboard::A) {
                active_page->left();
            } else if (event.key.code == sf::Keyboard::D) {
                active_page->right();
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (active_widget) {
                    return_code = active_widget->action();
                }
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
            return_code = active_widget->action();
        }
    }
}

void Menu::read_gamepad() {
    gamepad.update();
    if (!wait_for_gamepad()) {
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
    }
    if (fire_pressed) {
        mouse_mode = false;
        fire_pressed = false;
        return_code = active_widget->action();
    }
}
} // namespace ss
