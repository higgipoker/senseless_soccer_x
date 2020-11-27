//
// here we go, a flat, performant non-oop menu system
//
#include "../global.hpp"
#include "../input/GamepadController.hpp"
#include "../resources/Resources.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

namespace ss {
namespace menu {

const unsigned char state_mask_idle       {0};        // 0000 0000
const unsigned char state_mask_active     {1 << 0};   // 0000 0001
const unsigned char state_mask_disabled   {1 << 1};   // 0000 0010
const unsigned char state_mask_selected   {1 << 2};   // 0000 0100

const int MAX_WIDGETS_PER_PAGE = 20;

struct Mouse {
    sf::Vector2f position    {0,0};
    bool pressed             {false};
    bool moved               {false};
};

struct Keyboard {
    bool key_pressed            {false};
    bool key_released           {false};
    bool key_down               {false};
    sf::Keyboard::Key keycode   {sf::Keyboard::Key::Unknown};
};

struct MenuTheme {
    sf::Color color_button_std          {0, 120, 10};
    sf::Color color_button_ext          {55, 0, 0};
    sf::Color color_button_shadow       {0, 0, 0, 190};
    sf::Color color_button_outline      {255, 255, 255};
    sf::Color color_button_text         {0, 0, 0};
    sf::Color color_button_text_outline {255, 255, 255};
    sf::Color color_button_text_shadow  {255, 255, 255};
};

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

struct Button_Widget {
    std::string        caption       {"CAPTION"};
    sf::RectangleShape btn_rect      {{0,0}};
    sf::RectangleShape shadow_rect   {{0,0}};
    sf::Text           text;
    sf::Text           text_shadow;
};

struct ListRow_Widget {
    Button_Widget button;
};

struct Gamepad_Widget {
    ControllerState* controller_state {nullptr};
};

struct Image_Widget {
    sf::RectangleShape img_rect{{0, 0}};
};

struct Widget {
    enum {Anonymous, Button, ListItem, Gamepad, Image} type;
    // ideally this would be a union, but we can't have nice things because of all the sfml oo cruft. Next time...
    // union {
    Button_Widget       button;
    ListRow_Widget      list;
    Gamepad_Widget      gamepad;
    Image_Widget        image;
    // };

    // common data
    int               id              {0};
    unsigned char     states          {state_mask_idle};
    sf::FloatRect     bounds          {0,0,0,0};
    sf::Transformable transformable;

    // navigation order
    struct {
        int above  {0};
        int below  {0};
        int right  {0};
        int left   {0};
    } neighbours;
};

struct Menu {
    Widget page_main      [MAX_WIDGETS_PER_PAGE];
    Widget page_clibrate  [MAX_WIDGETS_PER_PAGE];

    bool  should_exit     = false;
    int   return_code     = 0;

    MenuTheme theme;

