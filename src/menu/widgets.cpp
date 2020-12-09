#include "widgets.h"
#include "menu.h"
#include "../global.hpp"
#include "../math/vector.hpp"
#include <algorithm>
#include <cassert>
namespace ss {
namespace menu {


void init_widget (Widget* widget, Menu* menu, const Widget::Type type) {
    widget->type = type;
    set_widget_enabled (widget, true);
    set_widget_visible (widget, true);

    //TODO for now all widgets share the same trasnform
    widget->transform = acquire_transform (menu);
}

std::string get_widget_caption (const Widget* widget, Menu* menu) {

    switch (widget->type) {

    case Widget::Label:
        return label (menu, widget->label.text).getString();
    case Widget::Button:
        return label (menu, widget->button.text).getString();
    case Widget::Frame:
        break;
    case Widget::ListItem:
        return label (menu, widget->list.button.text).getString();
    case Widget::Gamepad:
        break;
    case Widget::Calibrate:
        break;
    case Widget::Test:
        break;
    case Widget::Image:
        break;

    case Widget::Anonymous:
        break;
    }
    return "";
}

bool has_mouse (const Widget* widget, Menu* menu, const sf::Vector2f&  mouse) {
    return (get_widget_bounds (widget, menu).contains (mouse));
}

void init_button_widget (Widget* widget, Menu* menu, const Button_Attributes& attribs) {
    widget->id = attribs.id;
    set_widget_interactive (widget, true);
    // colors
    widget->button.fill_color = acquire_color (menu, attribs.button_colors[Button_Attributes::Fill]);


    widget->button.btn_rect   = acquire_rect (menu, attribs.geometry[Button_Attributes::Dimensions]);
    rect (menu, widget->button.btn_rect).setPosition (attribs.geometry[Button_Attributes::Position]);
    color (menu, widget->button.fill_color) = attribs.button_colors[Button_Attributes::Fill];
    rect (menu, widget->button.btn_rect).setOutlineColor (attribs.button_colors[Button_Attributes::Outline]);
    rect (menu, widget->button.btn_rect).setOutlineThickness (2); // TODO style themea

    // shadow rect
    widget->button.shadow_rect   = acquire_rect (menu, attribs.geometry[Button_Attributes::Dimensions]);
    rect (menu, widget->button.shadow_rect).setPosition (attribs.geometry[Button_Attributes::Position]);
    rect (menu, widget->button.shadow_rect).setFillColor (attribs.button_colors[Button_Attributes::Shadow]);
    rect (menu, widget->button.shadow_rect).setOutlineColor (attribs.button_colors[Button_Attributes::Outline]);
    rect (menu, widget->button.shadow_rect).setOutlineThickness (2); // TODO style themea
    rect (menu, widget->button.shadow_rect).move ({4, 4});

    // caption
    widget->button.text = acquire_label (menu, attribs.caption);
    widget->button.text_color = acquire_color (menu, attribs.caption_colors[Button_Attributes::Fill]);
    label (menu, widget->button.text).setFillColor (attribs.caption_colors[Button_Attributes::Fill]);
    label (menu, widget->button.text).setFont (font (menu, attribs.text_font));
    label (menu, widget->button.text).setOutlineThickness (2); // TODO themes
    label (menu, widget->button.text).setOutlineColor (attribs.caption_colors[Button_Attributes::Outline]);

    // size the caption
    while (label (menu, widget->button.text).getGlobalBounds().width >= rect (menu, widget->button.btn_rect).getGlobalBounds().width - 12) {
        sf::String  tmp = label (menu, widget->button.text).getString().substring (0, label (menu, widget->button.text).getString().getSize() - 3) + ".";
        label (menu, widget->button.text).setString (tmp);
    }

    // align the caption
    label (menu, widget->button.text).setOrigin (label (menu, widget->button.text).getLocalBounds().width / 2, label (menu, widget->button.text).getLocalBounds().height / 2);
    label (menu, widget->button.text).setPosition ({
        (attribs.geometry[Button_Attributes::Position].x + attribs.geometry[Button_Attributes::Dimensions].x / 2)
        ,
        (attribs.geometry[Button_Attributes::Position].y + attribs.geometry[Button_Attributes::Dimensions].y / 2 - label (menu, widget->button.text).getCharacterSize() / 4)
    });

    widget->button.text_shadow = acquire_label (menu, label (menu, widget->button.text).getString());
    label (menu, widget->button.text_shadow) = label (menu, (widget->button.text));
    label (menu, widget->button.text_shadow).setFillColor ({0, 0, 0, 100}); // todo color from theme
    label (menu, widget->button.text_shadow).move ({3, 2});
}

void init_image_widget (Widget* widget, Menu* menu, const sf::Vector2f dimensions, const sf::Texture* texture) {
    widget->image.img_rect = acquire_rect (menu, dimensions);
    rect (menu, widget->image.img_rect).setSize ({dimensions.x, dimensions.y});
    rect (menu, widget->image.img_rect).setTexture (texture);
    set_widget_interactive (widget, false);
}

void init_listrow_widget (Widget* widget, const Button_Attributes& btn_attribs) {

}

void init_gamepad_widget (Widget* widget,  Menu* menu) {
    // background
    widget->gamepad.background = acquire_rect (menu, {500, 320});
    rect (menu, widget->gamepad.background).setTexture (&texture (menu, menu->resources.texture_gamepad));
    rect (menu, widget->gamepad.background).setPosition ({700, 250});
}

void init_thumbstick_calibrate_widget (Widget* widget, Menu* menu) {
    widget->thumbstick_diagnostic.input_circle = acquire_circle (menu, 3);
    widget->thumbstick_diagnostic.outer_rect = acquire_rect (menu, {150, 150});

    circle (menu, widget->thumbstick_diagnostic.input_circle).setOrigin ({3, 3});
    circle (menu, widget->thumbstick_diagnostic.input_circle).setFillColor ({255, 0, 0, 255});
    circle (menu, widget->thumbstick_diagnostic.input_circle).setOutlineColor ({255, 0, 0, 255});
    circle (menu, widget->thumbstick_diagnostic.input_circle).setOutlineThickness (0);

    rect (menu, widget->thumbstick_diagnostic.outer_rect).setOrigin ({75, 75});
    rect (menu, widget->thumbstick_diagnostic.outer_rect).setFillColor ({0, 0, 0, 100});
    rect (menu, widget->thumbstick_diagnostic.outer_rect).setOutlineColor ({255, 255, 255, 255});
    rect (menu, widget->thumbstick_diagnostic.outer_rect).setOutlineThickness (2);
}

void init_thumbstick_test_widget (Widget* widget, Menu* menu) {

    // tmp
    static const float x_val = 1.0f;
    widget->thumbstick_test.input_circle = acquire_circle (menu, 3);
    widget->thumbstick_test.outer_circle = acquire_circle (menu, 50);
    widget->thumbstick_test.colored_circle = acquire_circle (menu, 50);
    widget->thumbstick_test.bg_circle = acquire_circle (menu, 50);
    widget->thumbstick_test.x_circle = acquire_circle (menu, 50 * x_val);

    circle (menu, widget->thumbstick_test.input_circle).setOrigin ({3, 3});
    circle (menu, widget->thumbstick_test.input_circle).setFillColor ({255, 0, 0, 255});
    circle (menu, widget->thumbstick_test.input_circle).setOutlineColor ({255, 0, 0, 255});
    circle (menu, widget->thumbstick_test.input_circle).setOutlineThickness (0);

    circle (menu, widget->thumbstick_test.outer_circle).setOrigin ({50, 50});
    circle (menu, widget->thumbstick_test.outer_circle).setFillColor ({0, 0, 0, 100});
    circle (menu, widget->thumbstick_test.outer_circle).setOutlineColor ({255, 255, 255, 255});
    circle (menu, widget->thumbstick_test.outer_circle).setOutlineThickness (2);

    circle (menu, widget->thumbstick_test.colored_circle).setOrigin ({50, 50});
    circle (menu, widget->thumbstick_test.colored_circle).setFillColor ({0, 0, 0, 0});

    circle (menu, widget->thumbstick_test.bg_circle).setOrigin ({50, 50});
    circle (menu, widget->thumbstick_test.bg_circle).setFillColor (sf::Color::White);

    circle (menu, widget->thumbstick_test.x_circle).setOrigin ({50 * x_val, 50 * x_val});
    circle (menu, widget->thumbstick_test.x_circle).setFillColor ({0, 0, 0, 0});
    circle (menu, widget->thumbstick_test.x_circle).setOutlineColor (sf::Color::White);
    circle (menu, widget->thumbstick_test.x_circle).setOutlineThickness (1);

}

void update_button_widget (Widget* widget, Menu* menu) {
    // we have to cast to a pointer because of c++ constness (this pointer passed to memeber fucntion of sf::rect)
    sf::RectangleShape* btn_rect     = const_cast<sf::RectangleShape*> (&rect (menu, widget->button.btn_rect));
    sf::Text* text                   = const_cast<sf::Text*> (&label (menu, widget->button.text));

    if (widget_enabled (widget)) {
        // widget is enabled, set default colors
        btn_rect->setFillColor (color (menu, widget->button.fill_color));
        btn_rect->setOutlineColor (color (menu, widget->button.fill_color));
        text->setFillColor (color (menu, widget->button.text_color));

        if (widget_active (widget)) {
            // the list row is active, set the highlighted outline (todo, get outline color from menu theme)
            btn_rect->setOutlineColor ({255, 255, 255, 255});
        }

    } else {
        // widget is disabled, set transparent colors
        sf::Color trans_fill = color (menu, widget->button.fill_color);
        trans_fill.a = 90;

        sf::Color trans_text = color (menu, widget->button.text_color);
        trans_text.a = 90;

        btn_rect->setFillColor (trans_fill);
        btn_rect->setOutlineColor (trans_fill);
        text->setFillColor (trans_text);
    }

}

void update_list_widget (Widget* widget, Menu* menu) {
    // we have to cast to a pointer because of c++ constness (this pointer passed to memeber fucntion of sf::rect)
    sf::RectangleShape* btn_rect  = const_cast<sf::RectangleShape*> (&rect (menu, widget->list.button.btn_rect));
    sf::Text* text            = const_cast<sf::Text*> (&label (menu, widget->list.button.text));

    if (widget_active (widget)) {
        // the list row is active, set the highlighted outline (todo, get outline color from menu theme)
        btn_rect->setOutlineColor ({255, 255, 255, 255});
    } else {
        // not active, check for disabled
        if (widget_enabled (widget)) {
            // widget is enabled, set default cols
            btn_rect->setFillColor (color (menu, widget->list.button.fill_color));
            btn_rect->setOutlineColor (color (menu, widget->list.button.fill_color));
            text->setFillColor (menu->theme.color_button_text);
        } else {
            // widget is disabled, set transparend cols
            sf::Color trans_fill = color (menu, widget->list.button.fill_color);
            trans_fill.a = 30;

            sf::Color trans_text = color (menu, widget->list.button.text_color);
            trans_text.a = 30;

            btn_rect->setFillColor (trans_fill);
            btn_rect->setOutlineColor (trans_fill);
            text->setFillColor (trans_text);
        }

        // check if the list row is selected
        if (widget_selected (widget)) {
            // widget is selected, set selected fill color
            btn_rect->setFillColor (menu->theme.color_button_ext);
        } else {
            btn_rect->setFillColor (color (menu, widget->list.fill_color));
        }
    }
}

void update_gamepad_widget (Widget* widget, Menu* menu) {
    if (widget->gamepad.controller_state) {
        // ControllerState* state = widget->gamepad.controller_state;
    }
}

void update_thumbstick_calibrate_widget (Widget* widget, Menu* menu) {
    if (widget->thumbstick_diagnostic.controller_state) {
        auto attached_stick = widget->thumbstick_diagnostic.attached_stick;
        ControllerState* state = widget->thumbstick_diagnostic.controller_state;
        sf::Vector2f raw_data = attached_stick == Thumbstick_Diagnostic_Widget::Left ?  state->left_stick_raw : state->right_stick_raw;

        sf::CircleShape* input_circle = const_cast<sf::CircleShape*> (&circle (menu, widget->thumbstick_diagnostic.input_circle));
        sf::Vector2f display_data = raw_data;
        sf::Vector2f origin = circle (menu, widget->thumbstick_diagnostic.outer_rect).getPosition();
        sf::Vector2f direction = origin + display_data * 0.5f;
        input_circle->setPosition (direction);

        // save the data?
        if (menu->state == Menu_State::State_CalibratePhase2) {
            auto cali = widget->thumbstick_diagnostic.cali;
            std::vector<sf::Vector2f>* samples = attached_stick == Thumbstick_Diagnostic_Widget::Left ? &cali->left_stick.samples : &cali->right_stick.samples;
            if (samples->size() < 1000) {
                samples->push_back (raw_data);
            }
        }
    }
}

void update_thumbstick_test_widget (Widget* widget, Menu* menu) {
    if (widget->thumbstick_test.controller_state) {
        auto attached_stick = widget->thumbstick_test.attached_stick;
        ControllerState* state = widget->thumbstick_test.controller_state;
        sf::Vector2f data = attached_stick == Thumbstick_Test_Widget::Left ?  state->left_stick_vector : state->right_stick_vector;


        sf::CircleShape* outer_circle = const_cast<sf::CircleShape*> (&circle (menu, widget->thumbstick_test.outer_circle));
        sf::CircleShape* input_circle = const_cast<sf::CircleShape*> (&circle (menu, widget->thumbstick_test.input_circle));
        sf::CircleShape* colored_circle = const_cast<sf::CircleShape*> (&circle (menu, widget->thumbstick_test.colored_circle));
//         sf::CircleShape* x_circle = const_cast<sf::CircleShape*> (&circle (menu, widget->thumbstick_test.x_circle));
        float max_mag = outer_circle->getRadius();
        auto input = data * max_mag;

        if (vec_magnitude (input) >= max_mag) {
            vec_set_magnitude (input, max_mag);
            outer_circle->setOutlineColor (sf::Color::Red);
        } else {
            outer_circle->setOutlineColor (sf::Color::White);
        }
        input_circle->setPosition (input);

        auto normalized_mag = vec_magnitude (input) / max_mag;
        sf::Color c{0, 0, 0, 0};
        if (greater_than_or_equal (normalized_mag, 1.0f)) {
            // red zone
            c.r = 255;
            c.a = normalized_mag * 255 * 0.5;

        } else if (greater_than (normalized_mag, 0)) {
            // green zone
            c.g = 255;
            c.a = normalized_mag * 255;

        } else {
            // centered
        }
        colored_circle->setFillColor (c);
    }
}

void set_widget_position (Widget* widget, Menu* menu, const sf::Vector2f& pos) {
    transform (menu, widget->transform).translate (pos);
}

void draw_label (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (label (menu, widget->label.text), *states);
}

void draw_button (const Widget* widget,  Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    if (widget_enabled (widget)) { // don't draw shadow if button is disabled
        window->draw (rect (menu, widget->button.shadow_rect),    *states);
    }
    window->draw (rect (menu, widget->button.btn_rect),        *states);
    window->draw (label (menu, widget->button.text_shadow),    *states);
    window->draw (label (menu, widget->button.text),           *states);
}

void draw_frame (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (rect (menu, widget->frame.rect));
}

void draw_image (const Widget* widget,  Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (rect (menu, widget->image.img_rect), *states);
}

void draw_listrow (const Widget* widget,  Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (rect (menu, widget->list.button.shadow_rect),    *states);
    window->draw (rect (menu, widget->list.button.btn_rect),       *states);
    window->draw (label (menu, widget->list.button.text_shadow),   *states);
    window->draw (label (menu, widget->list.button.text),          *states);
}

void draw_gamepad (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {

    // draw the background
    window->draw (rect (menu, widget->gamepad.background), *states);

    // draw the dpad

    // draw the buttons
}

void draw_thumbstick_calibrate (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (rect (menu, widget->thumbstick_diagnostic.outer_rect),  *states);
    window->draw (circle (menu, widget->thumbstick_diagnostic.input_circle),  *states);
    //window->draw (widget->thumbstick_diagnostic.line, 2, sf::Lines, *states);
}

void draw_thumbstick_test (const Widget* widget, Menu* menu, sf::RenderWindow* window, const sf::RenderStates* states) {
    window->draw (circle (menu, widget->thumbstick_test.bg_circle),     *states);
    window->draw (circle (menu, widget->thumbstick_test.colored_circle), *states);
    window->draw (circle (menu, widget->thumbstick_test.outer_circle),  *states);
    window->draw (circle (menu, widget->thumbstick_test.input_circle),  *states);
//     window->draw (circle (menu, widget->thumbstick_test.x_circle),      *states);
    window->draw (widget->thumbstick_test.line, 2, sf::Lines,           *states);
}

void update_widget (Widget* widget, Menu* menu) {
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
    case Widget::Calibrate:
        update_thumbstick_calibrate_widget (widget, menu);
        break;
    case Widget::Test:
        update_thumbstick_test_widget (widget, menu);
        break;
    case Widget::Image:
        break;

    case Widget::Anonymous:
        global::log ("TRYING TO UPDATE AN ANONYMOUSE WIDGET");
        assert (false);
        break;
    }
}

void draw_widget (const Widget* widget, Menu* menu, sf::RenderWindow* window) {
    if (!widget_visible (widget)) return;
    static sf::RenderStates states;
    states.transform = transform (menu, widget->transform);

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
    case Widget::Calibrate:
        draw_thumbstick_calibrate (widget, menu, window, &states);
        break;
    case Widget::Test:
        draw_thumbstick_test (widget, menu, window, &states);
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

sf::FloatRect get_widget_bounds (const Widget* widget, Menu* menu) {
    switch (widget->type) {

    case Widget::Label:
        return label (menu, widget->label.text).getGlobalBounds();
    case Widget::Button:
        return rect (menu, widget->button.btn_rect).getGlobalBounds();
    case Widget::Frame:
        return rect (menu, widget->frame.rect).getGlobalBounds();
    case Widget::ListItem:
        return rect (menu, widget->list.button.btn_rect).getGlobalBounds();
    case Widget::Gamepad:
        return rect (menu, widget->gamepad.background).getGlobalBounds();
    case Widget::Calibrate:
        return rect (menu, widget->thumbstick_diagnostic.input_circle).getGlobalBounds();
    case Widget::Test:
        return rect (menu, widget->thumbstick_test.outer_circle).getGlobalBounds();
        break;
    case Widget::Image:
        return rect (menu, widget->image.img_rect).getGlobalBounds();

    case Widget::Anonymous:
        global::log ("TRYING TO GET BOUNDS OF ANONYMOUS WIDGET");
        assert (false);
        break;
    }
    static sf::FloatRect bounds;
    return bounds;
}

sf::Vector2f get_widget_position (const Widget* widget, Menu* menu) {
    switch (widget->type) {

    case Widget::Label: {
            auto bounds = label (menu, widget->label.text).getGlobalBounds();
            return {bounds.left, bounds.top};
        }
    case Widget::Button: {
            auto bounds =  rect (menu, widget->button.btn_rect).getGlobalBounds();
            return {bounds.left, bounds.top};
        }
    case Widget::Frame: {
            auto bounds = rect (menu, widget->frame.rect).getGlobalBounds();
            return {bounds.left, bounds.top};
        }
    case Widget::ListItem: {
            auto bounds = rect (menu, widget->list.button.btn_rect).getGlobalBounds();
            return {bounds.left, bounds.top};
        }
    case Widget::Gamepad: {
            auto bounds = rect (menu, widget->gamepad.background).getGlobalBounds();
            return {bounds.left, bounds.top};
        }
    case Widget::Calibrate: {
            return transform (menu, widget->transform).transformPoint (rect (menu, widget->thumbstick_diagnostic.outer_rect).getOrigin());
        }
    case Widget::Test: {
            return transform (menu, widget->transform).transformPoint (circle (menu, widget->thumbstick_test.outer_circle).getOrigin());
        }
    case Widget::Image: {
            auto bounds = rect (menu, widget->image.img_rect).getGlobalBounds();
            return {bounds.left, bounds.top};
        }

    case Widget::Anonymous:
        global::log ("TRYING TO GET BOUNDS OF ANONYMOUS WIDGET");
        assert (false);
        break;
    }
    static sf::Vector2f pos;
    return pos;
}

Widget* get_widget_neighbour (const Widget* widget, const Event trigger) {
    Widget* neighbour = nullptr;
    switch (trigger) {
    case Event::Up:
        neighbour = widget->neighbours.above;
        break;
    case Event::Down:
        neighbour = widget->neighbours.below;
        break;
    case Event::Left:
        neighbour = widget->neighbours.left;
        break;
    case Event::Right:
        neighbour = widget->neighbours.right;
        break;
    default:
        break;
    }
    return neighbour;
}

void attach_controller (Thumbstick_Test_Widget* widget, Thumbstick_Test_Widget::Stick stick, ControllerState* state) {
    widget->attached_stick = stick;
    widget->controller_state = state;
}

void detatch_controller (Thumbstick_Test_Widget* widget) {
    widget->controller_state = nullptr;
}

void attach_controller (Thumbstick_Diagnostic_Widget* widget, Thumbstick_Diagnostic_Widget::Stick stick, ControllerState* state,  Calibration* cali) {
    widget->attached_stick = stick;
    widget->controller_state = state;
    widget->cali = cali;
}

void detatch_controller (Thumbstick_Diagnostic_Widget* widget) {
    widget->controller_state = nullptr;
}

} // namespace
} // namespace
