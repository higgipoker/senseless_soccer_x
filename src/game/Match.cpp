#include "Match.hpp"
#include "Player.hpp"
#include "player_animations.h"
#include <iostream>

namespace ss {
namespace game {

Player* add_player(Engine& engine){
    int id = engine.addPlayer();
    Player* player = engine.getPlayer(id);
    player->id = id;
    player->sprite = engine.getSprite(player->id);
    player->sprite->setPosition({100, 100});
    player->sprite->setActiveAnimation(0);
    player->sprite->setRotation(-45);
    return player;
  }

Match::Match (sf::RenderWindow& wnd) : engine (wnd) {
   MatchResources::init();
   Player *player1 = add_player(engine);
   std::cout << "add player with id: " << player1->id << std::endl;
}

void Match::init() {
    // set up the players, match parameters etc here
    finished = false;
}

void Match::exit() {
    // do any cleanup, reporting etc here
}

void Match::play() {
    while (!finished) {
        // TODO debug
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) {
            finished = true;
        }
        update();
        if (engine.paused) {
            // do pause menu here
        } else {
            engine.frame();
        }
    }
}

void Match::update() {
    current_state->step();
    if (current_state->finished()) {
        current_state->stop();
        current_state->changeState (*this);
        current_state->start();
    }
}
}// namespace game
}// namespace ss
