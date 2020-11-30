#include "widgets.h"
#include "menu.h"
#include "../global.hpp"
#include <cassert>
namespace ss {
namespace menu {


void init_widget (Widget* widget, const std::string& id, const sf::FloatRect& geometry) {
    widget->id = id;
    widget->bounds = geometry;
    set_widget_enabled (widget, true);
}
std::string get_widget_caption (Widget* widget) {
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


bool has_mouse (const Widget* widget, const sf::Vector2f&  mouse) {
    return (widget->bounds.contains (mouse));
}

void init_button_widget (Widget* widget, const Button_Attributes& attribs) {
    widget->button.fill_color = attribs.fill_color;
    widget->button.text_color = attribs.text_color;
    widget->button.btn_rect.setPosition (attribs.position);
    widget->button.btn_rect.setSize (attribs.dimensions);
    widget->button.btn_rect.setFillColor (attribs.fill_color);
    widget->button.btn_rect.setOutlineColor (attribs.outline_color);
    widget->button.btn_rect.setOutlineThickness (2); // TODO style themea


    if (attribs.has_shadow) {
        widget->button.shadow_rect = widget->button.btn_rect;
        widget->button.shadow_rect.setFillColor (attribs.shadow_color);
        widget->button.shadow_rect.setOutlineColor (attribs.shadow_color);
        widget->button.shadow_rect.move ({4, 4});
    }

    widget->button.caption = attribs.caption;
    widget->button.text.setString (attribs.caption);
    widget->button.text.setFillColor (attribs.text_color);
    widget->button.text.setFont (*attribs.text_font);

    if (attribs.has_text_outline) {
        widget->button.text.setOutlineThickness (2); // TODO themes
        widget->button.text.setOutlineColor (attribs.text_outline_color);
    }

    // size the caption
    while (widget->button.text.getGlobalBounds().width >= widget->button.btn_rect.getGlobalBounds().width - 12) {
        widget->button.caption = widget->button.caption.substr (0, widget->button.caption.size() - 3) + ".";
        widget->button.text.setString (widget->button.caption);
    }
    widget->button.text_shadow.setString (widget->button.caption);
    // align the caption
    widget->button.text.setOrigin (widget->button.text.getLocalBounds().width / 2, widget->button.text.getLocalBounds().height / 2);
    widget->button.text.setPosition ({
        (attribs.position.x + attribs.dimensions.x / 2)
        ,
        (attribs.position.y + attribs.dimensions.y / 2 - widget->button.text.getCharacterSize() / 4)
    });
    if (attribs.has_text_shadow) {
        widget->button.text_shadow.setFont (*attribs.text_font);
        widget->button.text_shadow.setOutlineThickness (2);
        widget->button.text_shadow.setFillColor (sf::Color::Black);
        widget->button.text_shadow.setOrigin (widget->button.text.getLocalBounds().width / 2, widget->button.text.getLocalBounds().height / 2);
        widget->button.text_shadow.setPosition (widget->button.text.getPosition());
        widget->button.text_shadow.move ({3, 2});
    }

    init_widget (widget, widget->button.text.getString(), widget->button.btn_rect.getGlobalBounds());
}

void init_image_widget (Widget* widget, const sf::FloatRect dimensions, sf::Texture* texture) {
    widget->type = Widget::Image;
    widget->interactive = false;
    widget->image.img_rect.setPosition ({dimensions.left, dimensions.top});
    widget->image.img_rect.setSize ({dimensions.width, dimensions.height});
    widget->image.img_rect.setTexture (texture);
    init_widget (widget, "image", widget->image.img_rect.getGlobalBounds());
}

void init_listrow_widget (Widget* widget, const Button_Attributes& btn_attribs) {

}

void init_gamepad_widget (Widget* widget, const Menu* menu) {
    widget->type = Widget::Gamepad;

    // background
    widget->gamepad.background.setSize ({500, 320});
    widget->gamepad.background.setTexture (&menu->resources.texture_gamepad);

    // thumbsticks
    widget->gamepad.left_stick.setRadius (39);
    widget->gamepad.right_stick.setRadius (39);
    widget->gamepad.left_stick.setTexture (&menu->resources.texture_thumbstick);
    widget->gamepad.right_stick.setTexture (&menu->resources.texture_thumbstick);
    widget->gamepad.left_stick.setOrigin (widget->gamepad.left_stick.getLocalBounds().width / 2, widget->gamepad.left_stick.getLocalBounds().height / 2);
    widget->gamepad.right_stick.setOrigin (widget->gamepad.right_stick.getLocalBounds().width / 2, widget->gamepad.right_stick.getLocalBounds().height / 2);

    widget->gamepad.left_stick_origin = {112, 110};
    widget->gamepad.right_stick_origin = {330, 192};
    widget->gamepad.left_stick.setPosition ({112, 110});
    widget->gamepad.right_stick.setPosition ({330, 192});

    widget->gamepad.left_stick.move (widget->gamepad.background.getPosition());
    widget->gamepad.right_stick.move (widget->gamepad.background.getPosition());

    init_widget (widget, "gamepad", widget->gamepad.background.getGlobalBounds());
}

void update_button_widget (Widget* widget, const Menu* menu) {
    if (widget_enabled (widget)) {

        widget->button.btn_rect.setFillColor (widget->button.fill_color);
        widget->button.btn_rect.setOutlineColor (widget->button.fill_color);
        widget->button.text.setFillColor (widget->button.text_color);

        if (widget_active (widget)) {
            widget->button.btn_rect.setOutlineColor ({255, 255, 255, 255});
        } else {
            widget->button.btn_rect.setOutlineColor (widget->button.fill_color);
        }

    } else {
        widget->button.btn_rect.setFillColor ({widget->button.fill_color.r, widget->button.fill_color.g, widget->button.fill_color.b, 30});
        widget->button.btn_rect.setOutlineColor ({widget->button.fill_color.r, widget->button.fill_color.g, widget->button.fill_color.b, 30});
        widget->button.text.setFillColor ({widget->button.text_color.r, widget->button.text_color.g, widget->button.text_color.b, 30});
    }
}

void update_list_widget (Widget* widget, const Menu* menu) {
    if (widget_active (widget)) {
        widget->list.button.btn_rect.setOutlineColor ({255, 255, 255, 255});
    } else {
        widget->list.button.btn_rect.setOutlineColor (widget->list.fill_color);
        if (!widget_enabled (widget)) {
            widget->list.button.btn_rect.setFillColor ({widget->list.button.fill_color.r, widget->list.button.fill_color.g, widget->list.button.fill_color.b, 90});
            widget->list.button.btn_rect.setOutlineColor ({widget->list.button.fill_color.r, widget->list.button.fill_color.g, widget->list.button.fill_color.b, 90});
            widget->list.button.text.setFillColor ({widget->list.button.text_color.r, widget->list.button.text_color.g, widget->list.button.text_color.b, 90});
        } else {
            widget->list.button.btn_rect.setFillColor (widget->list.button.fill_color);
            widget->list.button.btn_rect.setOutlineColor (widget->list.button.fill_color);
            widget->list.button.text.setFillColor (menu->theme.color_button_text);
        }
    }
    if (widget_selected (widget)) {
        widget->list.button.btn_rect.setFillColor (menu->theme.color_button_ext);

    } else {
        widget->list.button.btn_rect.setFillColor (widget->list.fill_color);
    }
}

void update_gamepad_widget (Widget* widget, const Menu* menu) {
    if (widget->gamepad.controller_state) {
        ControllerState* state = widget->gamepad.controller_state;
        widget->gamepad.left_stick.setPosition (widget->gamepad.background.getPosition() + widget->gamepad.left_stick_origin + state->left_stick_vector * 50.f);
        widget->gamepad.right_stick.setPosition (widget->gamepad.background.getPosition() +  widget->gamepad.right_stick_origin + state->right_stick_vector * 50.f);
    }
}

void update_widget (Widget* widget, const Menu* menu) {
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
        update_gamepad_widget (widget);
        break;
    case Widget::Image:
        break;
    case Widget::Anonymous:
        global::log ("TRYING TO UPDATE AN ANONYMOUSE WIDGET");
        assert (false);
        break;
    }
}

void draw_label (const Label_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->text, *states);
}

void draw_button (const Button_Widget* widget,  sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->shadow_rect,    *states);
    window->draw (widget->btn_rect,       *states);
    window->draw (widget->text_shadow,    *states);
    window->draw (widget->text,           *states);
}

void draw_frame (const Frame_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->rect);
}

void draw_image (const Image_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->img_rect, *states);
}

void draw_listrow (const ListRow_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    draw_button (&widget->button, window, states);
}

void draw_gamepad (const Gamepad_Widget* widget, sf::RenderWindow* window, const sf::RenderStates* states) {
    //assert (widget->controller_state); // the widget must be injected with the controller state data

    // draw the background
    window->draw (widget->background, *states);

    // draw the thumbsticks
    window->draw (widget->left_stick, *states);
    window->draw (widget->right_stick, *states);

    // draw the dpad

    // draw the buttons
}

void draw_widget (const Widget* widget, sf::RenderWindow* window) {
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

void attach_controller (Gamepad_Widget* widget, ControllerState* state) {
    widget->controller_state = state;
}

} // namespace
} // namespace
