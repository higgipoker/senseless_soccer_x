#include "Engine.hpp"
#include "match_resources.hpp"
#include "player_animations.h"
#include "../menu/Button.hpp"

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
    sprite_pool[id].configAnimation(0, player_animations::animation_run());
    sprites.push_back (&sprite_pool[id]);
    return id;
}

Sprite& Engine::getSprite (int id) {
    return sprite_pool[id];
}

void Engine::frame() {
    get_input();
    update();
    draw();
}

void Engine::get_input () {
    for (size_t i = 0; i < number_controllers; ++i) {
        controllers[i].update();
    }
}

void Engine::update() {
    for (size_t i = 0; i < number_players; ++i) {
        players[i].update();
    }
    ball.update();
}

void Engine::draw() {
    window.clear (sf::Color::Magenta);
    window.draw (pitch);
    
    // draw sprites
    std::sort (sprites.begin(), sprites.end(), sprite_comparitor);
    for (auto& sprite : sprites) {
        sprite->animate();
        window.draw (*sprite);
    }
    window.display();
}

}// namespace game
}// namespace ss

