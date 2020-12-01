#pragma once
#include "../input/controller_tools.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <bitset>
#include <cassert>

namespace ss {
namespace menu {

// --------------------------------------------------------------------------------
//
// Data
//
// --------------------------------------------------------------------------------
struct Menu;
struct Widget;

static const int BIT_SHADOW      =0;
static const int BIT_TEXT_SHADOW =1;
static const int BIT_OUTLINE     =2;

static const int idx_fill    = 0;
static const int idx_outline = 1;
static const int idx_shadow  = 2;

static const int idx_position   = 0;
static const int idx_dimensions = 1;

struct Button_Attributes {
    sf::Vector2f   geometry[2]        {sf::Vector2f{0,0}, sf::Vector2f{300, 50}};
    sf::Color      button_colors[3]   {sf::Color{0,0,0}, sf::Color{0,0,0}, sf::Color{0,0,0}};
    sf::Color      caption_colors[3]  {sf::Color{255,255,255}, sf::Color{0,0,0}, sf::Color{255,255,255}};
    std::bitset<4> style;
    sf::Font*      text_font          {nullptr};
    std::string    caption            {"CAPTION"};
};

struct Label_Widget {
    int text        = -1;
    int text_shadow = -1;
};

struct Button_Widget {
    // soa style just indexes of entry in object pool (see menu)
    int btn_rect    = -1;
    int shadow_rect = -1;
    int text        = -1;
    int text_shadow = -1;
    int fill_color  = -1;
    int text_color  = -1;
};

struct Frame_Widget {
    int rect = -1;
};

struct ListRow_Widget {
    Button_Widget button;
    int fill_color = -1; // need to save this because it is changed depending on the row
    static const int MAX_LIST_ROWS = 8;
    Widget* siblings[MAX_LIST_ROWS];
    int number_siblings = 0;
};

struct Gamepad_Widget {
    sf::RectangleShape background;
    sf::CircleShape    left_stick;
    sf::CircleShape    right_stick;
    sf::Vector2f       left_stick_origin;
    sf::Vector2f       right_stick_origin;
    ControllerState*   controller_state {nullptr};
};

struct Image_Widget {
    int img_rect = -1;
};

struct Widget {
    enum {Anonymous, Label, Button, Frame, ListItem, Gamepad, Image} type;
    // ideally this would be a union, but we can't have nice things because of all the sfml oo cruft. Next time...
    // union {
    Label_Widget        label;
    Button_Widget       button;
    Frame_Widget        frame;
    ListRow_Widget      list;
    Gamepad_Widget      gamepad;
    Image_Widget        image;
    // };

    // common data
    std::string       id;
    bool              interactive     {true};
    std::bitset<4>    states;
    sf::Transformable transformable;

    // navigation order
    struct {
        Widget* above  {0};
        Widget* below  {0};
        Widget* right  {0};
        Widget* left   {0};
    } neighbours;
};

// --------------------------------------------------------------------------------
//
// Functions
//
// --------------------------------------------------------------------------------
std::string     get_widget_caption (Widget* widget, Menu* menu);
bool            has_mouse (const Widget* widget, Menu* menu, const sf::Vector2f&  mouse);

// inits
void            init_widget (Widget* widget, const std::string& id, const sf::FloatRect& geometry);
void            init_button_widget (Widget* widget, Menu* menu, const Button_Attributes& attribs);
void            init_image_widget (Widget* widget, Menu* menu, const sf::Vector2f dimensions, sf::Texture* texture) ;
void            init_listrow_widget (Widget* widget, const Button_Attributes& btn_attribs);
void            init_gamepad_widget (Widget* widget, const Menu* menu = nullptr);

// updates
void            update_button_widget (Widget* widget, const Menu* menu = nullptr);
void            update_list_widget (Widget* widget, const Menu* menu = nullptr);
void            update_gamepad_widget (Widget* widget, const Menu* menu = nullptr);
void            update_widget (Widget* widget, const Menu* menu = nullptr);

// draws
void            draw_label (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_frame (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_image (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_listrow (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_gamepad (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_widget (const Widget* widget, Menu* menu, sf::RenderWindow* window);

// misc
void            attach_controller (Gamepad_Widget* widget, ControllerState* state);
sf::FloatRect   get_widget_bounds(const Widget* widget, Menu* menu);


static int BIT_ACTIVE      = 0;
static int BIT_ENABLED     = 1;
static int BIT_SELECTED    = 2;
static int BIT_INTERACTIVE = 3;

inline void set_widget_enabled(Widget* widget, bool state) {
    widget->states[BIT_ENABLED] = state;
}

inline void set_widget_selected (Widget* widget, bool state) {
    widget->states[BIT_SELECTED] = state;

    // deselect all siblings
    if (widget->type == Widget::ListItem && state == true && widget->list.number_siblings) {
        for (int i = 0; i < widget->list.number_siblings; ++i) {
            assert (widget->list.siblings[i] != widget);
            widget->list.siblings[i]->states[BIT_SELECTED] = false;
        }
    }
}

inline void set_widget_active (Widget* widget, bool state) {
    widget->states[BIT_ACTIVE] = state;
}

inline void set_widget_interactive (Widget* widget, bool state) {
    widget->states[BIT_INTERACTIVE] = state;
}

inline bool widget_enabled (const Widget* widget) {
    return widget->states[BIT_ENABLED];
}

inline bool widget_selected (const Widget* widget) {
    return widget->states[BIT_SELECTED];
}

inline bool widget_active (const Widget* widget) {
    return widget->states[BIT_ACTIVE];
}
inline bool widget_interactive (const Widget* widget) {
    return widget->states[BIT_INTERACTIVE];
}
}
}


