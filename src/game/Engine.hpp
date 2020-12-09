#pragma once

#include "Player.hpp"
#include "Ball.h"
#include "../input/Controller.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/TileMap.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace ss {
namespace engine {

const size_t MAX_SPRITES      = 100;              // arbitrary, tweak later
const size_t MAX_PLAYERS      = 22 + 12 + 2 + 3;  // players + subs + managers + referee/linesmen
const size_t MAX_CONTROLLERS  = 22;               // never need more than one per player

struct MatchEngine {
    bool paused = false;
    std::vector<sprite::Sprite*> sprites; // a vector for sorting depending on draw z

    // resources
    sprite::Sprite               sprite_pool            [MAX_SPRITES];    // sprites
    sprite::Animation            animations             [MAX_SPRITES];    // sprite animations
    Player                       players                [MAX_PLAYERS];    // players
    Controller                   controllers            [MAX_CONTROLLERS];// controllers
    int                          controller_assignments [MAX_CONTROLLERS];// maps entries in controller pool to sprite pool 

    // resource counters
    int used_sprites      = 0;
    int used_animations   = 0;
    int used_players      = 0;
    int used_controllers  = 0

    TileMap pitch;
    Ball    ball;
};

void frame             (MatchEngine* engine, const float dt = 0.01f);
void attach_controller (MatchEngine* engine, const int controller, const int player);
void detatch_controller(MatchEngine* engine, const int controller);
//
// resource acquisition
//
inline int acquire_sprite(MatchEngine* engine){
  // special case acquisition for sprites -> add to the sortable list (std::vector)
  int id = engine->used_sprites;
  engine->sprites.push_back(&engine->sprites[id]);
  engine->used_sprites++;
  return id;
}
inline int acquire_player(MatchEngine* engine){
  return engine->used_players++;
}
inline int acquire_animation(MatchEngine* engine){
  return engine->used_animations++;
}
inline int acquire_controller(MatchEngine* engine){
  return engine->used_controllers++;
}

}// namespace engine
}// namespace ss


