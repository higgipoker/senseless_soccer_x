#pragma once
#include "events.h"
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
// data, then functions
// --------------------------------------------------------------------------------
//
// Data
//
// --------------------------------------------------------------------------------
struct Menu;
struct Widget;

struct Button_Attributes {
    // indexes for the below arrays
    enum {Position, Dimensions}   Geometry;
    enum {Fill, Outline, Shadow}  Colors;

    // position and dimensions
    sf::Vector2f geometry[2];

    // background colors for fill, outline and shadow
    sf::Color button_colors[3];

    // text colors for fill, outlinge and shadow
    sf::Color caption_colors[3];

    // button caption text
    std::string caption;

    // id for tracking if the button was pressed
    Event id {Event::None};

    // handle of font to be used for caption
    int text_font {-1};
};

// ***************************
// Label
// ***************************
struct Label_Widget {
    int text        {-1};
    int text_shadow {-1};
};
// ***************************
// Button
// ***************************
struct Button_Widget {
    // soa style just indexes of entry in object pool (see menu)
    int btn_rect    {-1};
    int shadow_rect {-1};
    int text        {-1};
    int text_shadow {-1};
    int fill_color  {-1};
    int text_color  {-1};
};
// ***************************
// Frame
// ***************************
struct Frame_Widget {
    int rect = {-1};
};
// ***************************
// ListRow
// ***************************
struct ListRow_Widget {
    Button_Widget       button;               // a list row is implemented in terms of a button (has caption, can be active, pressed etc)
    int                 index {-1};
    int                 fill_color      {-1}; // need to save this because it is changed depending on the row
    int                 number_siblings {0};
    static const int    MAX_LIST_ROWS   {8};
    Widget*             siblings[MAX_LIST_ROWS];
};
// ***************************
// Calibrate
// ***************************
struct Thumbstick_Diagnostic_Widget {
    enum Stick {Left, Right} attached_stick {Stick::Left};
    int input_circle  {-1};
    int outer_rect    {-1};
    ControllerState*   controller_state {nullptr};
     Calibration* cali {nullptr};
};
// ***************************
// Test
// ***************************
struct Thumbstick_Test_Widget {
    enum Stick {Left, Right} attached_stick {Stick::Left};
    int input_circle   {-1};
    int colored_circle {-1};
    int outer_circle   {-1};
    int bg_circle      {-1};
    int x_circle       {-1};
    sf::Vertex line    [2];
    ControllerState*   controller_state {nullptr};
};
// ***************************
// Gamepad
// ***************************
struct Gamepad_Widget {
    int background     {-1};
    int left_stick;
    int right_stick;
    ControllerState*   controller_state {nullptr};
};
// ***************************
// Image
// ***************************
struct Image_Widget {
    int img_rect {-1};
};
// ***************************
// Widget
// ***************************
struct Widget {
    enum Type {Anonymous, Label, Button, Frame, ListItem, Gamepad, Calibrate, Test, Image} type;
    enum {Visible, Active, Enabled, Selected, Interactive} state;
    // ideally this would be a union, but we can't have nice things because of all the sfml oo cruft. Next time...
    // union {
    Label_Widget                    label;
    Button_Widget                   button;
    Frame_Widget                    frame;
    ListRow_Widget                  list;
    Gamepad_Widget                  gamepad;
    Image_Widget                    image;
    Thumbstick_Diagnostic_Widget    thumbstick_diagnostic;
    Thumbstick_Test_Widget          thumbstick_test;
    // };

