#pragma once
#include "widgets.h"
#include "calibration_page.h"
#include "events.h"
#include "../input/Controller.hpp"
#include "../input/GamepadController.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>
#include <map>

namespace ss {
namespace menu {
static const int MAX_WIDGETS_PER_PAGE = 20; // todo: optimize when we know for sure how many we need
static const int MAX_CONTROLLERS = 8; // max supported by sfml

// ***************************
// Keyboard Keys Map
// ***************************
static std::map<sf::Keyboard::Key, Event> keymap = {
    {sf::Keyboard::W,        Event::Up},
    {sf::Keyboard::S,        Event::Down},
    {sf::Keyboard::A,        Event::Left},
    {sf::Keyboard::D,        Event::Right},
    {sf::Keyboard::Enter,    Event::Fire},
    {sf::Keyboard::Space,    Event::Fire},
    {sf::Keyboard::RControl, Event::Fire},
};
// ***************************
// Mouse
// ***************************
struct Mouse {
    enum {Down, Pressed, Released, Moved} state;
    sf::Vector2f position    {0, 0};
    std::bitset<4> states;
};
// ***************************
// Keyboard
// ***************************
struct Keyboard {
    bool key_pressed            {false};
    bool key_released           {false};
    sf::Keyboard::Key keycode   {sf::Keyboard::Key::Unknown};
    int repeat_time             {10};
    int repeat_ticks            {10};
    bool wait_for_repeat        {false};
};
// ***************************
// MenuTheme
// ***************************
struct MenuTheme {
    // buttons
    sf::Vector2f dimensions_btn_small   {300, 50};
    sf::Vector2f dimensions_btn_big     {300, 50};
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
// ***************************
// Active_Widget_Animation
// ***************************
struct Active_Widget_Animation {
    int ticks           {0};
    int ticks_per_flash {10};
    bool state          {false};
};
// ***************************
// Menu
// ***************************
struct Menu {
    // a page is an array of widgets
    Widget page_main      [MAX_WIDGETS_PER_PAGE];
    Widget page_calibrate [MAX_WIDGETS_PER_PAGE];

    // controllers
    Controller controllers[MAX_CONTROLLERS];

    // track the currently displayed page
    Widget* active_page = page_main;

    // track the active widget
    Widget* active_widget = &page_main[0];

    // animate the active widget
    Active_Widget_Animation active_animation;

    // tracks ids of widgets on the calibration page
    calibrate::Calibrate_Page_Layout calibrate_layout;

    // menu exit conditions
    bool  should_exit     = false;
    int   return_code     = 0;
    Event event           = Event::None;

    // color and style of widgets
    MenuTheme theme;

    // resource pool
    static const int MAX_WIDGETS = 50;
    struct {
        int used_rects    = 0;
        int used_circles  = 0;
        int used_labels   = 0;
        int used_colors   = 0;
        int used_fonts    = 0;
        int used_textures = 0;
        int used_transforms = 0;

        // max sizes will be optimized later depemding on usage requirements
        sf::RectangleShape rects      [MAX_WIDGETS * 2];
        sf::CircleShape    circles    [MAX_WIDGETS * 2];
        sf::Text           labels     [MAX_WIDGETS * 2];
        sf::Color          colors     [MAX_WIDGETS * 2];
        sf::Font           fonts      [MAX_WIDGETS * 2];
        sf::Texture        textures   [MAX_WIDGETS];
        sf::Transform      transforms [MAX_WIDGETS];

    } object_pool;

    // handles for common resources
    struct {
        int texture_menu_background = -1;
        int font_button             = -1;
        int texture_menu_logo       = -1;
        int texture_gamepad         = -1;
        int texture_thumbstick      = -1;

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
static void     next_active_widget (Menu* menu, const Event trigger);
static void     set_active_widget (Widget* widget, Menu* menu);
static void     detect_and_load_gamepads (Menu* menu);
static int      run_menu (Menu* menu, sf::RenderWindow* window);
static Widget*  get_widget_neighbour (const Widget* widget, const Event trigger);
//
// functions to acquire resources
//
inline static int acquire_rect (Menu* menu, const sf::Vector2f size = {0, 0}) {
    int id = menu->object_pool.used_rects;
    menu->object_pool.rects[id].setSize (size);
    menu->object_pool.used_rects++;
    return id;
}
inline static int acquire_circle (Menu* menu, const float radius = 10.f) {
    int id = menu->object_pool.used_circles;
    menu->object_pool.circles[id].setRadius (radius);
    menu->object_pool.used_circles++;
    return id;
}
inline static int acquire_label (Menu* menu, const std::string& caption = "") {
    int id = menu->object_pool.used_labels;
    menu->object_pool.labels[id].setString (caption);
    menu->object_pool.used_labels++;
    return id;
}
inline static int acquire_color (Menu* menu, sf::Color color = sf::Color::Black) {
    int id = menu->object_pool.used_colors;
    menu->object_pool.colors[id] = color;
    menu->object_pool.used_colors++;
    return id;
}
inline static int acquire_texture (Menu* menu, const std::string& file) {
    int id = menu->object_pool.used_textures;
    menu->object_pool.textures[id].loadFromFile (file);
    menu->object_pool.used_textures++;
    return id;
}
inline static int acquire_font (Menu* menu, const std::string& file) {
    int id = menu->object_pool.used_fonts;
    menu->object_pool.fonts[id].loadFromFile (file);
    menu->object_pool.used_fonts++;
    return id;
}
inline static int acquire_transform (Menu* menu) {
    int id = menu->object_pool.used_transforms;
    menu->object_pool.used_transforms++;
    return id;
}
inline sf::RectangleShape get_rect (Menu* menu, int id) {
    return menu->object_pool.rects[id];
}
} // namespace
} // namespace


