#include "Engine.hpp"
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

int Engine::addEntity (const SpriteDefinition& sprite_def) {
    int id = number_entities++;
    sprite_pool[id].init (sprite_def);
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
    for (size_t i = 0; i < number_entities; ++i) {
        players[i].update();
    }
    ball.update();
}

void Engine::draw() {
    window.clear (sf::Color::Magenta);
    window.draw (pitch);
    
    // update animations
    for (size_t i = 0; i < number_entities; ++i) {
        animations[i].update();
        sprite_pool[i].setFrame(animations[i].act_frame);
    }

    // draw sprites
    std::sort (sprites.begin(), sprites.end(), sprite_comparitor);
    for (auto& sprite : sprites) {
        window.draw (*sprite);
    }
    window.display();
}

}// namespace game
}// namespace ss

