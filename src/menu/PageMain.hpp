#pragma once
#include "Page.hpp"

namespace ss {

class PageMain : public Page {
public:
    PageMain (sf::RenderWindow& wnd, Menu* ctx);
    void handleButtonPress (Widget* button) override;
private:

};
}


