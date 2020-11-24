#pragma once
#include "Widget.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

namespace ss {
class Label : public Widget {
public:
    Label (sf::RenderWindow& wnd, const sf::Font& f, const float size, const sf::Color& col);
    void setText (const std::string& t);
    void setPosition (const sf::Vector2f& p) override;
    void setSize (const sf::Vector2f& s) override;
    void setAlpha (const sf::Uint8 a) override;
    void onHighlight() override;
    void onUnHighlight() override;
protected:
    void draw_self() override;

private:
    std::string label;
    sf::Text text;
    sf::Color color{0, 0, 0, 255};
};
}