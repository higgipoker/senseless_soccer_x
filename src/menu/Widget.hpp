#pragma once
#include "events.hpp"
#include "Menu.hpp"
#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
namespace ss{
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
    virtual void update();
    void draw();
    virtual void onPressed() {
    }
    virtual void setPosition (const sf::Vector2f& p) = 0;
    virtual void setSize (const sf::Vector2f& s) = 0;
    virtual void onHighlight() = 0;
    virtual void onUnHighlight() = 0;
    virtual void setAlpha (const sf::Uint8 a);
    virtual void onDisable() {
        enabled = false;
    }
    virtual void onEnable() {
        enabled = true;
    }
    bool enabled = true;
    bool has_mouse = false;

    struct {
        Widget* above = nullptr;
        Widget* below = nullptr;
        Widget* right = nullptr;
        Widget* left = nullptr;
    } neighbours;

    int id = -1;

protected:
    Menu* context = nullptr;
    std::vector<std::unique_ptr<Widget>> children;
    sf::RenderWindow& window;
    bool highlighted = false;
    virtual void draw_self() = 0;
    virtual void update_self() {};
};
}


