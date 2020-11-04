#pragma once
#include "widget.h"

namespace ss {
class Page : public Widget {
public:
    Page (sf::RenderWindow& wnd) :  Widget (wnd) {};
    virtual ~Page() final = default;
    virtual void setPosition (const sf::Vector2f& p) override;
    virtual void setSize (const sf::Vector2f& s) override;
    void onHighlight() override;
    void onUnHighlight() override;
protected:
    void draw_self() override;
};
}
