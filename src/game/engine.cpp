#include "engine.h"
#include "player.h"
#include "ball.h"
#include "match_resources.hpp"
#include "player_animations.h"
#include "../input/input.hpp"
#include "../math/vector.hpp"
#include "../graphics/Sprite.hpp"
#include "../sfml/sfml_tools.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

namespace ss {
namespace engine {

static void sort_sprites (sprite::Sprite* sprites) {

}

void init (MatchEngine* engine) {
    // gamepads
    std::map<int,  Calibration> calibrations;
    input::load_gamepads (engine->controllers);

    // assigned controllers
    for (size_t i = 0; i < MAX_CONTROLLERS; ++i) {
        engine->controller_assignments[i] = -1;

    }
    // camera
    engine->camera.view.setSize (1280, 720);
    //attach_to(&engine->camera, &player(engine, 0)->movable);
    
}

void add_ball(MatchEngine* engine){
    engine->ball.id = acquire_sprite(engine);
    ball::init(&engine->ball, engine);
}

void handle_input (MatchEngine* engine, sf::RenderWindow* window) {
    // controllables
    sf::Joystick::update();
    for (int i = 0; i < engine->used_sprites; ++i) {
        if (engine->sprites.controllable[i].input == -1) continue;
        // update the controller attached to this controllable
        engine->controllers[engine->sprites.controllable[i].input].update();
        // update the player associated with this entity id
        player::handle_input (&engine->players[i], engine, engine->controllers[engine->sprites.controllable[i].input].state);
    }

}

void simulate (MatchEngine* engine, const float dt) {
    // movables
    for (int i = 0; i < engine->used_sprites; ++i) {
        simulate (&engine->sprites.movable[i], dt);
    }
    // palyers
    for (int i = 0; i < engine->used_players; ++i) {
        player::update (&engine->players[i], engine);
    }
    
    // ball
    ball::update(&engine->ball, engine);
}

void draw (MatchEngine* engine, sf::RenderWindow* window) {

    // TODO test camera
    engine->camera.view.setSize (1280, 720);
    update (&engine->camera);
    window->setView (engine->camera.view);

    // update sprite animations
    for (int i = 0; i < engine->used_sprites; ++i) {
        animate (&engine->sprites.animation[i]);
        if (engine->sprites.animation[i].running) {
            int current_frame = engine->sprites.animation[i].act_frame;
            sf::IntRect rect = engine->sprites.drawable[i].frames[current_frame];
            engine->sprites.drawable[i].sprite.setTextureRect (rect);
        }
    }


    // draw everyting
//    window->clear (sf::Color::Red);
    window->draw (engine->pitch_grass);
    window->draw (engine->pitch_lines);
    sort_sprites (*engine->sorted_sprites);
    for (int i = 0; i < engine->used_sprites; ++i) {
        window->draw (engine->sorted_sprites[i]->sprite);
    }

    // debug
#ifndef NDEBUG
    for (int i = 0; i < engine->used_players; ++i) {
        window->draw (engine->players[i].feet);
    }
#endif

    window->display();
}

void frame (MatchEngine* engine, sf::RenderWindow* window, const float dt) {
    handle_input (engine, window);
    simulate (engine, dt);
    draw (engine, window);
}

void attach_controller (MatchEngine* engine, const int controller, const int player) {
    engine->sprites.controllable[player].input = controller;
}

void detatch_controller (MatchEngine* engine, const int player) {
    engine->sprites.controllable[player].input = -1;
}

}// namespace game
}// namespace ss



