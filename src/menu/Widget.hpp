#pragma once
#include "events.hpp"
#include "Menu.hpp"
#include "../input/controller_tools.hpp"
#include "../resources/Resources.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <memory>
#include <bitset>
#include <cassert>

namespace ss {
namespace widget {

enum class   Widget_Type  {Anonymous, Button, ListItem, Gamepad};
enum class   Widget_State {Idle, Active, Disabled, Selected};
const size_t WIDGET_STATES = static_cast<size_t> (Widget_State::Selected) + 1;

struct Widget_Resources {
    // resources needed to draw a gamepad
    sf::Texture gamepad_texture;
    sf::Texture thumbstick_texture;
    
} widget_resources;


struct Widget {

    // basic info
    int                         id = 0;
    Widget_Type                 type = Widget_Type::Anonymous;
    std::bitset<WIDGET_STATES>  states;
    sf::Transformable           transformable;

    // drawable things
    sf::FloatRect               geometry;
    sf::RectangleShape          drawshape;
    sf::RectangleShape          drawshape_shadow;
    sf::Text                    text;
    sf::Text                    text_shadow;

    // colors
    sf::Color                   fill_color;
    sf::Color                   outline_color;
    sf::Color                   text_color;

    // navigation order
    struct {
        int above = 0;
        int below = 0;
        int right = 0;
        int left = 0;
    } neighbours;

    // pointer to any data the widget might need to connect to
    void* data = nullptr;
};

static void init_widget_resources() {
    widget_resources.thumbstick_texture.loadFromFile (resources::gfx_folder() + "gamepad_bg.png");

}

static void init_gamepad_widget (Widget& widget) {
    widget.drawshape.setSize({599, 450});
    widget.drawshape.setTexture(&idget_resources.gamepad_texture);
}

static void update_widget (Widget& widget) {

}

static bool has_mouse (const Widget& widget, const Menu& menu) {
    if (menu->mouse_moved) {
        sf::Vector2f mouse{static_cast<float> (menu->mouse_position.x), static_cast<float> (menu.mouse_position.y) };
        if (widget.geometry.contains (mouse)) {
            has_mouse = true;
        } else if (has_mouse) {
            has_mouse = false;
        }
    }
}

static void draw_button (const Widget& widget,  sf::RenderWindow& window, const sf::RenderStates& states) {
    window.draw (widget.drawshape_shadow);
    window.draw (widget.drawshape);
    window.draw (widget.text_shadow);
    window.draw (widget.text);
}

static void draw_gamepad (const Widget& widget, sf::RenderWindow& window, const sf::RenderStates& states) {
    assert (widget.data); // the widget must be injected with the controller state data
    ControllerState* controller_state = static_cast<ControllerState*> (widget.data);

    // draw the background

    // draw the left stick

    // draw the right stick

    // draw the dpad

    // draw the buttons
}

static void draw_widget (const Widget& widget, sf::RenderWindow& window) {
    sf::RenderStates states;
    states.transform = widget.transformable.getTransform();
    
    switch (widget.type) {
    case Widget_Type::Button:
        draw_button (widget, window, states);
        break;
    case Widget_Type::ListItem:
        break;
    case Widget_Type::Gamepad:
        break;
    case Widget_Type::Anonymous:
        break;
    }
}



class Menu;
class Widget {
public:
    Widget (sf::RenderWindow& wnd) : window (wnd) {}
    virtual ~Widget() = default;
    void setContext (Menu* menu);
    int childCount() {
        return children.size();
    }
    virtual Widget* addChild (std::unique_ptr<Widget> child);
    virtual void update();
    void draw();
    virtual void onPressed() {
    }
    virtual void setPosition (const sf::Vector2f& p) = 0;
    virtual void setSize (const sf::Vector2f& s) = 0;
    virtual void onHighlight() = 0;
    virtual void onUnHighlight() = 0;
    virtual void setAlpha (const sf::Uint8 a);
    virtual void onDisable() {
        enabled = false;
    }
    virtual void onEnable() {
        enabled = true;
    }
    bool enabled = true;
    bool has_mouse = false;

    struct {
        Widget* above = nullptr;
        Widget* below = nullptr;
        Widget* right = nullptr;
        Widget* left = nullptr;
    } neighbours;

    int id = -1;

protected:
    Menu* context = nullptr;
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;
    bool highlighted = false;
    virtual void draw_self() = 0;
    virtual void update_self() {};
};
}
}

