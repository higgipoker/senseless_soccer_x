//
// here we go, a flat, performant non-oop menu system
//
#include "menu.h"
#include "widgets.h"
#include "calibration_page.h"
#include "../global.hpp"
#include "../input/Controller.hpp"
#include "../resources/Resources.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

namespace ss {
namespace menu {

void handle_mouse (Mouse* mouse, Menu* menu, sf::RenderWindow* window) {
    bool was_down = mouse->down;
    mouse->pressed = false;
    mouse->down = sf::Mouse::isButtonPressed (sf::Mouse::Left);
    if (mouse->down && !was_down) {
        mouse->pressed = true;
    } else if (!mouse->down && was_down) {
        mouse->released = true;
        if (has_mouse (menu->active_widget, mouse->position)) {
            menu->event = Menu_Event::FIRE;
        }
    }

    sf::Vector2f last_mouse_position = mouse->position;
    sf::Vector2i m = sf::Mouse::getPosition (*window);
    mouse->position = window->mapPixelToCoords (m);

    if (mouse->position != last_mouse_position) {
        last_mouse_position = mouse->position;
        mouse->moved = true;
    } else {
        mouse->moved = false;
    }
}

void handle_keyboard (Keyboard* keyboard, Menu* menu) {
    if (keyboard->wait_for_repeat) {
        if (--keyboard->repeat_ticks <= 0) {
            keyboard->repeat_ticks = keyboard->repeat_time;
            keyboard->wait_for_repeat = false;
        }
    }
    bool was_pressed = keyboard->keycode == sf::Keyboard::Key::Unknown ? false : true;

    keyboard->keycode = sf::Keyboard::Key::Unknown;

    // check if a key is down (return on first found!)
    for (size_t i = static_cast<int> (sf::Keyboard::A); i < sf::Keyboard::KeyCount; ++i) {
        // note only one key at a time for now
        if (sf::Keyboard::isKeyPressed (static_cast<sf::Keyboard::Key> (i))) {
            keyboard->keycode = static_cast<sf::Keyboard::Key> (i);

            // check for key pressed event
            if (!was_pressed) {
                keyboard->key_pressed = true;
            }

            // check for menu event
            if (!keyboard->wait_for_repeat) {
                if (keymap.find (keyboard->keycode) != keymap.end()) {
                    menu->event =  keymap[keyboard->keycode];
                }
            }
            keyboard->wait_for_repeat = true;
            // early exit, further proccessing is only necessary if no key is down
            return;
        }
    }
}

void handle_window (sf::RenderWindow* window, Menu* menu) {
    sf::Event event;
    while (window->pollEvent (event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            menu->should_exit = true;
        }
    }
}

void handle_gamepad (GamepadController* gamepad, Menu* menu) {
    sf::Joystick::update();
    if (menu->calibrate_layout.selected_gamepad_index >= 0) {
        menu->controllers[menu->calibrate_layout.selected_gamepad_index].update();
    }
}

void init_resources (Menu* menu) {
    menu->resources.texture_menu_background.loadFromFile (resources::gfx_folder() + "bg1.png");
    menu->resources.texture_menu_logo.loadFromFile (resources::gfx_folder() + "logo2.png");
    menu->resources.texture_gamepad.loadFromFile (resources::gfx_folder() + "gamepad_bg.png");
    menu->resources.texture_thumbstick.loadFromFile (resources::gfx_folder() + "thumbstick.png");
    menu->resources.texture_thumbstick.loadFromFile (resources::gfx_folder() + "thumbstick.png");
    menu->resources.font_button.loadFromFile (resources::font_folder() + "swos2.ttf");
}





void init_main_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_main[i];
    sf::FloatRect dimensions{0, 0, 1280, 720};
    init_image_widget (bg, dimensions, &menu->resources.texture_menu_background);
    bg->bounds = dimensions;
    bg->id = i;

