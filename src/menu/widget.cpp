#include "widget.h"

namespace ss {
void Widget::addChild (std::unique_ptr<Widget> child) {
    children.push_back (std::move (child));
}
void Widget::draw() {
    draw_self();
    for (auto& child : children) {
        child->draw();
    }
}
}
