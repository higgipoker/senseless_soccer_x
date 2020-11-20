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

const size_t MAX_SPRITES = 250;             // arbitrary, tweak later
const size_t MAX_PLAYERS = 22 + 12 + 2 + 3; // players + subs + managers + referee/linesmen
const size_t MAX_CONTROLLERS = 22;          // should never need more than one per player

class Engine {
public:
    explicit Engine (sf::RenderWindow& wnd);

    int addPlayer ();
    Sprite* getSprite (size_t id);
    Player* getPlayer(size_t id);

    void frame();
    bool paused = false;

private:
    sf::RenderWindow& window;
    TileMap pitch;
    Ball ball;
    std::vector<Sprite*>                        sprites;
    std::array<Sprite, MAX_SPRITES>             sprite_pool;
    std::array<Player, MAX_PLAYERS>             players;
    std::array<Gamepad, MAX_CONTROLLERS>        controllers;

    size_t number_players = 0;
    size_t number_controllers = 0;

    void get_input();
    void update();
    void draw();
};
}// namespace game
}// namespace ss