    // the logo
    i++;
    Widget* logo = &menu->page_main[i];
    dimensions = {0, 0, 819, 144};
    init_image_widget (logo, dimensions, &menu->resources.texture_menu_logo);
    logo->bounds = dimensions;
    logo->image.img_rect.setOrigin ({logo->image.img_rect.getLocalBounds().width / 2, logo->image.img_rect.getLocalBounds().height / 2});
    logo->image.img_rect.setPosition ({bg->image.img_rect.getLocalBounds().width / 2, logo->image.img_rect.getLocalBounds().height / 2});
    logo->id = i;

    // button 1
    i++;
    Widget* button1 = &menu->page_main[i];
    Button_Attributes attribs;
    attribs.text_font = &menu->resources.font_button;
    attribs.position = {300, 300};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    init_button_widget (&button1->button, attribs);
    button1->bounds = button1->button.btn_rect.getGlobalBounds();
    button1->type = Widget::Button;
    menu->active_widget = button1;

    // button 2
    i++;
    Widget* button2 = &menu->page_main[i];
    attribs.text_font = &menu->resources.font_button;
    attribs.position = {300, 400};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    attribs.caption = "CALIBRATE";
    init_button_widget (&button2->button, attribs);
    button2->bounds = button2->button.btn_rect.getGlobalBounds();
    button2->type = Widget::Button;

    // button 3
    i++;
    Widget* button3 = &menu->page_main[i];
    attribs.text_font = &menu->resources.font_button;
    attribs.position = {650, 400};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    attribs.caption = "BLAHBLAH";
    init_button_widget (&button3->button, attribs);
    button3->type = Widget::Button;
    button3->bounds = button3->button.btn_rect.getGlobalBounds();

    // navigation order
    button1->neighbours.below = button2;
    button2->neighbours.above = button1;

    button2->neighbours.right = button3;
    button3->neighbours.left = button2;
}

void init_calibrate_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_calibrate[i];
    sf::FloatRect dimensions{0, 0, 1280, 720};
    init_image_widget (bg, dimensions, &menu->resources.texture_menu_background);
    bg->bounds = dimensions;
    bg->id = i;


    // the logo
    i++;
    Widget* logo = &menu->page_calibrate[i];
    dimensions = {0, 0, 819, 144};
    init_image_widget (logo, dimensions, &menu->resources.texture_menu_logo);
    logo->bounds = dimensions;
    logo->image.img_rect.setOrigin ({logo->image.img_rect.getLocalBounds().width / 2, logo->image.img_rect.getLocalBounds().height / 2});
    logo->image.img_rect.setPosition ({bg->image.img_rect.getLocalBounds().width / 2, logo->image.img_rect.getLocalBounds().height / 2});
    logo->id = i;

    // joystics listbox heading text
    i++;
    Widget* list_title = &menu->page_calibrate[i];
    list_title->type = Widget::Label;
    list_title->label.text.setFont (menu->resources.font_button);
    list_title->label.text.setCharacterSize (32);
    list_title->label.text.setString ("GAMEPADS");
    list_title->label.text.setPosition ({50, 200});
    menu->calibrate_layout.widget_idx.listbox_title = i;

