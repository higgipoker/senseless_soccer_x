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
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

namespace ss {
namespace menu {

void handle_mouse (Mouse* mouse, Menu* menu, sf::RenderWindow* window) {

    // mouse presses
    bool was_down = mouse->down;
    mouse->pressed = false;
    mouse->down = sf::Mouse::isButtonPressed (sf::Mouse::Left);
    if (mouse->down && !was_down) {
        mouse->pressed = true;
    } else if (!mouse->down && was_down) {
        mouse->released = true;
        if (has_mouse (menu->active_widget, menu, mouse->position)) {
            menu->event = Menu_Event::FIRE;
        }
    }

    // mouse movement
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
    menu->resources.texture_menu_logo.loadFromFile (resources::gfx_folder()       + "logo2.png");
    menu->resources.texture_gamepad.loadFromFile (resources::gfx_folder()         + "gamepad_bg.png");
    menu->resources.texture_thumbstick.loadFromFile (resources::gfx_folder()      + "thumbstick.png");
    menu->resources.texture_thumbstick.loadFromFile (resources::gfx_folder()      + "thumbstick.png");
    menu->resources.font_button.loadFromFile (resources::font_folder()            + "arial_bold.ttf");
}

void init_main_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_main[i];
    sf::Vector2f dimensions{1280.f, 720.f};
    init_image_widget (bg, menu, dimensions, &menu->resources.texture_menu_background);
    bg->id = i;

