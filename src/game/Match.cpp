#include "Match.hpp"
#include "Player.hpp"
#include "player_animations.h"
#include <iostream>

namespace ss {
namespace game {

void add_player(Player& player, Engine& engine){
    player.id = engine.addPlayer();
    player.sprite = &engine.getSprite(player.id);
    player.sprite->setPosition({100, 100});
    player.sprite->setActiveAnimation(0);
    player.sprite->setRotation(-45);
  }

Match::Match (sf::RenderWindow& wnd) : engine (wnd) {
   MatchResources::init();
   Player player1;
   add_player(player1, engine);
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
