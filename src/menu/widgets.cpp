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
std::string get_widget_caption (Widget* widget, Menu* menu) {
    std::string capt;
    switch (widget->type) {
    case Widget::Label:
        return widget->label.text.getString();
    case Widget::Button:
        return menu->object_pool.labels[widget->button.text].getString();
    case Widget::Frame:
        break;
    case Widget::ListItem:
        return menu->object_pool.labels[widget->list.button.text].getString();
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

void init_button_widget (Widget* widget, Menu* menu, const Button_Attributes& attribs) {
    // colors
    widget->button.fill_color = acquire_color(menu, attribs.button_colors[idx_fill]);


    // button rect
    widget->button.btn_rect   = acquire_rect(menu, attribs.geometry[idx_dimensions]);
    menu->object_pool.rects[widget->button.btn_rect].setPosition(attribs.geometry[idx_position]);
    menu->object_pool.colors[widget->button.fill_color] = attribs.button_colors[idx_fill];
    menu->object_pool.rects[widget->button.btn_rect].setOutlineColor (attribs.button_colors[idx_outline]);
    menu->object_pool.rects[widget->button.btn_rect].setOutlineThickness (2); // TODO style themea

    // shadow rect
    widget->button.shadow_rect   = acquire_rect(menu, attribs.geometry[idx_dimensions]);
    menu->object_pool.rects[widget->button.shadow_rect].setPosition(attribs.geometry[idx_position]);
    menu->object_pool.rects[widget->button.shadow_rect].setFillColor (attribs.button_colors[idx_shadow]);
    menu->object_pool.rects[widget->button.shadow_rect].setOutlineColor (attribs.button_colors[idx_outline]);
    menu->object_pool.rects[widget->button.shadow_rect].setOutlineThickness (2); // TODO style themea
    menu->object_pool.rects[widget->button.shadow_rect].move ({4, 4});


    widget->button.text_color = acquire_color(menu, attribs.caption_colors[idx_fill]);

    // caption
    widget->button.text = acquire_label(menu, attribs.caption);
    menu->object_pool.labels[widget->button.text].setFillColor (attribs.caption_colors[idx_fill]);
    menu->object_pool.labels[widget->button.text].setFont (*attribs.text_font);
    menu->object_pool.labels[widget->button.text].setOutlineThickness (2); // TODO themes
    menu->object_pool.labels[widget->button.text].setOutlineColor (attribs.caption_colors[idx_outline]);

    // size the caption
    sf::Text caption = menu->object_pool.labels[widget->button.text];
    sf::RectangleShape rect = menu->object_pool.rects[widget->button.btn_rect];
    while (caption.getGlobalBounds().width >= menu->object_pool.rects[widget->button.btn_rect].getGlobalBounds().width - 12) {
        sf::String  tmp = menu->object_pool.labels[widget->button.text].getString().substring (0, menu->object_pool.labels[widget->button.text].getString().getSize() - 3) + ".";
        menu->object_pool.labels[widget->button.text].setString (tmp);
    }

    // align the caption
    menu->object_pool.labels[widget->button.text].setOrigin (menu->object_pool.labels[widget->button.text].getLocalBounds().width / 2, menu->object_pool.labels[widget->button.text].getLocalBounds().height / 2);
    menu->object_pool.labels[widget->button.text].setPosition ({
        (attribs.geometry[idx_position].x + attribs.geometry[idx_dimensions].x / 2)
        ,
        (attribs.geometry[idx_position].y + attribs.geometry[idx_dimensions].y / 2 - menu->object_pool.labels[widget->button.text].getCharacterSize() / 4)
    });

    widget->button.text_shadow = acquire_label(menu, menu->object_pool.labels[widget->button.text].getString());
    menu->object_pool.labels[widget->button.text_shadow] = menu->object_pool.labels[widget->button.text];
    menu->object_pool.labels[widget->button.text_shadow].setFillColor({0,0,0,100}); // todo color from theme
    menu->object_pool.labels[widget->button.text_shadow].move ({3, 2});

    init_widget (widget, menu->object_pool.labels[widget->button.text].getString(), menu->object_pool.labels[widget->button.text].getGlobalBounds());
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
    // we have to cast to a pointer because of c++ constness (this pointer passed to memeber fucntion of sf::rect)
    sf::RectangleShape* rect         = const_cast<sf::RectangleShape*>( &menu->object_pool.rects[widget->button.btn_rect]);
    sf::Text* text                   = const_cast<sf::Text*>( &menu->object_pool.labels[widget->button.text]);

    if(widget_enabled(widget)) {
        // widget is enabled, set default colors
        rect->setFillColor(menu->object_pool.colors[widget->button.fill_color]);
        rect->setOutlineColor(menu->object_pool.colors[widget->button.fill_color]);
        text->setFillColor(menu->object_pool.colors[widget->button.text_color]);

        if(widget_active(widget)) {
            // the list row is active, set the highlighted outline (todo, get outline color from menu theme)
            rect->setOutlineColor({255, 255, 255, 255});
        }

    } else {
        // widget is disabled, set transparent colors
        sf::Color trans_fill = menu->object_pool.colors[widget->button.fill_color];
        trans_fill.a = 90;

        sf::Color trans_text = menu->object_pool.colors[widget->button.text_color];
        trans_text.a = 90;

        rect->setFillColor(trans_fill);
        rect->setOutlineColor(trans_fill);
        text->setFillColor(trans_text);
    }

}

void update_list_widget (Widget* widget, const Menu* menu) {
    // we have to cast to a pointer because of c++ constness (this pointer passed to memeber fucntion of sf::rect)
    sf::RectangleShape* rect  = const_cast<sf::RectangleShape*>( &menu->object_pool.rects[widget->list.button.btn_rect]);
    sf::Text* text            = const_cast<sf::Text*>( &menu->object_pool.labels[widget->list.button.text]);

    if(widget_active(widget)) {
        // the list row is active, set the highlighted outline (todo, get outline color from menu theme)
        rect->setOutlineColor({255, 255, 255, 255});
    } else {
        // not active, check for disabled
        if(widget_enabled(widget)) {
            // widget is enabled, set default cols
            rect->setFillColor(menu->object_pool.colors[widget->list.button.fill_color]);
            rect->setOutlineColor(menu->object_pool.colors[widget->list.button.fill_color]);
            text->setFillColor(menu->theme.color_button_text);
        } else {
            // widget is disabled, set transparend cols
            sf::Color trans_fill = menu->object_pool.colors[widget->list.button.fill_color];
            trans_fill.a = 30;

            sf::Color trans_text = menu->object_pool.colors[widget->list.button.text_color];
            trans_text.a = 30;

            rect->setFillColor(trans_fill);
            rect->setOutlineColor(trans_fill);
            text->setFillColor(trans_text);
        }

        // check if the list row is selected
        if(widget_selected(widget)) {
            // widget is selected, set selected fill color
            rect->setFillColor(menu->theme.color_button_ext);
        } else {
            rect->setFillColor(widget->list.fill_color);
        }
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

void draw_label (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->label.text, *states);
}

void draw_button (const Widget* widget,  Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    if(widget_enabled(widget))
        window->draw (menu->object_pool.rects[widget->button.shadow_rect],    *states);
    window->draw (menu->object_pool.rects[widget->button.btn_rect],       *states);
    window->draw (menu->object_pool.labels[widget->button.text_shadow],    *states);
    window->draw (menu->object_pool.labels[widget->button.text],           *states);
}

void draw_frame (const Widget* widget, Menu* menu,sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->frame.rect);
}

void draw_image (const Widget* widget, Menu* menu,sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (widget->image.img_rect, *states);
}

void draw_listrow (const Widget* widget, Menu* menu,sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (menu->object_pool.rects[widget->list.button.shadow_rect],    *states);
    window->draw (menu->object_pool.rects[widget->list.button.btn_rect],       *states);
    window->draw (menu->object_pool.labels[widget->list.button.text_shadow],    *states);
    window->draw (menu->object_pool.labels[widget->list.button.text],           *states);

}

void draw_gamepad (const Widget* widget, Menu* menu,sf::RenderWindow* window, const sf::RenderStates* states) {
    //assert (widget->controller_state); // the widget must be injected with the controller state data

    // draw the background
    window->draw (widget->gamepad.background, *states);

    // draw the thumbsticks
    window->draw (widget->gamepad.left_stick, *states);
    window->draw (widget->gamepad.right_stick, *states);

    // draw the dpad

    // draw the buttons
}

void draw_widget (const Widget* widget, Menu* menu,sf::RenderWindow* window) {
    sf::RenderStates states;
    states.transform = widget->transformable.getTransform();

    switch (widget->type) {
    case Widget::Label:
        draw_label (widget, menu, window, &states);
        break;
    case Widget::Button:
        draw_button (widget, menu, window, &states);
        break;
    case Widget::Frame:
        draw_frame (widget, menu, window, &states);
        break;
    case Widget::ListItem:
        draw_listrow (widget, menu, window, &states);
        break;
    case Widget::Gamepad:
        draw_gamepad (widget, menu, window, &states);
        break;
    case Widget::Image:
        draw_image (widget, menu, window, &states);
        break;
    case Widget::Anonymous:
        global::log ("TRYING TO DRAW AN ANONYMOUS WIDGET");
        assert (false);
        break;
    }
}

void attach_controller (Gamepad_Widget* widget, ControllerState* state) {
    widget->controller_state = state;
}

} // namespace
} // namespace