    struct {
        sf::Font font_button;
        sf::Texture texture_menu_background;
        sf::Texture texture_menu_logo;
        sf::Texture texture_gamepad;
        sf::Texture texture_thumbstick;
    } resources;
};

static inline void disable_widget (Widget* widget) {
    widget->states |= state_mask_disabled;
}
static inline void enable_widget (Widget* widget) {
    widget->states &= ~ (1 << state_mask_disabled);
}

static inline void select_widget (Widget* widget) {
    widget->states |= state_mask_selected;
}
static inline void deselect_widget (Widget* widget) {
    widget->states &= ~ (1 << state_mask_selected);
}

static inline void activate_widget (Widget* widget) {
    widget->states |= state_mask_active;
}
static inline void deactivate_widget (Widget* widget) {
    widget->states &= ~ (1 << state_mask_active);
}

static inline bool widget_disabled (Widget* widget) {
    return widget->states & state_mask_disabled;
}

static inline bool widget_selected (Widget* widget) {
    return widget->states & state_mask_selected;
}

static inline bool widget_active (Widget* widget) {
    return widget->states & state_mask_active;
}

static void handle_mouse (Mouse* mouse, sf::RenderWindow* window) {
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

static void handle_keyboard (Keyboard* keyboard) {
    for (size_t i = static_cast<int> (sf::Keyboard::A); i < sf::Keyboard::KeyCount; ++i) {
        // note only one key at a time for now
        if (sf::Keyboard::isKeyPressed (static_cast<sf::Keyboard::Key> (i))) {
            keyboard->keycode = static_cast<sf::Keyboard::Key> (i);
            if (!keyboard->key_pressed) {
                keyboard->key_down = true;
            }
            keyboard->key_pressed = true;

            return;
        }
    }
    if (keyboard->key_pressed) {
        keyboard->key_released = true;
    }
}

static void handle_window (sf::RenderWindow* window, Menu* menu) {
    sf::Event event;
    while (window->pollEvent (event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            menu->should_exit = true;
        }
    }
}

static void handle_gamepad (GamepadController* gamepad) {
}

static void init_resources (Menu* menu) {
    menu->resources.texture_menu_background.loadFromFile (resources::gfx_folder() + "bg1.png");
    menu->resources.texture_menu_logo.loadFromFile (resources::gfx_folder() + "logo2.png");
    menu->resources.texture_gamepad.loadFromFile (resources::gfx_folder() + "gamepad_bg.png");
    menu->resources.font_button.loadFromFile (resources::font_folder() + "swos2.ttf");
}

static bool has_mouse (const Widget* widget, const sf::Vector2f&  mouse) {
    return (widget->bounds.contains (mouse));
}

static void init_widget (Widget* widget, const int id, const sf::FloatRect& geometry) {
    widget->id = id;
    widget->bounds = geometry;
}

static void init_button_widget (Button_Widget* widget, const Button_Attributes& attribs) {
    widget->btn_rect.setPosition (attribs.position);
    widget->btn_rect.setSize (attribs.dimensions);
    widget->btn_rect.setFillColor (attribs.fill_color);
    widget->btn_rect.setOutlineColor (attribs.outline_color);
    widget->btn_rect.setOutlineThickness (2); // TODO style themea

    if (attribs.has_shadow) {
        widget->shadow_rect = widget->btn_rect;
        widget->shadow_rect.setFillColor (attribs.shadow_color);
        widget->shadow_rect.setOutlineColor (attribs.shadow_color);
        widget->shadow_rect.move ({4, 4});
    }

    widget->caption = attribs.caption;
    widget->text.setString (attribs.caption);
    widget->text.setFillColor (attribs.text_color);
    widget->text.setFont (*attribs.text_font);

    if (attribs.has_text_outline) {
        widget->text.setOutlineThickness (2); // TODO themes
        widget->text.setOutlineColor (attribs.text_outline_color);
    }

    // size the caption
    while (widget->text.getGlobalBounds().width >= widget->btn_rect.getGlobalBounds().width - 12) {
        widget->caption = widget->caption.substr (0, widget->caption.size() - 3) + ".";
        widget->text.setString (widget->caption);
    }
    widget->text_shadow.setString (widget->caption);
    // align the caption
    widget->text.setOrigin (widget->text.getLocalBounds().width / 2, widget->text.getLocalBounds().height / 2);
    widget-> text.setPosition ({
        (attribs.position.x + attribs.dimensions.x / 2)
        ,
        (attribs.position.y + attribs.dimensions.y / 2 - widget->text.getCharacterSize() / 4)
    });
    if (attribs.has_text_shadow) {
        widget->text_shadow.setFont (*attribs.text_font);
        widget->text_shadow.setOutlineThickness (2);
        widget->text_shadow.setFillColor (sf::Color::Black);
        widget->text_shadow.setOrigin (widget->text.getLocalBounds().width / 2, widget->text.getLocalBounds().height / 2);
        widget->text_shadow.setPosition (widget->text.getPosition());
        widget->text_shadow.move ({3, 2});
    }
}

static void init_image_widget (Widget* widget, const sf::FloatRect dimensions, sf::Texture* texture) {
    widget->type = Widget::Image;
    widget->image.img_rect.setPosition ({dimensions.left, dimensions.top});
    widget->image.img_rect.setSize ({dimensions.width, dimensions.height});
    widget->image.img_rect.setTexture (texture);
}

static void init_listrow_widget (Widget* widget, const Button_Attributes& btn_attribs) {
    // init the button aspect
    init_button_widget (&widget->list.button, btn_attribs);

    // init the extra list row aspects
}

static void init_gamepad_widget (Menu* menu, Widget* widget) {
}

static void update_button_widget (Widget* widget, Menu* menu) {
    if(widget_disabled(widget)) {
        widget->button.btn_rect.setFillColor({0,0,0,0});
    } else {
        // set back to original color here
    }
}

static void update_widget (Widget* widget, Menu* menu) {
    switch (widget->type) {
    case Widget::Button:
        update_button_widget (widget, menu);
        break;
    case Widget::ListItem:
        break;
    case Widget::Gamepad:
        break;
    case Widget::Image:
        break;
    case Widget::Anonymous:
        global::log("TRYING TO UPDATE AN ANONYMOUSE WIDGET");
        assert(false);
        break;
    }
}

static void draw_button (const Button_Widget* widget,  sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->shadow_rect,    *states);
    window->draw (widget->btn_rect,       *states);
    window->draw (widget->text_shadow,    *states);
    window->draw (widget->text,           *states);
}

static void draw_image (const Image_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->img_rect, *states);
}

static void draw_listrow (const ListRow_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    draw_button (&widget->button, window, states);
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
    case Widget::Button:
        draw_button (&widget->button, window, &states);
        break;
    case Widget::ListItem:
        draw_listrow (&widget->list, window, &states);
        break;
    case Widget::Gamepad:
        draw_gamepad (&widget->gamepad, window, &states);
        break;
    case Widget::Image:
        draw_image (&widget->image, window, &states);
        break;
    case Widget::Anonymous:
        global::log("TRYING TO DRAW AN ANONYMOUSE WIDGET");
        assert(false);
        break;

    }
}

static void init_main_page (Menu* menu) {
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
    button1->type = Widget::Button;
    disable_widget(button1);
}

static int run_menu (Menu* menu, sf::RenderWindow* window) {
    Mouse             mouse;
    Keyboard          keyboard;
    GamepadController gamepad;

    init_resources (menu);
    init_main_page (menu);

    while (!menu->should_exit) {

        // handle input
        if (window->hasFocus()) {
            handle_mouse    (&mouse, window);
            handle_keyboard (&keyboard);
            handle_gamepad  (&gamepad);
            handle_window   (window, menu);

            // tmp special case exit
            if (keyboard.keycode == sf::Keyboard::Key::Escape) {
                menu->should_exit = true;
                menu->return_code = 0;
            }
        }

        // update
        int i = 0;
        while (menu->page_main[i].type != Widget::Anonymous) {
            update_widget (&menu->page_main[i++], menu);
        }

        // render
        window->clear (sf::Color::Magenta);
        i = 0;
        while (menu->page_main[i].type != Widget::Anonymous) {
            draw_widget (&menu->page_main[i++], window);
        }
        window->display();
    }
    return menu->return_code;
}

} // namespace menu
} // namespace ss

