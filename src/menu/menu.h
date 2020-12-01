#pragma once
#include "widgets.h"
#include "calibration_page.h"
#include "../input/Controller.hpp"
#include "../input/GamepadController.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>
#include <map>

namespace ss {
namespace menu {
static const int MAX_WIDGETS_PER_PAGE = 200;
static const int MAX_CONTROLLERS = 8;

enum Menu_Event {NOTHING, UP, DOWN, LEFT, RIGHT, FIRE};
// TODO keyboard/ input profiles
static std::map<sf::Keyboard::Key, Menu_Event> keymap = {
    {sf::Keyboard::W,     Menu_Event::UP},
    {sf::Keyboard::S,     Menu_Event::DOWN},
    {sf::Keyboard::A,     Menu_Event::LEFT},
    {sf::Keyboard::D,     Menu_Event::RIGHT},
    {sf::Keyboard::Enter, Menu_Event::FIRE},
};

struct Mouse {
    sf::Vector2f position    {0, 0};
    bool down                {false};
    bool pressed             {false};
    bool released            {false};
    bool moved               {false};
};

struct Keyboard {
    bool key_pressed            {false};
    bool key_released           {false};
    sf::Keyboard::Key keycode   {sf::Keyboard::Key::Unknown};
    int repeat_time             {10};
    int repeat_ticks            {10};
    bool wait_for_repeat        {false};
};

struct MenuTheme {
    // buttons
    sf::Color color_button_std          {0, 120, 10};
    sf::Color color_button_ext          {55, 0, 0};
    sf::Color color_button_shadow       {0, 0, 0, 190};
    sf::Color color_button_outline      {0, 120, 10};
    sf::Color color_button_text         {255, 255, 255};
    sf::Color color_button_text_outline {255, 255, 255};
    sf::Color color_button_text_shadow  {255, 255, 255};

    // lists
    sf::Color color_list_bg1            {0, 0, 101};
    sf::Color color_list_bg2            {51, 51, 255};
    sf::Color color_list_selected       {255, 0, 0};
};

struct Active_Widget_Animation {
    int ticks           {0};
    int ticks_per_flash {10};
    bool state          {false};
};



struct Menu {
    Widget page_main      [MAX_WIDGETS_PER_PAGE];
    Widget page_calibrate [MAX_WIDGETS_PER_PAGE];
    Controller controllers[MAX_CONTROLLERS];
    Active_Widget_Animation active_animation;
    Widget* active_page = page_main;
    calibrate::Calibrate_Page_Layout calibrate_layout;

    Widget* active_widget = &page_main[0];
    bool  should_exit     = false;
    int   return_code     = 0;

    MenuTheme theme;
    Menu_Event event = Menu_Event::NOTHING;

    static const int MAX_WIDGETS = 100;
    struct {
        int used_rects   = 0;
        int used_circles = 0;
        int used_labels  = 0;
        int used_colors  = 0;
        sf::RectangleShape rects      [MAX_WIDGETS*2];
        sf::CircleShape    circles    [MAX_WIDGETS*2];
        sf::Text           labels     [MAX_WIDGETS*2];
        sf::Color          colors     [MAX_WIDGETS*2];
    } object_pool;

    struct {
        sf::Font    font_button;
        sf::Texture texture_menu_background;
        sf::Texture texture_menu_logo;
        sf::Texture texture_gamepad;
        sf::Texture texture_thumbstick;
    } resources;

};
//
// functions
//
static void     handle_mouse (Mouse* mouse, Menu* menu, sf::RenderWindow* window);
static void     handle_keyboard (Keyboard* keyboard, Menu* menu);
static void     handle_window (sf::RenderWindow* window, Menu* menu);
static void     handle_gamepad (GamepadController* gamepad, Menu* menu);
static void     init_resources (Menu* menu);
static void     init_main_page (Menu* menu);
static void     init_calibrate_page (Menu* menu);
static void     update_active_animation (Menu* menu);
static Widget*  get_widget_neighbour (const Widget* widget, const Menu_Event trigger);
static void     change_active_widget (Menu* menu, const Menu_Event trigger);
static void     set_active_widget (Widget* widget, Menu* menu);
static void     detect_and_load_gamepads (Menu* menu);
static int      run_menu (Menu* menu, sf::RenderWindow* window);

static int acquire_rect(Menu* menu, const sf::Vector2f size = {0,0}) {
    int id = menu->object_pool.used_rects;
    menu->object_pool.rects[id].setSize(size);
    menu->object_pool.used_rects++;
    return id;
}
static int acquire_circle(Menu* menu, const float radius = 10.f) {
    int id = menu->object_pool.used_circles;
    menu->object_pool.circles[id].setRadius(radius);
    menu->object_pool.used_circles++;
    return id;
}
static int acquire_label(Menu* menu, const std::string &caption = "") {
    int id = menu->object_pool.used_labels;
    menu->object_pool.labels[id].setString(caption);
    menu->object_pool.used_labels++;
    return id;
}
static int acquire_color(Menu* menu, sf::Color color = sf::Color::Black) {
    int id = menu->object_pool.used_colors;
    menu->object_pool.colors[id] = color;
    menu->object_pool.used_colors++;
    return id;
}
}
}

