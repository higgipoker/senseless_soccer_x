#pragma once

#include "../input/Controller.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/TileMap.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace ss {
namespace engine {

using namespace sprite;
using namespace graphics;

struct Camera {
   sf::View view; 
};

struct Movable {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f heading;
    sf::Vector2f target_heading;
    sf::Vector2f applied_force;
    float max_speed = 0;
};

struct Movable_Ball {
    sf::Vector3f position;
    sf::Vector3f velocity;
    sf::Vector3f acceleration;
    sf::Vector3f target_heading;
    sf::Vector3f applied_force;
};

const size_t MAX_SPRITES      = 100;              // arbitrary, tweak later
const size_t MAX_PLAYERS      = 22 + 12 + 2 + 3;  // players + subs + managers + referee/linesmen
const size_t MAX_CONTROLLERS  = 22;               // never need more than one per player

struct MatchEngine {
    bool paused = false;
    sprite::Sprite* sprites[MAX_SPRITES]; // for sorting depending on draw z

    // resources
    sprite::Sprite               sprite_pool            [MAX_SPRITES];    // sprites
    SpriteAnimation              animations             [MAX_SPRITES];    // sprite animations
    Movable                      movables               [MAX_PLAYERS];    // players
    Controller                   controllers            [MAX_CONTROLLERS];// controllers
    int                          controller_assignments [MAX_CONTROLLERS];// maps entries in controller pool to sprite pool

    // resource counters
    int used_sprites      = 0;
    int used_animations   = 0;
    int used_movables     = 0;
    int used_controllers  = 0;

    Camera           camera;
    TileMap          pitch_grass;
    TileMap          pitch_lines;
    Movable_Ball     ball;
};

void frame              (MatchEngine* engine, sf::RenderWindow* window, const float dt = 0.01f);
void attach_controller  (MatchEngine* engine, const int controller, const int player);
void detatch_controller (MatchEngine* engine, const int controller);
//
// resource acquisition
//
inline int acquire_sprite (MatchEngine* engine) {
    // special case acquisition for sprites -> add to the sortable list
    int id = engine->used_sprites;
    engine->sprites[id] = &engine->sprite_pool[id];
    engine->used_sprites++;
    return id;
}
inline int acquire_movable (MatchEngine* engine) {
    return engine->used_movables++;
}
inline int acquire_animation (MatchEngine* engine) {
    return engine->used_animations++;
}
inline int acquire_controller (MatchEngine* engine) {
    return engine->used_controllers++;
}
//
// resource access
//
inline Movable* player (MatchEngine* engine, int id) {
    return &engine->movables[id];
}
inline Sprite* sprite(MatchEngine* engine, int id){
    return &engine->sprite_pool[id];
}

}// namespace engine
}// namespace ss


