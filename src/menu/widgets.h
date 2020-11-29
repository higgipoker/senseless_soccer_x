#pragma once
#include "../input/controller_tools.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <bitset>

namespace ss {
namespace menu {

// --------------------------------------------------------------------------------
//
// Data
//
// --------------------------------------------------------------------------------
struct Menu;
struct Widget;

struct Button_Attributes {
    std::string   caption            {"CAPTION"};
    sf::Vector2f  position           {0, 0};
    sf::Vector2f  dimensions         {300, 50};
    sf::Color     fill_color         {sf::Color::Green};
    sf::Color     outline_color      {sf::Color::Black};
    sf::Color     shadow_color       {sf::Color::Black};
    sf::Color     text_color         {sf::Color::White};
    sf::Color     text_outline_color {sf::Color::Black};
    sf::Color     text_shadow_color  {sf::Color::Black};
    bool          has_shadow         {false};
    bool          has_text_shadow    {true};
    bool          has_text_outline   {false};
    sf::Font*     text_font          {nullptr};
};

struct Label_Widget {
    std::string caption;
    sf::Text text;
    sf::Text text_shadow;
};

struct Button_Widget {
    std::string        caption       {"CAPTION"};
    sf::RectangleShape btn_rect      {{0, 0}};
    sf::RectangleShape shadow_rect   {{0, 0}};
    sf::Text           text;
    sf::Text           text_shadow;
    sf::Color          fill_color;
    sf::Color          text_color;
};

struct Frame_Widget {
    sf::RectangleShape rect;
};

struct ListRow_Widget {
    Button_Widget button;
    sf::Color fill_color;

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
    sf::RectangleShape img_rect{{0, 0}};
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
    sf::FloatRect     bounds          {0, 0, 0, 0};
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
void            init_widget            (Widget* widget, const int id, const sf::FloatRect& geometry);
std::string     get_widget_caption     (Widget* widget);
bool            has_mouse              (const Widget* widget, const sf::Vector2f&  mouse);

// updates
void            init_button_widget     (Button_Widget* widget, const Button_Attributes& attribs);
void            init_image_widget      (Widget* widget, const sf::FloatRect dimensions, sf::Texture* texture) ;
void            init_listrow_widget    (Widget* widget, const Button_Attributes& btn_attribs);
void            init_gamepad_widget    (Widget* widget, const Menu* menu = nullptr);

// inits
void            update_button_widget   (Widget* widget, const Menu* menu = nullptr);
void            update_list_widget     (Widget* widget, const Menu* menu = nullptr);
void            update_gamepad_widget  (Widget* widget, const Menu* menu = nullptr);
void            update_widget          (Widget* widget, const Menu* menu = nullptr);

// draws
void            draw_label             (const Label_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_frame             (const Frame_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_image             (const Image_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_listrow           (const ListRow_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_gamepad           (const Gamepad_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_widget            (const Widget* widget, sf::RenderWindow* window);

// misc
void            attach_controller      (Gamepad_Widget* widget, ControllerState* state) ;


static int BIT_ACTIVE   = 0;
static int BIT_DISABLED = 1;
static int BIT_SELECTED = 2;
inline void disable_widget (Widget* widget) {
    widget->states[BIT_DISABLED] = 1;
}
inline void enable_widget (Widget* widget) {
    widget->states[BIT_DISABLED] = 0;
}

inline void deselect_widget (Widget* widget) {
    widget->states[BIT_SELECTED] = 0;
}

inline void select_widget (Widget* widget) {
    widget->states[BIT_SELECTED] = 1;

    if (widget->type == Widget::ListItem) {
        // deselect all siblings
        for (int i = 0; i < widget->list.number_siblings; ++i) {
            deselect_widget (widget->list.siblings[i]);
        }
    }
}

inline void activate_widget (Widget* widget) {
    widget->states[BIT_ACTIVE] = 1;
}

inline void deactivate_widget (Widget* widget) {
    widget->states[BIT_ACTIVE] = 0;
}

inline bool widget_disabled (Widget* widget) {
    return widget->states[BIT_DISABLED];
}

inline bool widget_selected (Widget* widget) {
    return widget->states[BIT_SELECTED];
}

inline bool widget_active (Widget* widget) {
    return widget->states[BIT_ACTIVE];
}
}
}


