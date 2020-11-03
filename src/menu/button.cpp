#include "button.h"
#include <filesystem>
#include <iostream>

namespace ss {
Button::Button (sf::RenderWindow& wnd) : Widget (wnd) {
    btn_rect.setSize ({120, 60});
    btn_rect.setFillColor (bg_color);
    btn_rect.setOutlineColor (outline_color);
    btn_rect.setOutlineThickness (outline_size);

    // test font
    std::filesystem::path path (std::filesystem::current_path());
    std::string fontpath = path.string() + "/fonts/arial_bold.ttf";
    if (!font.loadFromFile (fontpath)) {
        std::cout << "could not load font: " << fontpath << std::endl;
    }

    text.setFont (font);
    text.setString (caption);
    text.setCharacterSize (24);
    text.setFillColor (caption_color);
    text.setPosition (btn_rect.getPosition());

}
void Button::draw_self() {
    window.draw (btn_rect);
    window.draw (text);
}

void Button::setSize (const sf::Vector2f& s) {
    btn_rect.setSize (s);
}

void Button::setPosition (const sf::Vector2f& p) {
    btn_rect.setPosition (p);
    text.setPosition(btn_rect.getPosition());
}

void Button::setCaption (const std::string& c) {
    caption = c;
}
}

