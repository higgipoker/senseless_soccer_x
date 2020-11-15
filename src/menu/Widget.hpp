#pragma once
#include "events.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
namespace ss {
class Menu;
class Widget {
public:
    Widget (sf::RenderWindow& wnd) : window (wnd) {}
    virtual ~Widget() = default;
    void setContext (Menu* menu);
    int childCount() {
        return children.size();
    }
    virtual Widget* addChild (std::unique_ptr<Widget> child);
    void update();
    void draw();
    virtual MenuEvent action() {
        return MenuEvent::None;
    }
    virtual void setPosition (const sf::Vector2f& p) = 0;
    virtual void setSize (const sf::Vector2f& s) = 0;
    virtual void setAlpha(const sf::Uint8 a);
    virtual void onHighlight() = 0;
    virtual void onUnHighlight() = 0;
    bool has_mouse = false;

    struct {
        Widget* above = nullptr;
        Widget* below = nullptr;
        Widget* right = nullptr;
        Widget* left = nullptr;
    } neighbours;

protected:
    Menu* context = nullptr;
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;
    bool highlighted = false;
    virtual void draw_self() = 0;
    virtual void update_self() {};
};
}