    // common data
    Event             id              {Event::None};
    bool              interactive     {true};
    int               transform       {-1};
    std::bitset<8>    states;

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
// inits
void            init_widget                         (Widget* widget, Menu* menu, const Widget::Type type);
void            init_button_widget                  (Widget* widget, Menu* menu, const Button_Attributes& attribs);
void            init_image_widget                   (Widget* widget, Menu* menu, const sf::Vector2f dimensions, const sf::Texture* texture) ;
void            init_listrow_widget                 (Widget* widget, const Button_Attributes& btn_attribs);
void            init_gamepad_widget                 (Widget* widget, Menu* menu = nullptr);
void            init_thumbstick_calibrate_widget    (Widget* widget, Menu* menu = nullptr);
void            init_thumbstick_test_widget         (Widget* widget, Menu* menu = nullptr);

// updates
void            update_button_widget                (Widget* widget, Menu* menu = nullptr);
void            update_list_widget                  (Widget* widget, Menu* menu = nullptr);
void            update_gamepad_widget               (Widget* widget, Menu* menu = nullptr);
void            update_thumbstick_calibrate_widget  (Widget* widget, Menu* menu = nullptr);
void            update_thumbstick_test_widget       (Widget* widget, Menu* menu = nullptr);
void            update_widget                       (Widget* widget, Menu* menu = nullptr);

// draws
void            draw_thumbstick_calibrate   (const Widget* widget, Menu *menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_thumbstick_test        (const Widget* widget, Menu *menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_label                  (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_frame                  (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_image                  (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_listrow                (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_gamepad                (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states);
void            draw_widget                 (const Widget* widget, Menu* menu, sf::RenderWindow* window);

// positions
void            set_widget_position    (Widget* widget, Menu* menu, const sf::Vector2f& pos);
sf::Vector2f    get_widget_position    (const Widget* widget, Menu* menu);

// misc
void            attach_controller      (Thumbstick_Test_Widget* widget,Thumbstick_Test_Widget::Stick stick, ControllerState* state);
void            detatch_controller     (Thumbstick_Test_Widget* widget);
void            attach_controller      (Thumbstick_Diagnostic_Widget* widget, Thumbstick_Diagnostic_Widget::Stick stick, ControllerState* state,  Calibration* cali);
void            detatch_controller     (Thumbstick_Diagnostic_Widget* widget);
sf::FloatRect   get_widget_bounds      (const Widget* widget, Menu* menu);
std::string     get_widget_caption     (const Widget* widget, Menu* menu);
bool            has_mouse              (const Widget* widget, Menu* menu, const sf::Vector2f&  mouse);
Widget*         get_widget_neighbour   (const Widget* widget, const Event trigger);



// --
// set visible state
// --
inline void set_widget_visible(Widget* widget, bool state) {
    widget->states[Widget::Visible] = state;
}
// --
// set enabled state
// --
inline void set_widget_enabled (Widget* widget, bool state) {
    widget->states[Widget::Enabled] = state;
}
// --
// set selected state
//
inline void set_widget_selected (Widget* widget, bool state) {
    widget->states[Widget::Selected] = state;

    // deselect all siblings
    if (widget->type == Widget::ListItem && state == true && widget->list.number_siblings) {
        for (int i = 0; i < widget->list.number_siblings; ++i) {
            widget->list.siblings[i]->states[Widget::Selected] = false;
        }
    }
}
// --
// set active state
// --
inline void set_widget_active (Widget* widget, bool state) {
    widget->states[Widget::Active] = state;
}
// --
// set interactive state
// --
inline void set_widget_interactive (Widget* widget, bool state) {
    widget->states[Widget::Interactive] = state;
}
// --
// get visible state
// --
inline bool widget_visible(const Widget* widget) {
    return widget->states[Widget::Visible];
}
// --
// get enabled state
// --
inline bool widget_enabled (const Widget* widget) {
    return widget->states[Widget::Enabled];
}
// --
// get selected state
// --
inline bool widget_selected (const Widget* widget) {
    return widget->states[Widget::Selected];
}
// --
// get active state
// --
inline bool widget_active (const Widget* widget) {
    return widget->states[Widget::Active];
}
// --
// get interactive state
// --
inline bool widget_interactive (const Widget* widget) {
    return widget->states[Widget::Interactive];
}
} // namespace
} // namespace


