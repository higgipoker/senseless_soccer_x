#include "Engine.hpp"
#include "../menu/Button.hpp"

#include <iostream>
#include <memory>

namespace ss {
namespace game {
    
Engine::Engine (sf::RenderWindow& wnd) : window (wnd) {
}

int Engine::addSprite(const SpriteDefinition& def){
    int return_id = number_sprites++;
    sprites[return_id].init(def);
    return return_id;
}

void Engine::frame() {
    handle_input();
    update();
    draw();
}

void Engine::handle_input () {
    for(size_t i=0;i<number_controllers; ++i){
        controllers[i].update();
    }
    for(size_t i=0; 0<number_controllables; ++i){
        controllables[i].update();
    }
}

void Engine::update() {
    for(size_t i=0; i<number_movables; ++i){
        movables[i].update();
    }
}

void Engine::draw() {
    window.clear(sf::Color::Magenta);
    window.draw (pitch);
    for(size_t i=0; i< number_sprites; ++i){
        animations[i].update();
        sprites[animations[i].anim_def.entity_id].setFrame(animations[i].act_frame);
    }
    for(size_t i=0; i< number_sprites; ++i){
        window.draw(sprites[i]);
    }
    window.display();
}

}// namespace game
}// namespace ss

