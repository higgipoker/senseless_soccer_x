#include "Engine.hpp"
#include "match_resources.hpp"
#include "player_animations.h"
#include "../graphics/Sprite.hpp"
#include "../sfml/sfml_tools.hpp"
#include <iostream>

namespace ss {
namespace game {

struct {
    bool operator() (const sprite::Sprite* a, const sprite::Sprite* b) const {
        return  a->z < b->z;
    }
} sprite_comparitor;

static void sort_sprites(sprite::Sprite* sprites){

}

static void handle_input (MatchEngine* engine){
  for(int i=0; i<engine->number_controllers; ++i){
    update_controller(&engine->controllers[i]);
    if(engine->controller_assignments[i]>=0){
      player::handle_input(&engine->players[engine->controller_assignments[i]], engine->controllers[i]);
    }
  }
}

static void simulate(MatchEngine* engine, const float dt){
  for(int i=0; i<engine->number_players; ++i){
    player::simulate(engine->players[i]);
  }
  ball::simulate(engine->ball);
}

static void draw (MatchEngine* engine, sf::RenderWindow* window){
  engine->window->clear(sf::Color::Red);
  window->draw(engine->pitch);

  sort_sprites(engine->sprites);
  for(auto& sprite : engine->sprites){
    engine->window->draw(*sprite);
  }
}

void frame(MatchEngine* engine, const float dt){
  engine->handle_input(engine);
  engine->simulate(engine, dt);
  engine->draw(engine);
}

void attach_controller(MatchEngine* engine, const int controller, const int player){
  engine->controller_assignments[controller] = player;
}

void detatch_controller(MatchEngine* engine, const int controller){
  engine->controller_assignments[controller] = -1;
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