    // joysticks listbox
    const float ROW_WIDTH = 500;
    const float ROW_HEIGHT = 42;
    float row_x = 50;
    float row_y = 250;
    i++;
    menu->calibrate_layout.first_row_index = i;
    for (int row = 0; row < menu->calibrate_layout.number_rows; ++row) {
        Widget* listrow = &menu->page_calibrate[i];
        listrow->id = "gamepad_list." + std::to_string (row);
        listrow->type = Widget::ListItem;
        menu->calibrate_layout.widget_idx.listrow[i] = i;
        if (row % 2 == 0) {
            listrow->list.button.btn_rect.setFillColor (menu->theme.color_list_bg1);
            listrow->list.fill_color = menu->theme.color_list_bg1;
            listrow->list.button.btn_rect.setOutlineColor (menu->theme.color_list_bg1);
        } else {
            listrow->list.button.btn_rect.setFillColor (menu->theme.color_list_bg2);
            listrow->list.fill_color = menu->theme.color_list_bg2;
            listrow->list.button.btn_rect.setOutlineColor (menu->theme.color_list_bg2);
        }
        listrow->list.button.btn_rect.setSize ({ROW_WIDTH - 4, ROW_HEIGHT - 4});
        listrow->list.button.btn_rect.setPosition ({row_x, row_y + (ROW_HEIGHT * row) });
        listrow->list.button.btn_rect.setOutlineThickness (2);

        listrow->list.button.text.setFont (menu->resources.font_button);
        listrow->list.button.text.setPosition (listrow->list.button.btn_rect.getPosition());
        listrow->list.button.text.setFillColor (menu->theme.color_button_text);
        listrow->bounds = listrow->list.button.btn_rect.getGlobalBounds();

        if (row > 0) {
            listrow->neighbours.above = &menu->page_calibrate[i - 1];
        }
        if (row < 7) {
            listrow->neighbours.below = &menu->page_calibrate[i + 1];
        }
        i++;
    }

    // tell siblings about each other
    for (int row = menu->calibrate_layout.first_row_index; row < menu->calibrate_layout.first_row_index + menu->calibrate_layout.number_rows; ++row) {
        Widget* listrow = &menu->page_calibrate[row];

        for (int s = menu->calibrate_layout.first_row_index; s < menu->calibrate_layout.first_row_index + menu->calibrate_layout.number_rows; ++s) {
            Widget* sibling = &menu->page_calibrate[s];
            if (sibling == listrow) continue;
            listrow->list.siblings[listrow->list.number_siblings++] = sibling;
        }
    }

    // frame for the listbox (i was incremented last time through the list loop)
    Widget* frame = &menu->page_calibrate[i];
    frame->type = Widget::Frame;
    frame->frame.rect.setFillColor ({0, 0, 0, 0});
    frame->frame.rect.setOutlineColor ({255, 255, 255});
    frame->frame.rect.setOutlineThickness (2);
    frame->frame.rect.setPosition ({row_x - 2, row_y - 2});
    frame->frame.rect.setSize ({ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});
    menu->calibrate_layout.widget_idx.listbox_frame = i;

    // button test
    i++;
    Widget* button1 = &menu->page_calibrate[i];
    Button_Attributes attribs;
    button1->id = attribs.caption = "TEST";
    attribs.text_font = &menu->resources.font_button;
    attribs.dimensions = {ROW_WIDTH / 2 - 8, attribs.dimensions.y};
    attribs.position = {row_x, row_y + ROW_HEIGHT * 8 + 12};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    init_button_widget (&button1->button, attribs);
    button1->bounds = button1->button.btn_rect.getGlobalBounds();
    button1->type = Widget::Button;
    menu->active_widget = button1;
    menu->calibrate_layout.widget_idx.btn_test = i;

    // button calibrate
    i++;
    Widget* button2 = &menu->page_calibrate[i];
    attribs.caption = "CALIBRATE";
    attribs.position = {attribs.position.x + attribs.dimensions.x + 8, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (&button2->button, attribs);
    button2->bounds = button2->button.btn_rect.getGlobalBounds();
    button2->type = Widget::Button;
    menu->calibrate_layout.widget_idx.btn_calibrate = i;

    button1->neighbours.right = button2;
    button2->neighbours.left = button1;

    button1->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    button2->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7].neighbours.below = button1;

    // button  exit
    i++;
    Widget* button_exit = &menu->page_calibrate[i];
    attribs.caption = "EXIT";
    attribs.position = {row_x, row_y + ROW_HEIGHT * 8 + 12 + attribs.dimensions.y + 12};
    attribs.dimensions.x = attribs.dimensions.x * 2 + 8;
    attribs.fill_color = menu->theme.color_button_ext;
    init_button_widget (&button_exit->button, attribs);
    button_exit->bounds = button_exit->button.btn_rect.getGlobalBounds();
    button_exit->type = Widget::Button;
    menu->calibrate_layout.widget_idx.btn_exit = i;

