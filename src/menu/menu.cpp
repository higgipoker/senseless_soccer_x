#include "../input/GamepadController.hpp"
#include "../resources/Resources.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <bitset>
#include <cassert>

namespace ss {
namespace menu {

const unsigned char state_mask_idle       {0};        // 0000 0000
const unsigned char state_mask_active     {1 << 0};   // 0000 0001
const unsigned char state_mask_disabled   {1 << 1};   // 0000 0010
const unsigned char state_mask_selected   {1 << 2};   // 0000 0100

enum class   Widget_Type  {Anonymous, Button, ListItem, Gamepad};
enum class   Widget_State {Idle, Active, Disabled, Selected};
const int MAX_WIDGETS_PER_PAGE = 20;

static const sf::Vector2f IMAGE_SIZE_THUMBSTICK{599, 450};

struct Mouse {
    sf::Vector2f position;
    bool pressed = false;
    bool moved = false;
};

struct Keyboard {
    bool key_pressed = false;
    bool key_released = false;
    bool key_down = false;
    sf::Keyboard::Key keycode = sf::Keyboard::Key::Unknown;
};

struct Button_Attributes {
    std::string&  caption;
    sf::Vector2f& position;
    sf::Vector2f& dimensions;
    sf::Color&    fill_color;
    sf::Color&    outline_color;
    sf::Color&    text_color;
    sf::Color&    text_outline_color;
    sf::Font&     text_font;
    bool          has_shadow;
    bool          has_text_shadow;
    bool          has_text_outline;
};

struct Button_Widget {
    sf::RectangleShape btn_rect;
    sf::RectangleShape shadow_rect;
    sf::Text caption;
    sf::Text caption_shadow;
};

struct ListRow_Widget {
    Button_Widget button;
};

struct Gamepad_Widget {
    ControllerState* controller_state;
};

struct Widget {
    // stored as a union depending on widget type
    Widget_Type type = Widget_Type::Anonymous;
    union {
        Button_Widget  button;
        ListRow_Widget list;
        Gamepad_Widget gamepad;
    };

    // common data
    int                         id = 0;
    unsigned char               states = state_mask_idle;
    sf::FloatRect               bounds;
    sf::Transformable           transformable;

    // navigation order
    struct {
        int above = 0;
        int below = 0;
        int right = 0;
        int left  = 0;
    } neighbours;
};

struct Menu {
    Widget page_main      [MAX_WIDGETS_PER_PAGE];
    Widget page_clibrate  [MAX_WIDGETS_PER_PAGE];
    Widget* active_page = page_main;

    bool  should_exit     = false;
    int   return_code     = 0;

