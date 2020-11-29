#include "Engine.hpp"
#include "match_resources.hpp"
#include "player_animations.h"
#include "../sfml/sfml_tools.hpp"

#include <iostream>
#include <memory>

namespace ss {
namespace game {

struct {
    bool operator() (const Sprite* a, const Sprite* b) const {
        return  a->z < b->z;
    }
} sprite_comparitor;

Engine::Engine (sf::RenderWindow& wnd) : window (wnd) {
}

int Engine::addPlayer () {
    int id = number_players++;
    sprite_pool[id].init (MatchResources::getPlayerSpriteDef());
    Sprite* to_add = &sprite_pool[id];
    sprites.push_back (to_add);
    return id;
}

Sprite* Engine::getSprite (size_t id) {
    assert (id < sprites.size());
    return &sprite_pool[id];
}

Player* Engine::getPlayer (size_t id) {
    assert (id < number_players);
    return &players[id];
}

int Engine::addController() {
    int id = number_controllers++;
    return id;
}

Controller* Engine::getController (size_t id) {
    assert (id < MAX_CONTROLLERS);
    return &controllers[id];
}

void Engine::attachController (const size_t controller_id, const size_t player_id) {
    assert (controller_id < MAX_CONTROLLERS);
    assert (player_id < players.size());
    control_list[controller_id] = player_id;

#ifndef NDEBUG
//    debug_controller.controller_state = &controllers[controller_id].state;
//    debug_controller.setPosition({100,400});
//    debug_controller.setScale(0.5f);
#endif
}

void Engine::frame() {
    get_input();
    update();
    draw();
}

void Engine::get_input () {
    for (size_t i = 0; i < number_controllers; ++i) {
        controllers[i].update();
        // is this controller attached to a player?
        if (std::find (control_list.begin(), control_list.end(), i) != control_list.end()) {
            players[i].handleInput (controllers[i].state);
        }
    }
}

void Engine::update() {
    for (size_t i = 0; i < number_players; ++i) {
        players[i].update();
    }
    ball.update();
}

void Engine::draw() {
    window.clear (sfml::DarkGreen);
    window.draw (pitch);

    // draw sprites
    std::sort (sprites.begin(), sprites.end(), sprite_comparitor);
    for (auto& sprite : sprites) {
        sprite->animate();
        window.draw (*sprite);
    }

// ****************************************************************
#ifndef NDEBUG
    for (auto& primitive : primitives) {
        window.draw (*primitive);
    }
    primitives.clear();
//     debug_controller.update();
//     window.draw(debug_controller);
#endif
// ****************************************************************

    window.display();
}

}// namespace game
}// namespace ss


