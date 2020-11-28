//
// here we go, a flat, performant non-oop menu system
//
#include "../global.hpp"
#include "../input/GamepadController.hpp"
#include "../resources/Resources.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <bitset>
#include <iostream>
#include <cassert>

namespace ss {
namespace menu {

struct Widget;

static int BIT_ACTIVE   = 0;
static int BIT_DISABLED = 1;
static int BIT_SELECTED = 2;

const int MAX_WIDGETS_PER_PAGE = 20;

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
    sf::Color color_button_text         {0, 0, 0};
    sf::Color color_button_text_outline {255, 255, 255};
    sf::Color color_button_text_shadow  {255, 255, 255};

    // lists
    sf::Color color_list_bg1            {0, 0, 101};
    sf::Color color_list_bg2            {51, 51, 255};
    sf::Color color_list_selected       {255, 0, 0};
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

struct Active_Widget_Animation {
    int ticks           {0};
    int ticks_per_flash {10};
    bool state          {false};
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
    int               id              {0};
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

struct Menu {
    Widget page_main      [MAX_WIDGETS_PER_PAGE];
    Widget page_calibrate [MAX_WIDGETS_PER_PAGE];
    Widget* active_page = page_calibrate;

    Widget* active_widget = &page_main[0];
    bool  should_exit     = false;
    int   return_code     = 0;

    MenuTheme theme;
    Menu_Event event = Menu_Event::NOTHING;