    struct {
        sf::Font font_button;
        sf::Texture texture_gamepad;
        sf::Texture texture_thumbstick;
    } resources;
};

static inline void disable_widget(Widget* widget) {
    widget->states |= state_mask_disabled;
}
static inline void enable_widget(Widget* widget) {
    widget->states &= ~(1 << state_mask_disabled);
}

static inline void select_widget(Widget* widget) {
    widget->states |= state_mask_selected;
}
static inline void deselect_widget(Widget* widget) {
    widget->states &= ~(1 << state_mask_selected);
}

static inline void activate_widget(Widget* widget) {
    widget->states |= state_mask_active;
}
static inline void deactivate_widget(Widget* widget) {
    widget->states &= ~(1 << state_mask_active);
}

static inline bool widget_disabled(Widget* widget) {
    return widget->states & state_mask_disabled;
}

static inline bool widget_selected(Widget* widget) {
    return widget->states & state_mask_selected;
}

static inline bool widget_active(Widget* widget) {
    return widget->states & state_mask_active;
}

void update_mouse(Mouse* mouse, sf::RenderWindow* window) {
    mouse->pressed = sf::Mouse::isButtonPressed (sf::Mouse::Left);

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

void update_keyboard(Keyboard* keyboard) {
    for(size_t i=static_cast<int>(sf::Keyboard::A); i< sf::Keyboard::KeyCount; ++i) {
        // note only one key at a time for now
        if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
            keyboard->keycode = static_cast<sf::Keyboard::Key>(i);
            if(!keyboard->key_pressed) {
                keyboard->key_down = true;
            }
            keyboard->key_pressed = true;
            return;
        }
    }
    if(keyboard->key_pressed) {
        keyboard->key_released = true;
    }
}

static void update_gamepad(GamepadController *gamepad) {
}

static void init_resources(Menu* menu) {
    menu->resources.texture_gamepad.loadFromFile (resources::gfx_folder() + "gamepad_bg.png");
}

static bool has_mouse (const Widget* widget, const sf::Vector2f&  mouse) {
    return (widget->bounds.contains (mouse));
}

static void init_widget(Widget* widget, const int id, const sf::FloatRect& geometry) {
    widget->id = id;
    widget->bounds = geometry;
}

static void init_button_widget(Button_Widget* widget, const Button_Attributes& attribs) {
    widget->btn_rect.setPosition(attribs.position);
    widget->btn_rect.setSize(attribs.dimensions);
    widget->btn_rect.setFillColor(attribs.fill_color);
    widget->btn_rect.setOutlineColor(attribs.outline_color);
    widget->btn_rect.setOutlineThickness(2); // TODO style themea

    if(attribs.has_shadow) {
        widget->shadow_rect = widget->btn_rect;
        widget->shadow_rect.setFillColor(sf::Color::Black);
        widget->shadow_rect.setOutlineColor(sf::Color::Black);
    }

    widget->caption.setFillColor(attribs.text_color);
    widget->caption.setFont(attribs.text_font);

    if(attribs.has_text_outline) {
        widget->caption.setOutlineThickness(2); // TODO themes
        widget->caption.setOutlineColor(attribs.text_outline_color);
    }
    if(attribs.has_text_shadow) {
        widget->caption_shadow.setFont(attribs.text_font);
        widget->caption_shadow.setOutlineThickness(2);
        widget->caption_shadow.setFillColor(sf::Color::Black);
    }
}

static void init_listrow_widget (Widget* widget, const Button_Attributes& btn_attribs) {
    // init the button aspect
    init_button_widget(&widget->list.button, btn_attribs);

    // init the extra list row aspects
}

static void init_gamepad_widget (Menu* menu, Widget* widget) {
}


static void update_button_widget(Button_Widget* widget, Menu* menu) {
}

static void update_widget(Widget* widget, Menu* menu) {
    switch (widget->type) {
    case Widget_Type::Button:
        update_button_widget (&widget->button, menu);
        break;
    case Widget_Type::ListItem:
        break;
    case Widget_Type::Gamepad:
        break;
    case Widget_Type::Anonymous:
        break;
    }
}

static void draw_button (const Button_Widget* widget,  sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->shadow_rect,    *states);
    window->draw (widget->btn_rect,       *states);
    window->draw (widget->caption_shadow, *states);
    window->draw (widget->caption,        *states);
}

static void draw_listrow(const ListRow_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    draw_button(&widget->button, window, states);
}

static void draw_gamepad (const Gamepad_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    assert (widget->controller_state); // the widget must be injected with the controller state data

    // draw the background

    // draw the left stick

    // draw the right stick

    // draw the dpad

    // draw the buttons
}

static void draw_widget (const Widget* widget, sf::RenderWindow* window) {
    sf::RenderStates states;
    states.transform = widget->transformable.getTransform();

    switch (widget->type) {
    case Widget_Type::Button:
        draw_button (&widget->button, window, &states);
        break;
    case Widget_Type::ListItem:
        draw_listrow(&widget->list, window, &states);
        break;
    case Widget_Type::Gamepad:
        draw_gamepad(&widget->gamepad, window, &states);
        break;
    case Widget_Type::Anonymous:
        break;
    }
}

static int run_menu(Menu* menu, sf::RenderWindow* window) {
    Mouse mouse;
    Keyboard keyboard;
    GamepadController gamepad;
    sf::Vector2f last_mouse_position;

    while(!menu->should_exit) {
        // handle input
        if(window->hasFocus()) {
            update_mouse(&mouse, window);
            update_keyboard(&keyboard);
            update_gamepad(&gamepad);
        }

        // update
        for(int i=0; i<MAX_WIDGETS_PER_PAGE; ++i) {
            if(menu->active_page[i].type == Widget_Type::Anonymous) break;
            update_widget(&menu->active_page[i], menu);
        }

        // render

    }
    return menu->return_code;
}

} // namespace menu
} // namespace ss
