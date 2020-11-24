#include "ListBox.hpp"
#include "Menu.hpp"
#include "../global.hpp"

namespace ss {
void ListBoxRow::update_self() {
    if (context->mouse_moved) {
        sf::FloatRect hitrect{rect.getGlobalBounds() };
        sf::Vector2f mouse{static_cast<float> (context->mouse_position.x), static_cast<float> (context->mouse_position.y) };
        if (hitrect.contains (mouse)) {
            has_mouse = true;
        } else if (has_mouse) {
            has_mouse = false;
        }
    }
}

ListBox::ListBox (sf::RenderWindow& wnd, const int number_rows, const sf::Vector2f& pos, const sf::Vector2f& dims) : Widget (wnd),
    position (pos), dimensions (dims) {

    for (int i = 0; i < number_rows; ++i) {
        rows.push_back (std::make_unique<ListBoxRow> (wnd));
    }

    background.setFillColor (bg_color);
    background.setOutlineColor (bg_color);
    background.setOutlineThickness (BORDER_WIDTH);
    background.setPosition (position);
    background.setSize (dimensions);

    size_t c = 0;
    size_t r = 0;
    for (auto& row : rows) {

        // rect
        row->rect.setSize ({dimensions.x - BORDER_WIDTH * 2, dimensions.y / rows.size() - BORDER_WIDTH / 2});
        row->rect.setPosition (position.x + BORDER_WIDTH, position.y + row->rect.getSize().y * r + BORDER_WIDTH * r);
        row->rect.setFillColor (row->colors[c]);
        row->rect.setOutlineColor (row->colors[c]);
        row->color = row->colors[c];
        c = 1 - c;
        r++;

        // text
        row->text.setPosition (row->rect.getPosition());
        row->text.setFont (global::Resources::font_std);
        row->text.setCharacterSize (dimensions.y / rows.size() /2);
        row->text.setString ("<EMPTY>");

    }
}

void ListBox::onHighlight() {

}

void ListBox::onUnHighlight() {

}

void ListBox::setPosition (const sf::Vector2f& pos) {

}

void ListBox::setSize (const sf::Vector2f& s) {

}

void ListBox::draw_self() {
    window.draw (background);

}

}

