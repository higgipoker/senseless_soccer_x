#pragma once
#include "widget.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace ss {
class Button : public Widget {
public:
    Button (sf::RenderWindow& wnd);
    virtual ~Button() final = default;
    void draw_self() final override;
    void setPosition (const sf::Vector2f& p) override;
    void setSize (const sf::Vector2f& s) override;
    void setCaption (const std::string& c);
private:
    sf::RectangleShape btn_rect;
    sf::Color bg_color = sf::Color::Blue;
    sf::Color outline_color = sf::Color::Black;
    int outline_size = 2;
    std::string caption = "DEFAULT";
    sf::Color caption_color = sf::Color::White;
    sf::Font font;
    sf::Text text;
};
}