    struct {
        sf::Font font_button;
        sf::Texture texture_menu_background;
        sf::Texture texture_menu_logo;
        sf::Texture texture_gamepad;
        sf::Texture texture_thumbstick;
    } resources;
};

static std::string get_widget_caption (Widget* widget) {
    std::string capt;
    switch (widget->type) {
    case Widget::Label:
        return widget->label.text.getString();
    case Widget::Button:
        return widget->button.text.getString();
    case Widget::Frame:
        break;
    case Widget::ListItem:
        return widget->list.button.text.getString();
        break;
    case Widget::Gamepad:
        break;
    case Widget::Image:
        break;
    case Widget::Anonymous:
        break;
    }
    return capt;
}
static inline void disable_widget (Widget* widget) {
    widget->states[BIT_DISABLED] = 1;
}
static inline void enable_widget (Widget* widget) {
    widget->states[BIT_DISABLED] = 0;
}

static inline void deselect_widget (Widget* widget) {
    widget->states[BIT_SELECTED] = 0;
}

static inline void select_widget (Widget* widget) {
    widget->states[BIT_SELECTED] = 1;

    if (widget->type == Widget::ListItem) {
        // deselect all siblings
        for (int i = 0; i < widget->list.number_siblings; ++i) {
            deselect_widget (widget->list.siblings[i]);
        }
    }
}

static inline void activate_widget (Widget* widget) {
    widget->states[BIT_ACTIVE] = 1;
}
static inline void deactivate_widget (Widget* widget) {
    widget->states[BIT_ACTIVE] = 0;
}

static inline bool widget_disabled (Widget* widget) {
    return widget->states[BIT_DISABLED];
}

static inline bool widget_selected (Widget* widget) {
    return widget->states[BIT_SELECTED];
}

static inline bool widget_active (Widget* widget) {
    return widget->states[BIT_ACTIVE];
}

static bool has_mouse (const Widget* widget, const sf::Vector2f&  mouse) {
    return (widget->bounds.contains (mouse));
}

static void handle_mouse (Mouse* mouse, Menu* menu, sf::RenderWindow* window) {
    bool was_down = mouse->down;
    mouse->pressed = false;
    mouse->down = sf::Mouse::isButtonPressed (sf::Mouse::Left);
    if (mouse->down && !was_down) {
        mouse->pressed = true;
    } else if (!mouse->down && was_down) {
        mouse->released = true;
        if (has_mouse (menu->active_widget, mouse->position)) {
            menu->event = Menu_Event::FIRE;
        }
    }

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

static void handle_keyboard (Keyboard* keyboard, Menu* menu) {
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

static void handle_window (sf::RenderWindow* window, Menu* menu) {
    sf::Event event;
    while (window->pollEvent (event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            menu->should_exit = true;
        }
    }
}

static void handle_gamepad (GamepadController* gamepad, Menu* menu) {
}

static void init_resources (Menu* menu) {
    menu->resources.texture_menu_background.loadFromFile (resources::gfx_folder() + "bg1.png");
    menu->resources.texture_menu_logo.loadFromFile (resources::gfx_folder() + "logo2.png");
    menu->resources.texture_gamepad.loadFromFile (resources::gfx_folder() + "gamepad_bg.png");
    menu->resources.texture_thumbstick.loadFromFile (resources::gfx_folder() + "thumbstick.png");
    menu->resources.texture_thumbstick.loadFromFile (resources::gfx_folder() + "thumbstick.png");
    menu->resources.font_button.loadFromFile (resources::font_folder() + "swos2.ttf");
}

static void init_widget (Widget* widget, const int id, const sf::FloatRect& geometry) {
    widget->id = id;
    widget->bounds = geometry;
}

static void init_button_widget (Button_Widget* widget, const Button_Attributes& attribs) {
    widget->fill_color = attribs.fill_color;
    widget->text_color = attribs.text_color;
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
    widget->interactive = false;
    widget->image.img_rect.setPosition ({dimensions.left, dimensions.top});
    widget->image.img_rect.setSize ({dimensions.width, dimensions.height});
    widget->image.img_rect.setTexture (texture);
}

static void init_listrow_widget (Widget* widget, const Button_Attributes& btn_attribs) {

}

static void init_gamepad_widget (Widget* widget, Menu* menu) {
    widget->type = Widget::Gamepad;

    // background
    widget->gamepad.background.setSize ({500, 320});
    widget->gamepad.background.setTexture (&menu->resources.texture_gamepad);

    // thumbsticks
    widget->gamepad.left_stick.setRadius (39);
    widget->gamepad.right_stick.setRadius (39);



    widget->gamepad.left_stick.setTexture (&menu->resources.texture_thumbstick);
    widget->gamepad.right_stick.setTexture (&menu->resources.texture_thumbstick);
}

static void update_button_widget (Widget* widget, Menu* menu) {
    if (widget_active (widget)) {
        widget->button.btn_rect.setOutlineColor ({255, 255, 255, 255});
    } else {
        widget->button.btn_rect.setOutlineColor (menu->theme.color_button_outline);

        if (widget_disabled (widget)) {
            widget->button.btn_rect.setFillColor ({widget->button.fill_color.r, widget->button.fill_color.g, widget->button.fill_color.b, 30});
            widget->button.btn_rect.setOutlineColor ({widget->button.fill_color.r, widget->button.fill_color.g, widget->button.fill_color.b, 30});
            widget->button.text.setFillColor ({widget->button.text_color.r, widget->button.text_color.g, widget->button.text_color.b, 30});
        } else {
            widget->button.btn_rect.setFillColor (widget->button.fill_color);
            widget->button.btn_rect.setOutlineColor (widget->button.fill_color);
            widget->button.text.setFillColor (widget->button.text_color);
        }
    }
}

static void update_list_widget (Widget* widget, Menu* menu) {
    if (widget_active (widget)) {
        widget->list.button.btn_rect.setOutlineColor ({255, 255, 255, 255});
    } else {
        widget->list.button.btn_rect.setOutlineColor (widget->list.fill_color);
    }
    if (widget_selected (widget)) {
        widget->list.button.btn_rect.setFillColor (menu->theme.color_button_ext);

    } else {
        widget->list.button.btn_rect.setFillColor (widget->list.fill_color);
    }
}

static void update_widget (Widget* widget, Menu* menu) {
    switch (widget->type) {
    case Widget::Label:
        break;
    case Widget::Button:
        update_button_widget (widget, menu);
        break;
    case Widget::Frame:
        break;
    case Widget::ListItem:
        update_list_widget (widget, menu);
        break;
    case Widget::Gamepad:
        break;
    case Widget::Image:
        break;
    case Widget::Anonymous:
        global::log ("TRYING TO UPDATE AN ANONYMOUSE WIDGET");
        assert (false);
        break;
    }
}

static void draw_label (const Label_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->text, *states);
}

static void draw_button (const Button_Widget* widget,  sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->shadow_rect,    *states);
    window->draw (widget->btn_rect,       *states);
    window->draw (widget->text_shadow,    *states);
    window->draw (widget->text,           *states);
}

static void draw_frame (const Frame_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->rect);
}

static void draw_image (const Image_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->img_rect, *states);
}

static void draw_listrow (const ListRow_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    draw_button (&widget->button, window, states);
}

static void draw_gamepad (const Gamepad_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    //assert (widget->controller_state); // the widget must be injected with the controller state data

    // draw the background
    window->draw (widget->background, *states);

    // draw the thumbsticks
    window->draw (widget->left_stick, *states);
    window->draw (widget->right_stick, *states);

    // draw the dpad

    // draw the buttons
}

static void draw_widget (const Widget* widget, sf::RenderWindow* window) {
    sf::RenderStates states;
    states.transform = widget->transformable.getTransform();

    switch (widget->type) {
    case Widget::Label:
        draw_label (&widget->label, window, &states);
        break;
    case Widget::Button:
        draw_button (&widget->button, window, &states);
        break;
    case Widget::Frame:
        draw_frame (&widget->frame, window, &states);
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
        global::log ("TRYING TO DRAW AN ANONYMOUSE WIDGET");
        assert (false);
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
    button1->bounds = button1->button.btn_rect.getGlobalBounds();
    button1->type = Widget::Button;
    menu->active_widget = button1;

    // button 2
    i++;
    Widget* button2 = &menu->page_main[i];
    attribs.text_font = &menu->resources.font_button;
    attribs.position = {300, 400};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    attribs.caption = "CALIBRATE";
    init_button_widget (&button2->button, attribs);
    button2->bounds = button2->button.btn_rect.getGlobalBounds();
    button2->type = Widget::Button;

    // button 3
    i++;
    Widget* button3 = &menu->page_main[i];
    attribs.text_font = &menu->resources.font_button;
    attribs.position = {650, 400};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    attribs.caption = "BLAHBLAH";
    init_button_widget (&button3->button, attribs);
    button3->type = Widget::Button;
    button3->bounds = button3->button.btn_rect.getGlobalBounds();

    // navigation order
    button1->neighbours.below = button2;
    button2->neighbours.above = button1;

    button2->neighbours.right = button3;
    button3->neighbours.left = button2;
}

static void init_calibrate_page (Menu* menu) {
    int i = 0;
    // add the background widget
    Widget* bg = &menu->page_calibrate[i];
    sf::FloatRect dimensions{0, 0, 1280, 720};
    init_image_widget (bg, dimensions, &menu->resources.texture_menu_background);
    bg->bounds = dimensions;
    bg->id = i;

    // the logo
    i++;
    Widget* logo = &menu->page_calibrate[i];
    dimensions = {0, 0, 819, 144};
    init_image_widget (logo, dimensions, &menu->resources.texture_menu_logo);
    logo->bounds = dimensions;
    logo->image.img_rect.setOrigin ({logo->image.img_rect.getLocalBounds().width / 2, logo->image.img_rect.getLocalBounds().height / 2});
    logo->image.img_rect.setPosition ({bg->image.img_rect.getLocalBounds().width / 2, logo->image.img_rect.getLocalBounds().height / 2});
    logo->id = i;

    // joystics listbox heading text
    i++;
    Widget* list_title = &menu->page_calibrate[i];
    list_title->type = Widget::Label;
    list_title->label.text.setFont (menu->resources.font_button);
    list_title->label.text.setCharacterSize (32);
    list_title->label.text.setString ("GAMEPADS");
    list_title->label.text.setPosition ({50, 200});

    // joysticks listbox
    const float ROW_WIDTH = 500;
    const float ROW_HEIGHT = 42;
    float row_x = 50;
    float row_y = 250;
    i++;
    int first_row_index = i;
    for (int row = 0; row < 8; ++row) {
        Widget* listrow = &menu->page_calibrate[i];
        listrow->type = Widget::ListItem;
        if (row % 2 == 0) {
            listrow->list.button.btn_rect.setFillColor (menu->theme.color_list_bg1);
            listrow->list.fill_color = menu->theme.color_list_bg1;
            listrow->list.button.btn_rect.setOutlineColor (menu->theme.color_list_bg1);
        } else {
            listrow->list.button.btn_rect.setFillColor (menu->theme.color_list_bg2);
            listrow->list.fill_color = menu->theme.color_list_bg2;
            listrow->list.button.btn_rect.setOutlineColor (menu->theme.color_list_bg2);
        }
        listrow->list.button.btn_rect.setSize ({ROW_WIDTH - 4, ROW_HEIGHT - 4});
        listrow->list.button.btn_rect.setPosition ({row_x, row_y + (ROW_HEIGHT * row) });
        listrow->list.button.btn_rect.setOutlineThickness (2);

        listrow->list.button.caption = "ITEM " + std::to_string (row + 1);
        listrow->list.button.text.setString (listrow->list.button.caption);
        listrow->list.button.text.setFont (menu->resources.font_button);
        listrow->list.button.text.setPosition (listrow->list.button.btn_rect.getPosition());
        listrow->bounds = listrow->list.button.btn_rect.getGlobalBounds();

        if (row > 0) {
            listrow->neighbours.above = &menu->page_calibrate[i - 1];
        }
        if (row < 7) {
            listrow->neighbours.below = &menu->page_calibrate[i + 1];
        }
        i++;
    }

    // tell siblings about each other
    for (int row = first_row_index; row < first_row_index + 8; ++row) {
        Widget* listrow = &menu->page_calibrate[row];

        for (int s = first_row_index; s < first_row_index + 8; ++s) {
            Widget* sibling = &menu->page_calibrate[s];
            if (sibling == listrow) continue;
            listrow->list.siblings[listrow->list.number_siblings++] = sibling;
        }
    }

    // auto select first row?
    select_widget (&menu->page_calibrate[ first_row_index]);

    // frame for the listbox (i was incremented last time through the list loop)
    Widget* frame = &menu->page_calibrate[i];
    frame->type = Widget::Frame;
    frame->frame.rect.setFillColor ({0, 0, 0, 0});
    frame->frame.rect.setOutlineColor ({255, 255, 255});
    frame->frame.rect.setOutlineThickness (2);
    frame->frame.rect.setPosition ({row_x - 2, row_y - 2});
    frame->frame.rect.setSize ({ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});

    // button test
    i++;
    Widget* button1 = &menu->page_calibrate[i];
    Button_Attributes attribs;
    attribs.caption = "TEST";
    attribs.text_font = &menu->resources.font_button;
    attribs.dimensions = {ROW_WIDTH / 2 - 8, attribs.dimensions.y};
    attribs.position = {row_x, row_y + ROW_HEIGHT * 8 + 12};
    attribs.fill_color = menu->theme.color_button_std;
    attribs.shadow_color = menu->theme.color_button_shadow;
    attribs.has_shadow = true;
    init_button_widget (&button1->button, attribs);
    button1->bounds = button1->button.btn_rect.getGlobalBounds();
    button1->type = Widget::Button;
    menu->active_widget = button1;

    // button calibrate
    i++;
    Widget* button2 = &menu->page_calibrate[i];
    attribs.caption = "CALIBRATE";
    attribs.position = {attribs.position.x + attribs.dimensions.x + 8, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (&button2->button, attribs);
    button2->bounds = button2->button.btn_rect.getGlobalBounds();
    button2->type = Widget::Button;

    button1->neighbours.right = button2;
    button2->neighbours.left = button1;

    button1->neighbours.above = &menu->page_calibrate[ first_row_index + 7];
    button2->neighbours.above = &menu->page_calibrate[ first_row_index + 7];
    menu->page_calibrate[ first_row_index + 7].neighbours.below = button1;

    // other side of the page...

    // frame for the gamepad widget
    i++;
    frame = &menu->page_calibrate[i];
    frame->type = Widget::Frame;
    frame->frame.rect.setFillColor ({0, 0, 0, 0});
    frame->frame.rect.setOutlineColor ({255, 255, 255});
    frame->frame.rect.setOutlineThickness (2);
    frame->frame.rect.setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});
    frame->frame.rect.setSize ({ROW_WIDTH + 2, (ROW_HEIGHT * 8) + 2});

    // gamepad widget
    i++;
    Widget* gamepad = &menu->page_calibrate[i];
    init_gamepad_widget (gamepad, menu);
    gamepad->gamepad.background.setPosition ({row_x - 2 + ROW_WIDTH + 150, row_y - 2});

    gamepad->gamepad.left_stick.setPosition ({75, 70});
    gamepad->gamepad.right_stick.setPosition ({292, 155});

    gamepad->gamepad.left_stick.move (gamepad->gamepad.background.getPosition());
    gamepad->gamepad.right_stick.move (gamepad->gamepad.background.getPosition());

    // button done
    i++;
    Widget* button_done = &menu->page_calibrate[i];
    attribs.caption = "DONE";
    attribs.dimensions = {ROW_WIDTH - 8, attribs.dimensions.y};
    attribs.position = {row_x + ROW_WIDTH + 150, row_y + ROW_HEIGHT * 8 + 12};
    init_button_widget (&button_done->button, attribs);
    button_done->bounds = button_done->button.btn_rect.getGlobalBounds();
    button_done->type = Widget::Button;
    disable_widget (button_done);
}

static void update_active_animation (Menu* menu, Active_Widget_Animation* animation) {
    if (++animation->ticks >= animation->ticks_per_flash) {
        animation->state = !animation->state;
        animation->ticks = 0;
        if (animation->state) {
            activate_widget (menu->active_widget);
        } else {
            deactivate_widget (menu->active_widget);
        }
    }
}

static void change_active_widget (Menu* menu, Widget* active_widget, Active_Widget_Animation* animation) {
    if (widget_disabled (active_widget)) return;
    deactivate_widget (menu->active_widget);
    activate_widget ( (active_widget));
    animation->ticks = 0;
    menu->active_widget = active_widget;
}

static int run_menu (Menu* menu, sf::RenderWindow* window) {
    Mouse             mouse;
    Keyboard          keyboard;
    GamepadController gamepad;
    Active_Widget_Animation active_animation;
    int ticks = 0;
    bool mouse_mode = false;

    init_resources (menu);
    init_main_page (menu);
    init_calibrate_page (menu);

    while (!menu->should_exit) {
        ticks++;
        // handle input
        menu->event = Menu_Event::NOTHING;
        if (window->hasFocus()) {

            handle_mouse (&mouse, menu, window);
            handle_keyboard (&keyboard, menu);
            handle_gamepad (&gamepad, menu);
            handle_window (window, menu);

            switch (menu->event) {
            case Menu_Event::UP:
                if (menu->active_widget->neighbours.above) {
                    change_active_widget (menu, menu->active_widget->neighbours.above, &active_animation);
                }
                break;
            case Menu_Event::DOWN:
                if (menu->active_widget->neighbours.below) {
                    change_active_widget (menu, menu->active_widget->neighbours.below, &active_animation);
                }
                break;
            case Menu_Event::RIGHT:
                if (menu->active_widget->neighbours.right) {
                    change_active_widget (menu, menu->active_widget->neighbours.right, &active_animation);
                }
                break;
            case Menu_Event::LEFT:
                if (menu->active_widget->neighbours.left) {
                    change_active_widget (menu, menu->active_widget->neighbours.left, &active_animation);
                }
                break;
            case Menu_Event::FIRE:
                global::log (get_widget_caption (menu->active_widget));
                select_widget (menu->active_widget);
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
        mouse_mode  = false;
        if (mouse.moved || mouse.pressed) {
            mouse_mode = true;
        }
        int i = 0;
        while (menu->active_page[i].type != Widget::Anonymous) {
            if (mouse_mode) {
                if (menu->active_page[i].interactive && has_mouse (&menu->active_page[i], mouse.position)
                        && menu->active_widget != &menu->active_page[i]
                   ) {
                    change_active_widget (menu, &menu->active_page[i], &active_animation);
                }
            }
            update_widget (&menu->active_page[i++], menu);
        }
        update_active_animation (menu, &active_animation);

        // render
        window->clear (sf::Color::Magenta);
        i = 0;
        while (menu->active_page[i].type != Widget::Anonymous) {
            draw_widget (&menu->active_page[i++], window);
        }
        window->display();
    }
    return menu->return_code;
}

} // namespace menu
} // namespace ss
