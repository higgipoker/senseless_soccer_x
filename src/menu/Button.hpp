#pragma once
#include "Widget.hpp"
#include "events.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace ss {

class Button : public Widget {
public:
    Button (sf::RenderWindow& wnd,
            const MenuEvent evt,
            const std::string& capt,
            const sf::Vector2f pos,
            const sf::Color& bg_col = sf::Color::Blue,
            const sf::Color& out_col = sf::Color (200, 200, 200, 50));
    virtual ~Button() = default;
    MenuEvent action() override;
    void setPosition (const sf::Vector2f& p) override;
    void setSize (const sf::Vector2f& s) override;
    void setCaption (const std::string& c);
    void setAlpha (const sf::Uint8 a) override;
    void onHighlight() override;
    void onUnHighlight() override;
    MenuEvent event = MenuEvent::None;
private:
    sf::RectangleShape btn_rect;
    sf::RectangleShape shadow_rect;
    sf::Color bg_color = sf::Color::Blue;
    sf::Color outline_color = sf::Color::Black;
    int outline_size = 3;
    std::string caption = "CAPTION";
    sf::Color caption_color = sf::Color::White;
    sf::Font font;
    sf::Text text;
    sf::Text text_shadow;
    const float shadow_offset = 3;
    void draw_self() final override;
    void update_self() final override;
    void align_caption();
    void size_caption();
};
}
