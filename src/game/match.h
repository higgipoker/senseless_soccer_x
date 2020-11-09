#pragma once
#include "gamestate.h"
#include "../menu/menu.h"
#include "../input/gamepad.h"
#include "entity.h"
#include <SFML/Graphics.hpp>

namespace ss {
namespace game {
const size_t MAX_ENTITIES  = 100;
const size_t MAX_CONTROLLERS = 30;
class Match : public Gamestate{
public:
    explicit Match (sf::RenderWindow& wnd) : Gamestate (wnd) {};
   
    void start() override {};
    void end() override {};
     void handle_input (const sf::Event& evt) override;
     void update() override;
     void draw() override;
private:
    std::array<Movable, MAX_ENTITIES> movables;
    std::array<Renderable, MAX_ENTITIES> renderables;
    std::array<Controllable, MAX_ENTITIES> controllables;
    std::array<Gamepad, MAX_CONTROLLERS> controllers;
};
}// namespace game
}// namespace ss

