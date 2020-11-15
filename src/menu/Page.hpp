#pragma once
#include "Widget.hpp"
#include "Animation.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ss {
class Menu;
class Page : public Widget {
public:
    Page (sf::RenderWindow& wnd);
    virtual ~Page();
    void animate_in();
    void animate_out();
    virtual Widget* addChild(std::unique_ptr<Widget> child) override;
    virtual void setPosition (const sf::Vector2f& p) override;
    virtual void setSize (const sf::Vector2f& s) override;
    virtual void setAlpha (const sf::Uint8 a) override;
    void onHighlight() override;
    void onUnHighlight() override;
    Widget* getActiveWidget();
    
    void up();
    void down();
    void left();
    void right();
protected:
    sf::Texture bg_texture;
    sf::Sprite background;
    sf::Texture logo_texture;
    sf::Sprite logo;
    std::vector<Animation*> animations;
    void draw_self() override;
    void update_self() override;
    Widget* active_widget = nullptr;
};
}
