#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
namespace ss {
class Widget {
public:
    Widget (sf::RenderWindow& wnd) : window (wnd) {}
    virtual ~Widget() = default;
    int childCount(){return children.size();}
    void addChild(std::unique_ptr<Widget> child);
    void draw();
    virtual void setPosition(const sf::Vector2f& p) = 0;
    virtual void setSize(const sf::Vector2f& s) = 0;
    virtual void onHighlight() = 0;
    virtual void onUnHighlight() = 0;
protected:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;
    virtual void draw_self() = 0;
};
}

