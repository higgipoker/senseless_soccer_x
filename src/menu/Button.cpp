#include "Button.hpp"
#include "Menu.hpp"
#include "../global.hpp"
#include <filesystem>


namespace ss {

const int TEXT_BUFFER = 5;

Button::Button (sf::RenderWindow& wnd,
                const MenuEvent evt,
                const std::string& capt,
                const sf::Vector2f pos,
                const sf::Color& bg_col,
                const sf::Color& out_col)
    : Widget (wnd), event (evt), bg_color (bg_col), outline_color (out_col), caption (capt) {
    btn_rect.setPosition (pos);
    btn_rect.setSize ({500, 50});
    btn_rect.setFillColor (bg_color);
    btn_rect.setOutlineColor (outline_color);
    btn_rect.setOutlineThickness (outline_size);

    shadow_rect.setPosition (pos);
    shadow_rect.setSize ({500, 50});
    shadow_rect.setFillColor ({0, 0, 0, 100});
    shadow_rect.setOutlineColor ({0, 0, 0, 100});
    shadow_rect.setOutlineThickness (outline_size);
    shadow_rect.move (4, 4);

    text.setFont (global::Resources::font_std);
    text.setString (caption);
    text.setCharacterSize (24);
    text.setFillColor (caption_color);
    text_shadow.setFont (global::Resources::font_std);
    text_shadow.setString (caption);
    text_shadow.setCharacterSize (24);
    text_shadow.setFillColor (sf::Color::Black);
    align_caption();
}

MenuEvent Button::action() {
    global::log ("pressed " + caption);
    return event;
}

void Button::update_self() {
    if (context->mouse_moved) {
        sf::FloatRect hitrect{btn_rect.getGlobalBounds() };
        sf::Vector2f mouse{static_cast<float> (context->mouse_position.x), static_cast<float> (context->mouse_position.y) };
        if (hitrect.contains (mouse)) {
            has_mouse = true;
        } else if (has_mouse) {
            has_mouse = false;
        }
    }
}
void Button::draw_self() {
    window.draw (shadow_rect);
    window.draw (btn_rect);
    window.draw (text_shadow);
    window.draw (text);
}

void Button::setSize (const sf::Vector2f& s) {
    btn_rect.setSize (s);
    align_caption();
}

void Button::setPosition (const sf::Vector2f& p) {
    btn_rect.setPosition (p);
    text.setPosition (btn_rect.getPosition());
    text_shadow.setPosition (btn_rect.getPosition());
    align_caption();
}

void Button::setAlpha (const sf::Uint8 a) {
    sf::Color col = bg_color;
    col.a = a;
    btn_rect.setFillColor (col);

    col = caption_color;
    col.a = a;
    text.setFillColor (col);

    col = sf::Color::Black;
    col.a = a;
    text_shadow.setFillColor (col);


    Widget::setAlpha (a);
}

void Button::setCaption (const std::string& c) {
    caption = c;
    align_caption();
}

void Button::align_caption() {
    size_caption();
    text.setOrigin (text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition ({
        (btn_rect.getPosition().x + btn_rect.getSize().x / 2)
        ,
        (btn_rect.getPosition().y + btn_rect.getSize().y / 2 - text.getCharacterSize() / 4)
    });
    text_shadow.setOrigin (text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text_shadow.setPosition (text.getPosition());
    text_shadow.move ({shadow_offset, shadow_offset});
}

void Button::size_caption() {
    while (text.getGlobalBounds().width >= btn_rect.getGlobalBounds().width - TEXT_BUFFER * 2) {
        caption = caption.substr (0, caption.size() - 3) + ".";
        text.setString (caption);
    }
    text_shadow.setString (caption);
}

void Button::onHighlight() {
    btn_rect.setOutlineColor (sf::Color::White);
    highlighted = true;
}

void Button::onUnHighlight() {
    btn_rect.setOutlineColor (outline_color);
    highlighted = false;
}

void Button::onDisable() {
    Widget::onDisable();
    btn_rect.setFillColor ({bg_color.r, bg_color.g, bg_color.b, 50});
}

void Button::onEnable() {
    Widget::onDisable();
    btn_rect.setFillColor ({bg_color.r, bg_color.g, bg_color.b, 255});
}
}
