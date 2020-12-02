//
// here we go, a flat, performant non-oop menu system
//
#include "menu.h"
#include "widgets.h"
#include "calibration_page.h"
#include "events.h"
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
//
//
//
void handle_mouse (Mouse* mouse, Menu* menu, sf::RenderWindow* window) {

    // mouse presses
     bool was_down = mouse->states[Mouse::Down];
    mouse->states[Mouse::Pressed] = 0;
    mouse->states[Mouse::Down] = sf::Mouse::isButtonPressed (sf::Mouse::Left);
    if (mouse->states[Mouse::Down] && !was_down) {
        mouse->states[Mouse::Pressed] = true;
    } else if (!mouse->states[Mouse::Down] && was_down) {
        mouse->states[Mouse::Released]= 1;
        if (has_mouse (menu->active_widget, menu, mouse->position)) {
            menu->event = Event::Fire;
        }
    }

    // mouse movement
     sf::Vector2f last_mouse_position = mouse->position;
     sf::Vector2i m = sf::Mouse::getPosition (*window);
    mouse->position = window->mapPixelToCoords (m);

    if (mouse->position != last_mouse_position) {
        last_mouse_position = mouse->position;
        mouse->states[Mouse::Moved] = 1;
    } else {
        mouse->states[Mouse::Moved] = 0;
    }
}
//
//
//
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
//
//
//
void handle_window (sf::RenderWindow* window, Menu* menu) {
    sf::Event event;
    while (window->pollEvent (event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            menu->should_exit = true;
        }
    }
}
//
//
//
void handle_gamepad (GamepadController* gamepad, Menu* menu) {
    sf::Joystick::update();
    if (menu->calibrate_layout.selected_gamepad_index >= 0) {
        menu->controllers[menu->calibrate_layout.selected_gamepad_index].update();
    }
}
//
//
//
void init_resources (Menu* menu) {
    menu->resources.texture_menu_background = acquire_texture(menu, resources::gfx_folder() + "bg1.png");
    menu->resources.texture_menu_logo = acquire_texture(menu, resources::gfx_folder() + "logo2.png");
    menu->resources.texture_gamepad = acquire_texture(menu, resources::gfx_folder() + "gamepad_bg.png");
    menu->resources.texture_thumbstick = acquire_texture(menu, resources::gfx_folder() + "thumbstick.png");
    menu->resources.font_button = acquire_font(menu, resources::font_folder() + "arial_bold.ttf");
}
//
//
//
void init_main_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_main[i];
    sf::Vector2f dimensions{1280.f, 720.f};
    init_image_widget (bg, menu, dimensions, &menu->object_pool.textures[menu->resources.texture_menu_background]);

    // the logo
    i++;
    Widget* logo = &menu->page_main[i];
    dimensions = {819.f, 144.f};
    init_image_widget (logo, menu, dimensions, &menu->object_pool.textures[menu->resources.texture_menu_logo]);
    menu->object_pool.rects[logo->image.img_rect].setOrigin ({menu->object_pool.rects[logo->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});
    menu->object_pool.rects[logo->image.img_rect].setPosition ({menu->object_pool.rects[bg->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});

    // common button attributes
    Button_Attributes attribs;
    attribs.text_font = menu->resources.font_button;
    attribs.geometry[Button_Attributes::Dimensions] = {300, 50};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_std;
    attribs.button_colors[Button_Attributes::Shadow] = menu->theme.color_button_shadow;

    // button 1
    i++;
    Widget* button1 = &menu->page_main[i];
    attribs.caption = "BUTTON";
    attribs.id = Event::None;
    attribs.geometry[Button_Attributes::Position] = {300, 300};
    init_button_widget (button1, menu, attribs);
    menu->active_widget = button1;

    // button 2
    i++;
    Widget* button2 = &menu->page_main[i];
    attribs.caption = "SETTINGS";
    attribs.id = Event::PageCalibrate;
    attribs.geometry[Button_Attributes::Position] = {300, 400};
    init_button_widget (button2, menu, attribs);

    // button 3
    i++;
    Widget* button3 = &menu->page_main[i];
    attribs.caption = "BLAHBLAH";
    attribs.id = Event::None;
    attribs.geometry[Button_Attributes::Position] = {650, 400};
    init_button_widget (button3, menu, attribs);
    
    // exit button
    i++;
    Widget* exit_button = &menu->page_main[i];
    attribs.caption = "EXIT";
    attribs.id = Event::Exit;
    attribs.geometry[Button_Attributes::Position] = {300, 500};
    attribs.geometry[Button_Attributes::Dimensions] = {650, 50};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_ext;
    init_button_widget(exit_button, menu, attribs);

    // navigation order
    button1->neighbours.below = button2;
    button2->neighbours.above = button1;
    button2->neighbours.right = button3;
    button3->neighbours.left = button2;
    
    button2->neighbours.below = exit_button;
    button3->neighbours.below = exit_button;
    exit_button->neighbours.above = button2;
}
//
//
//
void init_calibrate_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_calibrate[i];
    sf::Vector2f dimensions{1280.f, 720.f};
    init_image_widget (bg, menu, dimensions, &menu->object_pool.textures[menu->resources.texture_menu_background]);

    // the logo
    i++;
    Widget* logo = &menu->page_calibrate[i];
    dimensions = {819.f, 144.f};
    init_image_widget (logo, menu, dimensions, &menu->object_pool.textures[menu->resources.texture_menu_logo]);
    menu->object_pool.rects[logo->image.img_rect].setOrigin ({menu->object_pool.rects[logo->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});
    menu->object_pool.rects[logo->image.img_rect].setPosition ({menu->object_pool.rects[bg->image.img_rect].getLocalBounds().width / 2, menu->object_pool.rects[bg->image.img_rect].getLocalBounds().height / 2});

    // joystics listbox heading text
    i++;
    Widget* list_title = &menu->page_calibrate[i];
    list_title->type = Widget::Label;
    list_title->label.text = acquire_label (menu, "GAMEPADS");
    menu->object_pool.labels[list_title->label.text].setFont (menu->object_pool.fonts[menu->resources.font_button]);
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

        menu->object_pool.labels[listrow->list.button.text].setFont (menu->object_pool.fonts[menu->resources.font_button]);
        menu->object_pool.labels[listrow->list.button.text].setPosition (menu->object_pool.rects[listrow->list.button.btn_rect].getPosition());
        menu->object_pool.labels[listrow->list.button.text].setFillColor (menu->theme.color_button_text);

        if (row > 0) { // not first row
            listrow->neighbours.above = &menu->page_calibrate[i - 1];
        }
        if (row < 7) { // not last row
            listrow->neighbours.below = &menu->page_calibrate[i + 1];
        }

        Event id = static_cast<Event>(static_cast<int>(Event::ListRow1) + row);
        listrow->id = id;

        init_widget (listrow);
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
    init_widget(frame);

    // commont button attributes
    Button_Attributes attribs;
    attribs.id = Event::None;
    attribs.text_font = menu->resources.font_button;
    attribs.geometry[Button_Attributes::Dimensions] = {ROW_WIDTH / 2 - 8, attribs.geometry[Button_Attributes::Dimensions].y};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_std;
    attribs.button_colors[Button_Attributes::Shadow] = menu->theme.color_button_shadow;
    // button test
    i++;
    Widget* btn_test = &menu->page_calibrate[i];
    attribs.caption = "TEST";
    btn_test->id = Event::Test;
    attribs.text_font = menu->resources.font_button;
    attribs.geometry[Button_Attributes::Position] = {row_x, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (btn_test, menu, attribs);
    menu->calibrate_layout.widget_idx.btn_test = i;

    // button calibrate
    i++;
    Widget* btn_calibrate = &menu->page_calibrate[i];
    attribs.caption = "CALIBRATE";
    attribs.id = Event::Calibrate;
    attribs.geometry[Button_Attributes::Position] = {attribs.geometry[Button_Attributes::Position].x + attribs.geometry[Button_Attributes::Dimensions].x + 8, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (btn_calibrate, menu, attribs);
    menu->calibrate_layout.widget_idx.btn_calibrate = i;

    btn_test->neighbours.right = btn_calibrate;
    btn_calibrate->neighbours.left = btn_test;

    btn_test->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    btn_calibrate->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7].neighbours.below = btn_test;


    // button  exit
    i++;
    Widget* button_exit = &menu->page_calibrate[i];
    attribs.caption = "EXIT";
    attribs.id = Event::None;
    attribs.geometry[Button_Attributes::Position] = {row_x, row_y + ROW_HEIGHT * 8 + 12 + attribs.geometry[Button_Attributes::Dimensions].y + 12};
    attribs.geometry[Button_Attributes::Dimensions].x = attribs.geometry[Button_Attributes::Dimensions].x * 2 + 8;
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_ext;
    init_button_widget (button_exit, menu, attribs);
    menu->calibrate_layout.widget_idx.btn_exit = i;

    btn_test->neighbours.right = btn_calibrate;
    btn_calibrate->neighbours.left = btn_test;

    btn_test->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    btn_calibrate->neighbours.above = &menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7];
    menu->page_calibrate[ menu->calibrate_layout.first_row_index + 7].neighbours.below = btn_test;

    btn_test->neighbours.below = button_exit;
    btn_calibrate->neighbours.below = button_exit;
    button_exit->neighbours.above = btn_test;

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
    init_widget(frame);

    // gamepad widget
    i++;
    Widget* gamepad = &menu->page_calibrate[i];
    menu->object_pool.rects[gamepad->gamepad.background].setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});
    gamepad->gamepad.left_stick_origin += menu->object_pool.rects[gamepad->gamepad.background].getPosition();
    gamepad->gamepad.right_stick_origin += menu->object_pool.rects[gamepad->gamepad.background].getPosition();
    init_gamepad_widget (gamepad, menu);
    menu->calibrate_layout.gamepad_widget = gamepad;
    set_widget_visible(gamepad, true);

    // calibrate widget
    i++;
    Widget* calibrate = &menu->page_calibrate[i];
    init_calibrate_widget(calibrate, menu);
    menu->calibrate_layout.calibrate_widget = calibrate;


    // button done
    i++;
    Widget* button_done = &menu->page_calibrate[i];
    attribs.caption = "DONE";
    attribs.geometry[Button_Attributes::Dimensions] = {ROW_WIDTH - 8, attribs.geometry[Button_Attributes::Dimensions].y};
    attribs.geometry[Button_Attributes::Position] = {row_x + ROW_WIDTH + 150, row_y + ROW_HEIGHT * 8 + 12};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_std;
    init_button_widget (button_done, menu, attribs);
    set_widget_enabled (button_done, false);
    menu->calibrate_layout.widget_idx.btn_done = i;
}
//
//
//
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
//
//
//
Widget* get_widget_neighbour (const Widget* widget, const Event trigger) {
    Widget* neighbour = nullptr;
    switch (trigger) {
    case Event::Up:
        neighbour = widget->neighbours.above;
        break;
    case Event::Down:
        neighbour = widget->neighbours.below;
        break;
    case Event::Left:
        neighbour = widget->neighbours.left;
        break;
    case Event::Right:
        neighbour = widget->neighbours.right;
        break;
    default:
        break;
    }
    return neighbour;
}
//
//
//
void next_active_widget (Menu* menu, const Event trigger) {
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
//
//
//
void set_active_widget (Widget* widget, Menu* menu) {
    if (widget_enabled (widget) && menu->active_widget != widget) {
        set_widget_active (menu->active_widget, false);
        set_widget_active (widget, true);
        menu->active_animation.ticks = 0;
        menu->active_widget = widget;
    }
}
//
//
//
void detect_and_load_gamepads (Menu* menu) {
    int row_index = menu->calibrate_layout.first_row_index;

    bool selected_a_row = false;
    for (int i = 0; i < MAX_CONTROLLERS; ++i) {
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
//
//
//
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
        menu->event = Event::None;
        mouse_mode  = false;
        if (window->hasFocus()) {

            if (mouse.states[Mouse::Moved] || mouse.states[Mouse::Pressed]) {
                mouse_mode = true;
            }
            handle_mouse (&mouse, menu, window);
            handle_keyboard (&keyboard, menu);
            handle_gamepad (&gamepad, menu);
            handle_window (window, menu);

            switch (menu->event) {
            case Event::Up:
                if (menu->active_widget->neighbours.above) {
                    next_active_widget (menu, menu->event);
                }
                break;
                
            case Event::Down:
                if (menu->active_widget->neighbours.below) {
                    next_active_widget (menu, menu->event);
                }
                break;
                
            case Event::Right:
                if (menu->active_widget->neighbours.right) {
                    next_active_widget (menu, menu->event);
                }
                break;
                
            case Event::Left:
                if (menu->active_widget->neighbours.left) {
                    next_active_widget (menu, menu->event);
                }
                break;
                
            case Event::Fire:
                global::log (get_widget_caption (menu->active_widget, menu));
                set_widget_selected (menu->active_widget, true);
                calibrate::handle_event (menu->active_widget->id, menu);

                switch (menu->active_widget->id) {
                case Event::Test:
                    set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], true);
                    set_active_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], menu);
                    break;
                    
                case Event::Done:
                    set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_done], false);
                    set_active_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], menu);
                    break;
                    
                case Event::PageCalibrate:
                    menu->active_page = menu->page_calibrate;
                    set_active_widget (&menu->active_page[12], menu);
                    break;
                    
                case Event::Exit:
                    menu->should_exit = true;
                    // test earily exit
                    return menu->return_code;
                    break;
                    
                default:
                    break;
                }

                break;
            case Event::None:
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
        while (menu->active_page[i].type != Widget::Anonymous) {                    // for all configured widgets
            if (mouse_mode) {
                if (menu->active_page[i].interactive &&                             // widget is interactive
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

