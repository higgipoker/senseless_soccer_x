#pragma once
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "ball.h"
#include "../input/Controller.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/TileMap.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cassert>

namespace ss {
namespace engine {

using namespace sprite;
using namespace graphics;

const size_t MAX_SPRITES      = 100;              // arbitrary, tweak later
const size_t MAX_PLAYERS      = 22 + 12 + 2 + 3;  // players + subs + managers + referee/linesmen
const size_t MAX_CONTROLLERS  = 8;               // never need more than one per player

struct MatchEngine {
    bool paused = false;

    // resources
    struct {
        Movable                      movable               [MAX_SPRITES];    // movable components
        Controllable                 controllable          [MAX_SPRITES];    // controllable components
        sprite::Sprite               drawable              [MAX_SPRITES];    // drawable components
        SpriteAnimation              animation             [MAX_SPRITES];    // sprite animations

    } sprites; // this struct describes a player or a ball
    
    sprite::Sprite*              sorted_sprites             [MAX_SPRITES];    // for sorting depending on draw z
    Controller                   controllers                [MAX_CONTROLLERS];// controllers
    int                          controller_assignments     [MAX_CONTROLLERS];// maps entries in controller pool to sprite pool
    
    // players
    player::Player players[MAX_PLAYERS];
    int used_players = 0;

    // resource counters
    int used_sprites      = 0;
    int used_controllers  = MAX_CONTROLLERS;

    Camera           camera;
    TileMap          pitch_grass;
    TileMap          pitch_lines;
    ball::Ball       ball;
};
void init (MatchEngine* engine);
void frame (MatchEngine* engine, sf::RenderWindow* window, const float dt = 0.01f);
void handle_input (MatchEngine* engine, sf::RenderWindow* window);
void simulate (MatchEngine* engine, const float dt);
void draw (MatchEngine* engine, sf::RenderWindow* window);
void attach_controller (MatchEngine* engine, const int controller, const int player);
void detatch_controller (MatchEngine* engine, const int controller);
//
// resource acquisition
//
static inline int acquire_sprite (MatchEngine* engine) {
    // special case acquisition for sprites -> add to the sortable list
    int id = engine->used_sprites;
    engine->sorted_sprites[id] = &engine->sprites.drawable[id];
    engine->used_sprites++;
    return id;
}
inline int add_player(MatchEngine* engine) {
    int id = acquire_sprite(engine);
    engine->players[engine->used_players].id = id;
    engine->sprites.movable[id].type = Movable::Two_D;
    engine->used_players++;
    return id;
}
}// namespace engine
}// namespace ss



