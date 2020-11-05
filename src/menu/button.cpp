#include "button.h"
#include <filesystem>
#include <iostream>

namespace ss {

const int TEXT_BUFFER = 5;

Button::Button (sf::RenderWindow& wnd, const sf::Color& bg_col, const sf::Color& out_col) : Widget (wnd), bg_color (bg_col), outline_color (out_col) {
    btn_rect.setSize ({500, 60});
    btn_rect.setFillColor (bg_color);
    btn_rect.setOutlineColor (outline_color);
    btn_rect.setOutlineThickness (outline_size);

    // test font
    std::filesystem::path path (std::filesystem::current_path());
    std::string fontpath = path.string() + "/../fonts/arial_bold.ttf";
    if (!font.loadFromFile (fontpath)) {
        std::cout << "could not load font: " << fontpath << std::endl;
    }

    text.setFont (font);
    text.setString (caption);
    text.setCharacterSize (24);
    text.setFillColor (caption_color);
    text.setOutlineThickness (2);
    text.setOutlineColor (sf::Color::Black);
    align_caption();
}

void Button::draw_self() {
    window.draw (btn_rect);
    window.draw (text);
}

void Button::setSize (const sf::Vector2f& s) {
    btn_rect.setSize (s);
    align_caption();
}

void Button::setPosition (const sf::Vector2f& p) {
    btn_rect.setPosition (p);
    text.setPosition (btn_rect.getPosition());
    align_caption();
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
}

void Button::size_caption() {
    while (text.getGlobalBounds().width >= btn_rect.getGlobalBounds().width - TEXT_BUFFER * 2) {
        caption = caption.substr (0, caption.size() - 3) + ".";
        text.setString (caption);
    }
}

void Button::onHighlight(){
  btn_rect.setOutlineColor(sf::Color::White); 
}

void Button::onUnHighlight(){
    btn_rect.setOutlineColor(sf::Color::Black);
}
}


