#pragma once

#include "Player.hpp"
#include "Ball.h"
#include "../menu/Menu.hpp"
#include "../input/Controllable.hpp"
#include "../input/Gamepad.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/TileMap.hpp"

#include <cassert>

namespace ss {
namespace game {
const size_t MAX_ENTITIES  = 100;
const size_t MAX_CONTROLLERS = 30;

class Engine {
public:
    explicit Engine (sf::RenderWindow& wnd);

    int addPlayer ();
    Sprite& getSprite (int id);

    void frame();
    bool paused = false;

private:
    sf::RenderWindow& window;
    TileMap pitch;

    Ball ball;
    std::vector<Sprite*>                        sprites;
    std::array<Sprite, MAX_ENTITIES>            sprite_pool;
    std::array<Player, MAX_ENTITIES>            players;
    std::array<Gamepad, MAX_CONTROLLERS>        controllers;


    size_t number_players = 0;
    size_t number_controllers = 0;

    void get_input();
    void update();
    void draw();
};
}// namespace game
}// namespace ss


