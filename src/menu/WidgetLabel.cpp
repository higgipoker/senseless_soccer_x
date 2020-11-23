#include "WidgetLabel.hpp"

namespace ss {
Label::Label (sf::RenderWindow& wnd, const sf::Font& f, const float size, const sf::Color& col) : Widget (wnd), color(col) {
    text.setFont (f);
    text.setCharacterSize(size);
}

void Label::draw_self() {
    window.draw (text);
}

void Label::setText(const std::string& t){
    text.setString(t);
}
void Label::setPosition (const sf::Vector2f& p) {
    text.setPosition (p);
}

void Label::setSize (const sf::Vector2f& s) {
}

void Label::setAlpha (const sf::Uint8 a) {
    color.a = a;
    text.setFillColor(color);
}

void Label::onHighlight() {}
void Label::onUnHighlight() {}
}
