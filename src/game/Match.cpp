#include "Match.hpp"
#include "player_animations.h"
#include <iostream>

namespace ss {
namespace game {
Match::Match (sf::RenderWindow& wnd) : engine (wnd) {
    MatchResources::init();
//    engine.addSprite (&MatchResources::match_sprite);
   int player_id = engine.addEntity(MatchResources::getPlayerSpriteDef());
   std::cout << "added player " << player_id << std::endl;
   engine.getSprite(player_id).setPosition({100, 100});
   AnimationDefinition anim_def = player_animations::animation_run();
   anim_def.entity_id = player_id;
   engine.setAnimation(anim_def);
   engine.getSprite(player_id).setRotation(-45);
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
