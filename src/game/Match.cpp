#include "Match.hpp"
#include <iostream>

namespace ss {
namespace game {
Match::Match (sf::RenderWindow& wnd) : engine (wnd) {
    MatchResources::init();
    engine.addSprite(&MatchResources::match_sprite);

}
void Match::init(){
   // set up the players, match parameters etc here 
    finished = false;
}

void Match::exit(){
  // do any cleanup, reporting etc here   
}

void Match::play() {
    while (!finished) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
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
