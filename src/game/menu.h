#pragma once
#include "gamestate.h"

namespace ss {
class Menu : public Gamestate {
public:
    Menu(sf::RenderWindow& wnd);
    void start() override;
    void end() override;
protected:
    void handle_input(const sf::Event& event) override;
    void update() override;
    void draw() override;
};
}
