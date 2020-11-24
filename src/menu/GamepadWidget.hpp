#pragma once
#include "Widget.hpp"
#include "../drawables/DrawableGamepad.hpp"
#include "../input/Controller.hpp"

namespace ss {
class GamepadWidget : public Widget {
public:
    GamepadWidget (sf::RenderWindow& wnd);

    void setPosition (const sf::Vector2f& p) override;
    void setSize (const sf::Vector2f& s) override;
    sf::Vector2f getSize() {
        return gamepad.getSize();
    }
    void onHighlight() override;
    void onUnHighlight() override;
    drawables::DrawableGamepad gamepad;
    Controller *controller;
protected:

    void draw_self() override;
};
}
