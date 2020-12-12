//
// here we go, a flat, performant non-oop menu system
//
#include "menu.h"
#include "../global.hpp"
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

    // buttons
    bool was_down = mouse->states[Mouse::Down];
    mouse->states[Mouse::Pressed] = 0;
    mouse->states[Mouse::Down] = sf::Mouse::isButtonPressed (sf::Mouse::Left);

    if (mouse->states[Mouse::Down] && !was_down) {
        mouse->states[Mouse::Pressed] = true;
        
    } else if (!mouse->states[Mouse::Down] && was_down) {
        mouse->states[Mouse::Released] = 1;
        if (has_mouse (menu->active_widget, menu, mouse->position)) {
            menu->event = Event::Fire;
        }
    }

    // movement
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
    // waiting for repeat timeout while key held down
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
void handle_gamepad (Menu* menu) {

    for (int i = 0; i < MAX_CONTROLLERS; ++i) {
        menu->controllers[i].update();
        if (menu->controllers[i].anyInput() && i != menu->active_controller) {
            menu->active_controller = i;
            std::cout << "handover to controller "  << i << std::endl;
            menu->gamepad_state.wait_for_repeat = true;
        }
    }
    if (menu->gamepad_state.wait_for_repeat) {
        if (--menu->gamepad_state.repeat_ticks <= 0) {
            menu->gamepad_state.repeat_ticks = menu->gamepad_state.repeat_time;
            menu->gamepad_state.wait_for_repeat = false;
        }
    }
    if (menu->controllers[menu->active_controller].state.last_event == ButtonEvent::Pressed) {
        menu->event = Event::Fire;
    }
    if (menu->gamepad_state.suspended) return;
    
    if (!menu->gamepad_state.wait_for_repeat) {
        
        if (menu->controllers[menu->active_controller].up()) {
            menu->event = Event::Up;
            menu->gamepad_state.wait_for_repeat = true;
            
        } else if (menu->controllers[menu->active_controller].down()) {
            menu->event = Event::Down;
            menu->gamepad_state.wait_for_repeat = true;
            
        } else if (menu->controllers[menu->active_controller].left()) {
            menu->event = Event::Left;
            menu->gamepad_state.wait_for_repeat = true;
            
        } else if (menu->controllers[menu->active_controller].right()) {
            menu->event = Event::Right;
            menu->gamepad_state.wait_for_repeat = true;
        }
    }
}
//
//
//
void init_resources (Menu* menu) {
    menu->resources.texture_menu_background = acquire_texture   (menu, resources::gfx_folder()  + "bg1.png");
    menu->resources.texture_menu_logo       = acquire_texture   (menu, resources::gfx_folder()  + "logo2.png");
    menu->resources.texture_gamepad         = acquire_texture   (menu, resources::gfx_folder()  + "gamepad_bg.png");
    menu->resources.texture_thumbstick      = acquire_texture   (menu, resources::gfx_folder()  + "thumbstick.png");
    menu->resources.font_button             = acquire_font      (menu, resources::font_folder() + "arial_bold.ttf");
}
//
//
//
void init_main_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_main[i];
    init_widget (bg, menu, Widget::Image);
    init_image_widget (bg, menu, {1280, 720}, &texture (menu, menu->resources.texture_menu_background));

    // the logo
    i++;
    Widget* logo = &menu->page_main[i];
    init_widget (logo, menu, Widget::Image);
    init_image_widget (logo, menu, {819, 144}, &texture (menu, menu->resources.texture_menu_logo));
    rect (menu, logo->image.img_rect).setOrigin ({rect (menu, logo->image.img_rect).getLocalBounds().width / 2, rect (menu, bg->image.img_rect).getLocalBounds().height / 2});
    rect (menu, logo->image.img_rect).setPosition ({rect (menu, bg->image.img_rect).getLocalBounds().width / 2, rect (menu, bg->image.img_rect).getLocalBounds().height / 2});

    // common button attributes
    Button_Attributes attribs;
    attribs.text_font = menu->resources.font_button;
    attribs.geometry[Button_Attributes::Dimensions] = menu->theme.dimensions_btn_small;
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_std;
    attribs.button_colors[Button_Attributes::Shadow] = menu->theme.color_button_shadow;
    attribs.caption_colors[Button_Attributes::Fill] = menu->theme.color_button_text;

    // button play 
    i++;
    Widget* button1 = &menu->page_main[i];
    init_widget (button1, menu, Widget::Button);
    attribs.caption = "PLAY";
    attribs.id = Event::None;
    attribs.geometry[Button_Attributes::Position] = {300, 300};
    init_button_widget (button1, menu, attribs);
    menu->active_widget = button1;

    // button settings
    i++;
    Widget* button2 = &menu->page_main[i];
    init_widget (button2, menu, Widget::Button);
    attribs.caption = "SETTINGS";
    attribs.id = Event::PageSettings;
    attribs.geometry[Button_Attributes::Position] = {300, 400};
    init_button_widget (button2, menu, attribs);

    // button test
    i++;
    Widget* button3 = &menu->page_main[i];
    init_widget (button3, menu, Widget::Button);
    attribs.caption = "BLAHBLAH";
    attribs.id = Event::None;
    attribs.geometry[Button_Attributes::Position] = {650, 400};
    init_button_widget (button3, menu, attribs);

    // button exit
    i++;
    Widget* exit_button = &menu->page_main[i];
    init_widget (exit_button, menu, Widget::Button);
    attribs.caption = "EXIT";
    attribs.id = Event::Exit;
    attribs.geometry[Button_Attributes::Position] = {300, 500};
    attribs.geometry[Button_Attributes::Dimensions] = {650, 50};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_ext;
    init_button_widget (exit_button, menu, attribs);

    // navigation order
    button1->neighbours.below       = button2;
    button2->neighbours.above       = button1;
    button2->neighbours.right       = button3;
    button3->neighbours.left        = button2;
    button2->neighbours.below       = exit_button;
    button3->neighbours.below       = exit_button;
    exit_button->neighbours.above   = button2;
}
//
//
//
void init_settings_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_settings[i];
    init_widget (bg, menu, Widget::Image);
    init_image_widget (bg, menu, {1280, 720}, &texture (menu, menu->resources.texture_menu_background));

    // the logo
    i++;
    Widget* logo = &menu->page_settings[i];
    init_widget (logo, menu, Widget::Image);
    init_image_widget (logo, menu, {819, 144}, &texture (menu, menu->resources.texture_menu_logo));
    rect (menu, logo->image.img_rect).setOrigin ({rect (menu, logo->image.img_rect).getLocalBounds().width / 2, rect (menu, bg->image.img_rect).getLocalBounds().height / 2});
    rect (menu, logo->image.img_rect).setPosition ({rect (menu, bg->image.img_rect).getLocalBounds().width / 2, rect (menu, bg->image.img_rect).getLocalBounds().height / 2});

    // joystics listbox heading text
    i++;
    Widget* list_title = &menu->page_settings[i];
    init_widget (list_title, menu, Widget::Label);
    set_widget_interactive (list_title, false);
    list_title->label.text = acquire_label (menu, "GAMEPADS");
    label (menu, list_title->label.text).setFont (font (menu, menu->resources.font_button));
    label (menu, list_title->label.text).setCharacterSize (32);
    label (menu, list_title->label.text).setPosition ({50, 200});
    menu->settings_layout.widget_idx.listbox_title = i;

    // joysticks listbox
    const float ROW_WIDTH = 500;
    const float ROW_HEIGHT = 42;
    float row_x = 50;
    float row_y = 250;
    i++;
    menu->settings_layout.first_row_index = i;
    for (int row = 0; row < menu->settings_layout.number_rows; ++row) {
        Widget* listrow = &menu->page_settings[i];
        init_widget (listrow, menu, Widget::ListItem);
        set_widget_interactive (listrow, true);
        listrow->list.index = row;
        menu->settings_layout.widget_idx.listrow[row] = i;
        listrow->list.button.btn_rect = acquire_rect (menu);
        listrow->list.button.shadow_rect = acquire_rect (menu);
        listrow->list.button.text = acquire_label (menu);
        listrow->list.button.text_shadow = acquire_label (menu);
        listrow->list.fill_color = acquire_color (menu);
        if (row % 2 == 0) {
            rect (menu, listrow->list.button.btn_rect).setFillColor (menu->theme.color_list_bg1);
            color (menu, listrow->list.fill_color) = menu->theme.color_list_bg1;
            rect (menu, listrow->list.button.btn_rect).setOutlineColor (menu->theme.color_list_bg1);
        } else {
            rect (menu, listrow->list.button.btn_rect).setFillColor (menu->theme.color_list_bg2);
            color (menu, listrow->list.fill_color) = menu->theme.color_list_bg2;
            rect (menu, listrow->list.button.btn_rect).setOutlineColor (menu->theme.color_list_bg2);
        }
        rect (menu, listrow->list.button.btn_rect).setSize ({ROW_WIDTH - 4, ROW_HEIGHT - 4});
        rect (menu, listrow->list.button.btn_rect).setPosition ({row_x, row_y + (ROW_HEIGHT * row) });
        rect (menu, listrow->list.button.btn_rect).setOutlineThickness (2);

        label (menu, listrow->list.button.text).setFont (font (menu, menu->resources.font_button));
        label (menu, listrow->list.button.text).setPosition (rect (menu, listrow->list.button.btn_rect).getPosition());
        label (menu, listrow->list.button.text).setFillColor (menu->theme.color_button_text);

        if (row > 0) { // not first row
            listrow->neighbours.above = &menu->page_settings[i - 1];
        }
        if (row < 7) { // not last row
            listrow->neighbours.below = &menu->page_settings[i + 1];
        }

        //Event id = static_cast<Event> (static_cast<int> (Event::ListRow1) + row);
        // listrow->id = id;
        listrow->id = Event::Fire;
        i++;
    }

    // tell siblings about each other
    for (int row = menu->settings_layout.first_row_index; row < menu->settings_layout.first_row_index + menu->settings_layout.number_rows; ++row) {
        Widget* listrow = &menu->page_settings[row];

        for (int s = menu->settings_layout.first_row_index; s < menu->settings_layout.first_row_index + menu->settings_layout.number_rows; ++s) {
            Widget* sibling = &menu->page_settings[s];
            if (sibling == listrow) continue;
            listrow->list.siblings[listrow->list.number_siblings++] = sibling;
        }
    }

    // frame for the listbox (i was incremented last time through the list loop)
    Widget* frame = &menu->page_settings[i];
    init_widget (frame, menu, Widget::Frame);
    set_widget_interactive (frame, false);
    frame->frame.rect = acquire_rect (menu, {ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});
    rect (menu, frame->frame.rect).setFillColor ({0, 0, 0, 0});
    rect (menu, frame->frame.rect).setOutlineColor ({255, 255, 255});
    rect (menu, frame->frame.rect).setOutlineThickness (2);
    rect (menu, frame->frame.rect).setPosition ({row_x - 2, row_y - 2});
    menu->settings_layout.widget_idx.listbox_frame = i;

    // commont button attributes
    Button_Attributes attribs;
    attribs.id = Event::None;
    attribs.text_font = menu->resources.font_button;
    attribs.geometry[Button_Attributes::Dimensions] = {ROW_WIDTH / 2 - 8, menu->theme.dimensions_btn_small.y};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_std;
    attribs.button_colors[Button_Attributes::Shadow] = menu->theme.color_button_shadow;
    attribs.caption_colors[Button_Attributes::Fill] = menu->theme.color_button_text;
    // button test
    i++;
    Widget* btn_test = &menu->page_settings[i];
    init_widget (btn_test, menu, Widget::Button);
    attribs.caption = "TEST";
    attribs.id = Event::Test;
    attribs.text_font = menu->resources.font_button;
    attribs.geometry[Button_Attributes::Position] = {row_x, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (btn_test, menu, attribs);
    menu->settings_layout.widget_idx.btn_test = i;

    // button calibrate
    i++;
    Widget* btn_calibrate = &menu->page_settings[i];
    init_widget (btn_calibrate, menu, Widget::Button);
    attribs.caption = "CALIBRATE";
    attribs.id = Event::Calibrate;
    attribs.geometry[Button_Attributes::Position] = {attribs.geometry[Button_Attributes::Position].x + attribs.geometry[Button_Attributes::Dimensions].x + 8, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (btn_calibrate, menu, attribs);
    menu->settings_layout.widget_idx.btn_calibrate = i;

    btn_test->neighbours.right = btn_calibrate;
    btn_calibrate->neighbours.left = btn_test;

    btn_test->neighbours.above = &menu->page_settings[ menu->settings_layout.first_row_index + 7];
    btn_calibrate->neighbours.above = &menu->page_settings[ menu->settings_layout.first_row_index + 7];
    menu->page_settings[ menu->settings_layout.first_row_index + 7].neighbours.below = btn_test;


    // button  exit
    i++;
    Widget* button_exit = &menu->page_settings[i];
    init_widget (button_exit, menu, Widget::Button);
    attribs.caption = "EXIT";
    attribs.id = Event::Exit;
    attribs.geometry[Button_Attributes::Position] = {row_x, row_y + ROW_HEIGHT * 8 + 12 + attribs.geometry[Button_Attributes::Dimensions].y + 12};
    attribs.geometry[Button_Attributes::Dimensions].x = attribs.geometry[Button_Attributes::Dimensions].x * 2 + 8;
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_ext;
    init_button_widget (button_exit, menu, attribs);
    menu->settings_layout.widget_idx.btn_exit = i;

    btn_test->neighbours.right = btn_calibrate;
    btn_calibrate->neighbours.left = btn_test;

    btn_test->neighbours.above = &menu->page_settings[ menu->settings_layout.first_row_index + 7];
    btn_calibrate->neighbours.above = &menu->page_settings[ menu->settings_layout.first_row_index + 7];
    menu->page_settings[ menu->settings_layout.first_row_index + 7].neighbours.below = btn_test;

    btn_test->neighbours.below = button_exit;
    btn_calibrate->neighbours.below = button_exit;
    button_exit->neighbours.above = btn_test;

    // other side of the page...

    // frame for the right hand side
    i++;
    frame = &menu->page_settings[i];
    init_widget (frame, menu, Widget::Frame);
    set_widget_interactive (frame, false);
    frame->frame.rect = acquire_rect (menu, {ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});
    rect (menu, frame->frame.rect).setFillColor ({0, 0, 0, 0});
    rect (menu, frame->frame.rect).setOutlineColor ({255, 255, 255});
    rect (menu, frame->frame.rect).setOutlineThickness (2);
    rect (menu, frame->frame.rect).setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});

    // gamepad widget
    i++;
    Widget* gamepad = &menu->page_settings[i];
    init_widget (gamepad, menu, Widget::Gamepad);
    init_gamepad_widget (gamepad, menu);
    rect (menu, gamepad->gamepad.background).setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});
    // left thumbstick
    i++;
    Widget* left_stick = &menu->page_settings[i];
    init_widget (left_stick, menu, Widget::Test);
    init_thumbstick_test_widget (left_stick, menu);
    set_widget_position (left_stick, menu, {820, 350});
    gamepad->gamepad.left_stick = i;
    // right thumbstick
    i++;
    Widget* right_stick = &menu->page_settings[i];
    init_widget (right_stick, menu, Widget::Test);
    init_thumbstick_test_widget (right_stick, menu);
    set_widget_position (right_stick, menu, {1030, 440});
    gamepad->gamepad.right_stick = i;

    menu->settings_layout.gamepad_widget = gamepad;

    // text for calibration
    i++;
    Widget* instruction_text = &menu->page_settings[i];
    init_widget (instruction_text, menu, Widget::Label);
    set_widget_interactive (instruction_text, false);
    instruction_text->label.text = acquire_label (menu, "STEP 1 OF 2:\nLEAVE THE STICKS CENTERED \nAND PRESS A BUTTON");
    label (menu, instruction_text->label.text).setFont (font (menu, menu->resources.font_button));
    label (menu, instruction_text->label.text).setCharacterSize (18);
    label (menu, instruction_text->label.text).setPosition ({710, 260});
    menu->settings_layout.cali_instructions = instruction_text;
    set_widget_visible (instruction_text, false);

    // left thumbstick diagnostic widget
    i++;
    Widget* leftstick_diag = &menu->page_settings[i];
    init_widget (leftstick_diag, menu, Widget::Calibrate);
    init_thumbstick_calibrate_widget (leftstick_diag, menu);
    menu->settings_layout.leftstick_diag = leftstick_diag;
    set_widget_visible (leftstick_diag, false);
    transform (menu, leftstick_diag->transform).translate ({842.f, 480.f});

    // right thumbstick diagnostic widget
    i++;
    Widget* rightstick_diag = &menu->page_settings[i];
    init_widget (rightstick_diag, menu, Widget::Calibrate);
    init_thumbstick_calibrate_widget (rightstick_diag, menu);
    menu->settings_layout.rightstick_diag = rightstick_diag;
    set_widget_visible (rightstick_diag, false);
    transform (menu, rightstick_diag->transform).translate ({1042.f, 480.f});


    // button done
    i++;
    Widget* button_done = &menu->page_settings[i];
    init_widget (button_done, menu, Widget::Button);
    attribs.caption = "DONE";
    attribs.id = Event::Done;
    attribs.geometry[Button_Attributes::Dimensions] = {ROW_WIDTH - 8, attribs.geometry[Button_Attributes::Dimensions].y};
    attribs.geometry[Button_Attributes::Position] = {row_x + ROW_WIDTH + 150, row_y + ROW_HEIGHT * 8 + 12};
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_std;
    init_button_widget (button_done, menu, attribs);
    set_widget_enabled (button_done, false);
    menu->settings_layout.widget_idx.btn_done = i;

    // button cancel
    i++;
    Widget* button_cancel = &menu->page_settings[i];
    init_widget (button_cancel, menu, Widget::Button);
    attribs.caption = "CANCEL";
    attribs.id = Event::Cancel;
    attribs.geometry[Button_Attributes::Position].y += 60;
    attribs.button_colors[Button_Attributes::Fill] = menu->theme.color_button_ext;
    init_button_widget (button_cancel, menu, attribs);
    set_widget_enabled (button_cancel, false);
    menu->settings_layout.widget_idx.btn_cancel = i;

    // buton tab order
    button_done->neighbours.below = button_cancel;
    button_cancel->neighbours.above = button_done;
}
//
//
//
void init_controllers (Menu* menu) {
    files::File file;
    files::open (file, files::working_dir() + "/data/calibration.cfg");
    std::map<int,  Calibration> calibrations;
    load_from_file (calibrations, file);
    files::close (file);

    for (int i = 0; i < MAX_CONTROLLERS; ++i) {
        menu->controllers[i].setId (i);

        // check for a calibration profile
        sf::Joystick::Identification id = sf::Joystick::getIdentification (i);

        for (auto& cali : calibrations) {
            if (cali.second.name == id.name && cali.second.vendor_id == id.vendorId && cali.second.product_id == id.productId) {
                std::cout << "found calibration for " << id.name.toAnsiString() << std::endl;
                menu->controllers[i].calibrate (&cali.second);
            }
        }
    }
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
//
//
//
void detect_and_load_gamepads (Menu* menu) {
    int row_index = menu->settings_layout.first_row_index;
    bool selected_a_row = false;
    for (int i = 0; i < MAX_CONTROLLERS; ++i) {
        if (sf::Joystick::isConnected (i)) {
            menu->settings_layout.active_rows[i] = 1;
            sf::Joystick::Identification id = sf::Joystick::getIdentification (i);

            std::cout << std::endl;
            std::cout << "name: "       << id.name.toAnsiString()   << std::endl;
            std::cout << "product id: " << id.productId             << std::endl;
            std::cout << "vendor id: "  << id.vendorId              << std::endl;
            std::cout << std::endl;

            const int MAX_LENGTH  = 20;
            std::string name = id.name;
            if (name.length() > MAX_LENGTH) {
                name.resize (MAX_LENGTH);
                name += "...";
            }
            label (menu, menu->page_settings[row_index + i].list.button.text).setString (std::to_string (i + 1) + ". " + name);
            menu->settings_layout.at_least_one_gamepad_found = true;
            if (!selected_a_row) {
                selected_a_row = true;
                set_widget_selected (&menu->page_settings[row_index + i], true);
                menu->settings_layout.selected_gamepad_index = i;

            }
        } else {
            label (menu, menu->page_settings[row_index + i].list.button.text).setString ("<EMPTY>");
            set_widget_enabled (&menu->page_settings[row_index + i], false);
        }
        label (menu, menu->page_settings[row_index + i].list.button.text).move (12, 0);
    }
}
//
//
//
void handle_event (Menu* menu, const Event trigger) {
    switch (menu->state) {
    default:
        break;

    case Menu_State::State_MainPage:
        switch (trigger) {

        default:
            break;
            
        case Event::PageSettings:
            menu->state = Menu_State::State_SettingsPage;
            menu->active_page = menu->page_settings;
            set_active_widget (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], menu);
            break;
        }
        break;

    case Menu_State::State_SettingsPage:
        switch (trigger) {
       
        default :
            break;
            
        case Event::Fire:
            if (menu->active_widget->type == Widget::ListItem) {
                menu->settings_layout.selected_gamepad_index = menu->active_widget->list.index;
            }
            break;
            
        case Event::Test:
            settings::show_test (menu);
            break;
            
        case Event::Calibrate:
            settings::show_calibrate_phase1 (menu);
            break;
            
        case Event::Exit:
            menu->state = Menu_State::State_MainPage;
            menu->active_page = menu->page_main;
            set_active_widget (&menu->page_main[2], menu);
            break;
        }
        break;

    case Menu_State::State_CalibratePhase1:
        switch (trigger) {
            
        default:
            break;
            
        case Event::Done:
            settings::show_calibrate_phase2 (menu);
            break;
            
        case Event::Cancel:
            settings::cancel_calibrate (menu);
            break;

        }
        break;

    case Menu_State::State_CalibratePhase2:
        switch (trigger) {
            
        default:
            break;
            
        case Event::Done:
            settings::calibrate_finished (menu);
            break;
            
        case Event::Cancel:
            settings::cancel_calibrate (menu);
            break;
        }
        break;
        
        case Menu_State::State_Test:
           switch(trigger){
               
               default:break;
               
               case Event::Done:
                   settings::test_finished(menu);
                   break;
                   
           }
           break;
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
    init_settings_page (menu);
    init_controllers (menu);

    detect_and_load_gamepads (menu);
    
    menu->gamepad_state.wait_for_repeat = true;

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
            handle_gamepad (menu);
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
                set_widget_selected (menu->active_widget, true);
                handle_event (menu, menu->active_widget->id);

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
                if (widget_interactive (&menu->active_page[i]) &&                   // widget is interactive
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
