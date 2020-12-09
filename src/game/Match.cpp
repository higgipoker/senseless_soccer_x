#include "Match.hpp"
#include "Player.hpp"
#include "player_animations.h"
#include "../graphics/Sprite.hpp"
#include "../global.hpp"
#include <SFML/Window/Keyboard.hpp>
namespace ss {
namespace game {

Player* add_player (Engine& engine) {
    int id = engine.addPlayer();
    Player* player = engine.getPlayer (id);
    player->id = id;
    player->sprite = engine.getSprite (player->id);
    player->sprite->setPosition ({100, 100});

    player->sprite->configAnimation (player_animations::ID_STAND, player_animations::animation_stand());
    player->sprite->configAnimation (player_animations::ID_RUN, player_animations::animation_run());
    player->sprite->setActiveAnimation (player_animations::ID_STAND);
    int cid = engine.addController();
//    engine.getController(cid)->type = ControllerType::Keyboard;
    engine.attachController(cid, id);
    return player;
}

Match::Match (sf::RenderWindow& wnd) : engine (wnd) {
    MatchResources::init();
    Player* player1 = add_player (engine);
    global::log("add player with id: " + std::to_string(player1->id));
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
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Escape)) {
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