    // the logo
    i++;
    Widget* logo = &menu->page_main[i];
    dimensions = {819.f, 144.f};
    init_image_widget (logo, menu, dimensions, &menu->resources.texture_menu_logo);
    menu->object_pool.rects[logo->image.img_rect].setOrigin ({menu->object_pool.rects[logo->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});
    menu->object_pool.rects[logo->image.img_rect].setPosition ({menu->object_pool.rects[bg->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});
    logo->id = i;

    // button 1
    i++;
    Widget* button1 = &menu->page_main[i];
    Button_Attributes attribs;
    attribs.text_font = &menu->resources.font_button;
    attribs.geometry[idx_position] = {300, 300};
    attribs.geometry[idx_dimensions] = {300, 50};
    attribs.button_colors[idx_fill] = menu->theme.color_button_std;
    attribs.button_colors[idx_shadow] = menu->theme.color_button_shadow;
    init_button_widget (button1, menu, attribs);
    button1->type = Widget::Button;
    menu->active_widget = button1;

    // button 2
    i++;
    Widget* button2 = &menu->page_main[i];
    attribs.text_font = &menu->resources.font_button;
    attribs.geometry[idx_position] = {300, 400};
    attribs.geometry[idx_dimensions] = {300, 50};
    attribs.button_colors[idx_fill] = menu->theme.color_button_std;
    attribs.button_colors[idx_shadow] = menu->theme.color_button_shadow;
    attribs.caption = "CALIBRATE";
    init_button_widget (button2, menu, attribs);
    button2->type = Widget::Button;

    // button 3
    i++;
    Widget* button3 = &menu->page_main[i];
    attribs.text_font = &menu->resources.font_button;
    attribs.geometry[idx_position] = {650, 400};
    attribs.geometry[idx_dimensions] = {300, 50};
    attribs.button_colors[idx_fill] = menu->theme.color_button_std;
    attribs.button_colors[idx_shadow] = menu->theme.color_button_shadow;
    attribs.caption = "BLAHBLAH";
    init_button_widget (button3, menu, attribs);
    button3->type = Widget::Button;

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
    sf::Vector2f dimensions{1280.f, 720.f};
    init_image_widget (bg, menu, dimensions, &menu->resources.texture_menu_background);
    bg->id = i;

    // the logo
    i++;
    Widget* logo = &menu->page_calibrate[i];
    dimensions = {819.f, 144.f};
    init_image_widget (logo, menu, dimensions, &menu->resources.texture_menu_logo);
    menu->object_pool.rects[logo->image.img_rect].setOrigin ({menu->object_pool.rects[logo->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});
    menu->object_pool.rects[logo->image.img_rect].setPosition ({menu->object_pool.rects[bg->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});
    logo->id = i;

    // joystics listbox heading text
    i++;
    Widget* list_title = &menu->page_calibrate[i];
    list_title->type = Widget::Label;
    list_title->label.text = acquire_label (menu, "GAMEPADS");
    menu->object_pool.labels[list_title->label.text].setFont (menu->resources.font_button);
    menu->object_pool.labels[list_title->label.text].setCharacterSize (32);
    menu->object_pool.labels[list_title->label.text].setPosition ({50, 200});
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
        listrow->type = Widget::ListItem;
        menu->calibrate_layout.widget_idx.listrow[row] = i;
        listrow->list.button.btn_rect = acquire_rect (menu);
        listrow->list.button.shadow_rect = acquire_rect (menu);
        listrow->list.button.text = acquire_label (menu);
        listrow->list.button.text_shadow = acquire_label (menu);
        listrow->list.fill_color = acquire_color (menu);
        if (row % 2 == 0) {
            menu->object_pool.rects[listrow->list.button.btn_rect].setFillColor (menu->theme.color_list_bg1);
            menu->object_pool.colors[listrow->list.fill_color] = menu->theme.color_list_bg1;
            menu->object_pool.rects[listrow->list.button.btn_rect].setOutlineColor (menu->theme.color_list_bg1);
        } else {
            menu->object_pool.rects[listrow->list.button.btn_rect].setFillColor (menu->theme.color_list_bg2);
            menu->object_pool.colors[listrow->list.fill_color] = menu->theme.color_list_bg2;
            menu->object_pool.rects[listrow->list.button.btn_rect].setOutlineColor (menu->theme.color_list_bg2);
        }
        menu->object_pool.rects[listrow->list.button.btn_rect].setSize ({ROW_WIDTH - 4, ROW_HEIGHT - 4});
        menu->object_pool.rects[listrow->list.button.btn_rect].setPosition ({row_x, row_y + (ROW_HEIGHT * row) });
        menu->object_pool.rects[listrow->list.button.btn_rect].setOutlineThickness (2);

        menu->object_pool.labels[listrow->list.button.text].setFont (menu->resources.font_button);
        menu->object_pool.labels[listrow->list.button.text].setPosition (menu->object_pool.rects[listrow->list.button.btn_rect].getPosition());
        menu->object_pool.labels[listrow->list.button.text].setFillColor (menu->theme.color_button_text);

        if (row > 0) { // not first row
            listrow->neighbours.above = &menu->page_calibrate[i - 1];
        }
        if (row < 7) { // not last row
            listrow->neighbours.below = &menu->page_calibrate[i + 1];
        }
        init_widget (listrow, "gamepad_list." + std::to_string (row));
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
    frame->frame.rect = acquire_rect (menu, {ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});
    menu->object_pool.rects[frame->frame.rect].setFillColor ({0, 0, 0, 0});
    menu->object_pool.rects[frame->frame.rect].setOutlineColor ({255, 255, 255});
    menu->object_pool.rects[frame->frame.rect].setOutlineThickness (2);
    menu->object_pool.rects[frame->frame.rect].setPosition ({row_x - 2, row_y - 2});
    menu->calibrate_layout.widget_idx.listbox_frame = i;
    init_widget(frame, "list_frame");

    // button test
    i++;
    Widget* button1 = &menu->page_calibrate[i];
    Button_Attributes attribs;
    button1->id = attribs.caption = "TEST";
    attribs.text_font = &menu->resources.font_button;
    attribs.geometry[idx_dimensions] = {ROW_WIDTH / 2 - 8, attribs.geometry[idx_dimensions].y};
    attribs.geometry[idx_position] = {row_x, row_y + ROW_HEIGHT * 8 + 12};
    attribs.button_colors[idx_fill] = menu->theme.color_button_std;
    attribs.button_colors[idx_shadow] = menu->theme.color_button_shadow;
    init_button_widget (button1, menu, attribs);
    button1->type = Widget::Button;
    menu->active_widget = button1;
    menu->calibrate_layout.widget_idx.btn_test = i;

    // button calibrate
    i++;
    Widget* button2 = &menu->page_calibrate[i];
    attribs.caption = "CALIBRATE";
    attribs.geometry[idx_position] = {attribs.geometry[idx_position].x + attribs.geometry[idx_dimensions].x + 8, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (button2, menu, attribs);
    button2->type = Widget::Button;
    init_widget(button2, "START_CALIBRATE");
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
    attribs.geometry[idx_position] = {row_x, row_y + ROW_HEIGHT * 8 + 12 + attribs.geometry[idx_dimensions].y + 12};
    attribs.geometry[idx_dimensions].x = attribs.geometry[idx_dimensions].x * 2 + 8;
    attribs.button_colors[idx_fill] = menu->theme.color_button_ext;
    init_button_widget (button_exit, menu, attribs);
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
    frame->frame.rect = acquire_rect (menu, {ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});
    menu->object_pool.rects[frame->frame.rect].setFillColor ({0, 0, 0, 0});
    menu->object_pool.rects[frame->frame.rect].setOutlineColor ({255, 255, 255});
    menu->object_pool.rects[frame->frame.rect].setOutlineThickness (2);
    menu->object_pool.rects[frame->frame.rect].setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});
    init_widget(frame, "list_frame");

    // gamepad widget
    i++;
    Widget* gamepad = &menu->page_calibrate[i];
    gamepad->gamepad.background.setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});
    gamepad->gamepad.left_stick_origin += gamepad->gamepad.background.getPosition();
    gamepad->gamepad.right_stick_origin += gamepad->gamepad.background.getPosition();
    init_gamepad_widget (gamepad, menu);    
    menu->calibrate_layout.gamepad_widget = gamepad;
    set_widget_visible(gamepad, false);
    
    // calibrate widget
    i++;
    Widget* calibrate = &menu->page_calibrate[i];
    init_calibrate_widget(calibrate, menu);
    menu->calibrate_layout.calibrate_widget = calibrate;
    

    // button done
    i++;
    Widget* button_done = &menu->page_calibrate[i];
    attribs.caption = "DONE";
    attribs.geometry[idx_dimensions] = {ROW_WIDTH - 8, attribs.geometry[idx_dimensions].y};
    attribs.geometry[idx_position] = {row_x + ROW_WIDTH + 150, row_y + ROW_HEIGHT * 8 + 12};
    attribs.button_colors[idx_fill] = menu->theme.color_button_std;
    init_button_widget (button_done, menu, attribs);
    button_done->type = Widget::Button;
    set_widget_enabled (button_done, false);
    menu->calibrate_layout.widget_idx.btn_done = i;
    button_done->id = attribs.caption = "DONE";

}

void update_active_animation (Menu* menu) {
    if (++menu->active_animation.ticks >= menu->active_animation.ticks_per_flash) {
        menu->active_animation.state = !menu->active_animation.state;
        menu->active_animation.ticks = 0;
        if (menu->active_animation.state) {
            set_widget_active (menu->active_widget, true);
        } else {
            set_widget_active (menu->active_widget, false);
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
    } while (next != nullptr && !widget_enabled (next));


    if (next && next != menu->active_widget) {
        set_widget_active (menu->active_widget, false);
        set_widget_active (next, true);
        menu->active_animation.ticks = 0;
        menu->active_widget = next;
    }
}

void set_active_widget (Widget* widget, Menu* menu) {
    if (widget_enabled (widget) && menu->active_widget != widget) {
        std::cout << widget->id << std::endl;
        set_widget_active (menu->active_widget, false);
        set_widget_active (widget, true);
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
            menu->calibrate_layout.active_rows[i] = 1;
            sf::Joystick::Identification id = sf::Joystick::getIdentification (i);

            const int MAX_LENGTH  = 20;
            std::string name = id.name;
            if (name.length() > MAX_LENGTH) {
                name.resize (MAX_LENGTH);
                name += "...";
            }
            menu->object_pool.labels[menu->page_calibrate[row_index + i].list.button.text].setString (std::to_string (i + 1) + ". " + name);
            menu->calibrate_layout.at_least_one_gamepad_found = true;
            if (!selected_a_row) {
                selected_a_row = true;
                set_widget_selected (&menu->page_calibrate[row_index + i], true);
                menu->calibrate_layout.selected_gamepad_index = i;

            }
        } else {
            menu->object_pool.labels[menu->page_calibrate[row_index + i].list.button.text].setString ("<EMPTY>");
            set_widget_enabled (&menu->page_calibrate[row_index + i], false);
        }
        menu->object_pool.labels[menu->page_calibrate[row_index + i].list.button.text].move (12, 0);
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
        mouse_mode  = false;
        if (window->hasFocus()) {

            if (mouse.moved || mouse.pressed) {
                mouse_mode = true;
            }
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
                global::log (get_widget_caption (menu->active_widget, menu));
                set_widget_selected (menu->active_widget, true);
                calibrate::handle_event (menu->active_widget->id, menu);

                if (menu->active_widget->id == "TEST") {
                    set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], true);
                    set_active_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], menu);

                } else if (menu->active_widget->id == "DONE") {
                    set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], false);
                    set_active_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], menu);

                } else if (menu->active_widget->id == "CALIBRATE") {
                    menu->active_page = menu->page_calibrate;
                    set_active_widget (&menu->active_page[12], menu);

                } else if (menu->active_widget->id == "EXIT") {
                    menu->active_page = menu->page_main;
                    set_active_widget (&menu->active_page[3], menu);
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
        int i = 0;
        while (menu->active_page[i].type != Widget::Anonymous) {                // for all configured widgets
            if (mouse_mode) {
                if (menu->active_page[i].interactive &&                         // widget is interactive
                        has_mouse (&menu->active_page[i], menu, mouse.position) &&  // widget has the mouse in its bounds
                        menu->active_widget != &menu->active_page[i]) {             // widget not already the active widget
                    set_active_widget (&menu->active_page[i], menu);
                }
            }
            update_widget (&menu->active_page[i++], menu);
        }
        update_active_animation (menu);

        // render
        window->clear (sf::Color::Magenta);
        i = 0;
        while (menu->active_page[i].type != Widget::Anonymous) {    // for all configured widgets
            draw_widget (&menu->active_page[i++], menu, window);
        }
        window->display();
    }
    return menu->return_code;
}

} // namespace menu
} // namespace ss
