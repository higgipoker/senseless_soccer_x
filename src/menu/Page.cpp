#include "Page.hpp"
#include "Menu.hpp"
#include "../global.hpp"
#include <filesystem>

namespace ss {

Page::Page (sf::RenderWindow& wnd, Menu* ctx, const Page_ID pid) : Widget (wnd), id (pid) {
    context = ctx;
    std::filesystem::path path (std::filesystem::current_path());
    std::string imagepath = path.string() + "/gfx/bg1.png";
    if (!bg_texture.loadFromFile (imagepath)) {
        global::log ("could not load texture: " + imagepath);
    }
    background.setTexture (bg_texture);

    imagepath = path.string() + "/gfx/logo2.png";
    if (!logo_texture.loadFromFile (imagepath)) {
        global::log ("could not load texture: " + imagepath);
    }
    logo.setTexture (logo_texture);
    logo.setOrigin (logo.getLocalBounds().width / 2, logo.getLocalBounds().height / 2);
    logo.setPosition ({background.getLocalBounds().width / 2, logo.getLocalBounds().height / 2});
}

Page::~Page() {
    for (auto& animation : animations) {
        delete animation;
    }
}
void Page::animate_in() {

}

void Page::animate_out() {

}

Widget* Page::addChild (std::unique_ptr<Widget> child) {
    Widget* added =  Widget::addChild (std::move (child));
    if (children.size() == 1) {
        active_widget = children[0].get();
    }
    added->setContext (context);
    return added;
}
void Page::setPosition (const sf::Vector2f& p) {

}

void Page::setSize (const sf::Vector2f& s) {

}

void Page::setAlpha (const sf::Uint8 a) {
    background.setColor ({255, 255, 255, a});
    logo.setColor ({255, 255, 255, a});
    Widget::setAlpha (a);
}

void Page::update_self() {
    if (context) {
        bool animations_finished = true;
        for (auto& animation : animations) {
            animation->update();
            if (animation->finished()) {
                animations_finished = false;
            }
        }
        if (animations_finished) animations.clear();
        for (auto& child : children) {
            if (context->mouse_mode && child->has_mouse && child->enabled) {
                active_widget = child.get();
            }
        }
    }
}
void Page::draw_self() {
    window.draw (background);
    window.draw (logo);
}
void Page::onHighlight() {

}
void Page::onUnHighlight() {

}
Widget* Page::getActiveWidget() {
    return active_widget;
}
void Page::up() {
    if (active_widget)
        if (active_widget->neighbours.above)
            if (active_widget->neighbours.above->enabled)
                active_widget = active_widget->neighbours.above;
}
void Page::down() {
    if (active_widget)
        if (active_widget->neighbours.below)
            if (active_widget->neighbours.below->enabled)
                active_widget = active_widget->neighbours.below;
}
void Page::left() {
    if (active_widget)
        if (active_widget->neighbours.left)
            if (active_widget->neighbours.left->enabled)
                active_widget = active_widget->neighbours.left;

}
void Page::right() {
    if (active_widget)
        if (active_widget->neighbours.right)
            if (active_widget->neighbours.right->enabled)
                active_widget = active_widget->neighbours.right;
}
}