    button1->neighbours.right = button2;
    button2->neighbours.left = button1;

    button1->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    button2->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7].neighbours.below = button1;

    button1->neighbours.below = button_exit;
    button2->neighbours.below = button_exit;
    button_exit->neighbours.above = button1;

    // other side of the page...

    // frame for the gamepad widget
    i++;
    frame = &menu->page_calibrate[i];
    frame->type = Widget::Frame;
    frame->frame.rect.setFillColor ({0, 0, 0, 0});
    frame->frame.rect.setOutlineColor ({255, 255, 255});
    frame->frame.rect.setOutlineThickness (2);
    frame->frame.rect.setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});
    frame->frame.rect.setSize ({ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});

    // gamepad widget
    i++;
    Widget* gamepad = &menu->page_calibrate[i];
    init_gamepad_widget (gamepad, menu);
    gamepad->gamepad.background.setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});

    gamepad->gamepad.left_stick_origin += gamepad->gamepad.background.getPosition();
    gamepad->gamepad.right_stick_origin += gamepad->gamepad.background.getPosition();
    gamepad->gamepad.left_stick.move (gamepad->gamepad.background.getPosition());
    gamepad->gamepad.right_stick.move (gamepad->gamepad.background.getPosition());

    menu->calibrate_layout.gamepad_widget = gamepad;

    // button done
    i++;
    Widget* button_done = &menu->page_calibrate[i];
    attribs.caption = "DONE";
    attribs.dimensions = {ROW_WIDTH - 8, attribs.dimensions.y};
    attribs.position = {row_x + ROW_WIDTH + 150, row_y + ROW_HEIGHT * 8 + 12};
    attribs.fill_color = menu->theme.color_button_std;
    init_button_widget (&button_done->button, attribs);
    button_done->bounds = button_done->button.btn_rect.getGlobalBounds();
    button_done->type = Widget::Button;
    disable_widget (button_done);
    menu->calibrate_layout.widget_idx.btn_done = i;
    button_done->id = attribs.caption = "DONE";
}

void update_active_animation (Menu* menu) {
    if (++menu->active_animation.ticks >= menu->active_animation.ticks_per_flash) {
        menu->active_animation.state = !menu->active_animation.state;
        menu->active_animation.ticks = 0;
        if (menu->active_animation.state) {
            activate_widget (menu->active_widget);
        } else {
            deactivate_widget (menu->active_widget);
        }
    }
}

Widget* get_widget_neighbour (const Widget* widget, const Menu_Event trigger) {
    Widget* neighbour = nullptr;
    switch (trigger) {
    case Menu_Event::UP:
        neighbour = widget->neighbours.above;
        break;
    case Menu_Event::DOWN:
        neighbour = widget->neighbours.below;
        break;
    case Menu_Event::LEFT:
        neighbour = widget->neighbours.left;
        break;
    case Menu_Event::RIGHT:
        neighbour = widget->neighbours.right;
        break;
    default:
        break;
    }
    return neighbour;
}

void change_active_widget (Menu* menu, const Menu_Event trigger) {
    Widget* next = menu->active_widget;

    // recurse to get a valid next widget
    do {
        next = get_widget_neighbour (next, trigger);
    } while (next != nullptr && widget_disabled (next));


    if (next && next != menu->active_widget) {
        deactivate_widget (menu->active_widget);
        activate_widget (next);
        menu->active_animation.ticks = 0;
        menu->active_widget = next;
    }
}

void set_active_widget (Widget* widget, Menu* menu) {
    if (!widget_disabled (widget)) {
        deactivate_widget (menu->active_widget);
        activate_widget (widget);
        menu->active_animation.ticks = 0;
        menu->active_widget = widget;
    }
}

