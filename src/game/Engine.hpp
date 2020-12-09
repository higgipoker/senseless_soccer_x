#pragma once

#include "Player.hpp"
#include "Ball.h"
#include "../input/Controller.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/TileMap.hpp"
#include "../drawables/DrawableGamepad.hpp"
#include <SFML/Graphics/RenderWindow.hpp>


#include <map>
#include <cassert>

namespace ss {
namespace game {

const size_t MAX_SPRITES = 100;             // arbitrary, tweak later
const size_t MAX_PLAYERS = 22 + 12 + 2 + 3; // players + subs + managers + referee/linesmen
const size_t MAX_CONTROLLERS = 22;          // should never need more than one per player

class Engine {
public:
    explicit Engine (sf::RenderWindow& wnd);

    int             addPlayer ();
    sprite::Sprite*         getSprite (size_t id);
    Player*         getPlayer(size_t id);
    int             addController();
    Controller*     getController(size_t id);
    void            attachController(const size_t controller_id, const size_t player_id);
    void            frame();
    
    bool paused = false;

    
// *******************************************************
#ifndef NDEBUG
    std::vector<std::unique_ptr<sf::Drawable>> primitives;
    //drawables::DrawableGamepad debug_controller;
#endif
 // *******************************************************
    
private:
    sf::RenderWindow&   window;
    TileMap             pitch;
    Ball                ball;
    
    std::vector<sprite::Sprite*>                        sprites;        // sortable sprites for drawing order
    std::array<sprite::Sprite, MAX_SPRITES>     sprite_pool;    // cache friendly sprite pool
    std::array<Player, MAX_PLAYERS>             players;        // cache friendly player pool
    std::array<Controller, MAX_CONTROLLERS>     controllers;    // cache freindly controller pool
    std::array<size_t, MAX_CONTROLLERS>         control_list;   // maps entries in controller pool to sprite pool 
    
    size_t number_players = 0;
    size_t number_controllers = 0;

    void get_input();
    void update();
    void draw();

};
}// namespace game
}// namespace ss


