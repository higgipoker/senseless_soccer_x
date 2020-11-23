#pragma once
#include "Widget.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <array>

namespace ss {

const int NUMBER_ROWS = 8;
const int BORDER_WIDTH = 4;

class ListBoxRow : public Widget {
public:
    ListBoxRow (sf::RenderWindow& wnd) : Widget (wnd) {
        colors[0] = {0, 0, 102};
        colors[1] = {51, 51, 255};
        rect.setOutlineThickness (2);
    }
    void setPosition (const sf::Vector2f& p) override {

    }
    void setSize (const sf::Vector2f& s) override {

    }
    void onHighlight() override {
        rect.setOutlineColor (sf::Color::White);
    }
    void onUnHighlight() override {
        rect.setOutlineColor (color);
    }
    void draw_self() {
        window.draw (rect);
        window.draw (text);
    }
    void update_self() override;

    sf::RectangleShape rect;
    sf::Text text;
    sf::Color color;
    std::array<sf::Color, 2> colors; // TODO static
    sf::Color highlight_color{255, 255, 133}; // TODO static

private:

};

class ListBox : public Widget {
public:
    ListBox (sf::RenderWindow& wnd, const sf::Vector2f& pos, const sf::Vector2f& dims);
    void setPosition (const sf::Vector2f& p) override;
    void setSize (const sf::Vector2f& s) override;
    void onHighlight() override;
    void onUnHighlight() override;
    std::array<std::unique_ptr<ListBoxRow>, NUMBER_ROWS> data;
protected:
    void draw_self() override;

    sf::Vector2f position;
    sf::Vector2f dimensions;
    sf::Color bg_color{sf::Color::White};
    sf::RectangleShape background;


private:

};
}