void detect_and_load_gamepads (Menu* menu) {
    int row_index = menu->calibrate_layout.first_row_index;
    int max_gamepads = 8;

    bool selected_a_row = false;
    for (int i = 0; i < max_gamepads; ++i) {
        if (sf::Joystick::isConnected (i)) {
            sf::Joystick::Identification id = sf::Joystick::getIdentification (i);

            const int MAX_LENGTH  = 20;
            std::string name = id.name;
            if (name.length() > MAX_LENGTH) {
                name.resize (MAX_LENGTH);
                name += "...";
            }
            menu->page_calibrate[row_index + i].list.button.text.setString (std::to_string (i + 1) + ". " + name);
            menu->calibrate_layout.at_least_one_gamepad_found = true;
            if (!selected_a_row) {
                selected_a_row = true;
                select_widget (&menu->page_calibrate[row_index + i]);
                menu->calibrate_layout.selected_gamepad_index = i;

            }
        } else {
            menu->page_calibrate[row_index + i].list.button.text.setString ("<EMPTY>");
            disable_widget (&menu->page_calibrate[row_index + i]);
        }
        menu->page_calibrate[row_index + i].list.button.text.move (12, 0);
    }
}

int run_menu (Menu* menu, sf::RenderWindow* window) {
    Mouse             mouse;
    Keyboard          keyboard;
    GamepadController gamepad;

    int ticks = 0;
    bool mouse_mode = false;

    init_resources (menu);
    init_main_page (menu);
    init_calibrate_page (menu);

    detect_and_load_gamepads (menu);

    while (!menu->should_exit) {
        ticks++;
        // handle input
        menu->event = Menu_Event::NOTHING;
        if (window->hasFocus()) {

            handle_mouse (&mouse, menu, window);
            handle_keyboard (&keyboard, menu);
            handle_gamepad (&gamepad, menu);
            handle_window (window, menu);

            switch (menu->event) {
            case Menu_Event::UP:
                if (menu->active_widget->neighbours.above) {
                    change_active_widget (menu, menu->event);
                }
                break;
            case Menu_Event::DOWN:
                if (menu->active_widget->neighbours.below) {
                    change_active_widget (menu, menu->event);
                }
                break;
            case Menu_Event::RIGHT:
                if (menu->active_widget->neighbours.right) {
                    change_active_widget (menu, menu->event);
                }
                break;
            case Menu_Event::LEFT:
                if (menu->active_widget->neighbours.left) {
                    change_active_widget (menu, menu->event);
                }
                break;
            case Menu_Event::FIRE:
                global::log (get_widget_caption (menu->active_widget));
                select_widget (menu->active_widget);
                calibrate::handle_event (menu->active_widget->id, menu);
                if (menu->active_widget->id == "TEST") {
                    enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done]);
                    set_active_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], menu);
                } else if (menu->active_widget->id == "DONE") {
                    disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done]);
                    set_active_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], menu);
                }
                break;
            case Menu_Event::NOTHING:
            default:
                break;
            }

            // tmp special case exit on ESC key
            if (keyboard.keycode == sf::Keyboard::Key::Escape) {
                menu->should_exit = true;
                menu->return_code = 0;
            }
        }

        // update
        mouse_mode  = false;
        if (mouse.moved || mouse.pressed) {
            mouse_mode = true;
        }
        int i = 0;
        while (menu->active_page[i].type != Widget::Anonymous) {
            if (mouse_mode) {
                if (menu->active_page[i].interactive && has_mouse (&menu->active_page[i], mouse.position)
                        && menu->active_widget != &menu->active_page[i]
                   ) {
                    set_active_widget (&menu->active_page[i], menu);
                }
            }
            update_widget (&menu->active_page[i++], menu);
        }
        update_active_animation (menu);

        // render
        window->clear (sf::Color::Magenta);
        i = 0;
        while (menu->active_page[i].type != Widget::Anonymous) {
            draw_widget (&menu->active_page[i++], window);
        }
        window->display();
    }
    return menu->return_code;
}

} // namespace menu
} // namespace ss
