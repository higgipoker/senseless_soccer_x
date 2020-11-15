#include "Widget.hpp"
#include "Menu.hpp"
#include <SFML/Window/Mouse.hpp>

namespace ss {
void Widget::setContext (Menu* menu) {
    context = menu;
}
Widget* Widget::addChild (std::unique_ptr<Widget> child) {
    children.push_back (std::move (child));
    return children.back().get();
}
void Widget::update() {
    update_self();
    for (auto& child : children) {
        child->update();
    }
}

void Widget::draw() {
    draw_self();
    for (auto& child : children) {
        child->draw();
    }
}

void Widget::setAlpha(const sf::Uint8 a){
    for(auto& child : children){
        child->setAlpha(a);
    }
}
}
