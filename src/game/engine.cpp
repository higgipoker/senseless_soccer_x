#include "engine.h"
#include "player.h"
#include "match_resources.hpp"
#include "player_animations.h"
#include "../math/vector.hpp"
#include "../graphics/Sprite.hpp"
#include "../sfml/sfml_tools.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

namespace ss {
namespace engine {

static void sort_sprites (sprite::Sprite* sprites) {

}

static void handle_input (Movable* movable, const ControllerState* controller) {
}

static void simulate_player (Movable* movable, MatchEngine* engine, const float dt) {
    movable->acceleration =  movable->acceleration + movable->applied_force; // acceleration is applied by controller input
    movable->velocity += movable->acceleration;                              // TODO limit to player top speed attribute
    movable->position = movable->position + movable->velocity;

    // damp very low velocities
    static const float DAMP_VELOCITY = 0.1f;
    if (less_than (vec_magnitude (movable->velocity), DAMP_VELOCITY)) {
        vec_reset (movable->velocity);
    }
    // reset force for next frames input
    vec_reset (movable->applied_force);
}

static void simulate_ball (Movable_Ball* ball) {
}

static void handle_input (MatchEngine* engine, sf::RenderWindow* window) {
    // window events
    static sf::Event event;
    while (window->pollEvent (event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }

    // controllers
    for (int i = 0; i < engine->used_controllers; ++i) {
        engine->controllers[i].update();
        if (engine->controller_assignments[i] >= 0) {
            handle_input (&engine->movables[engine->controller_assignments[i]], &engine->controllers[i].state);
        }
    }
}

static void simulate (MatchEngine* engine, const float dt) {
    // palyers
    for (int i = 0; i < engine->used_movables; ++i) {
        simulate_player (&engine->movables[i], engine, dt);
    }
    simulate_ball (&engine->ball);
}

static void draw (MatchEngine* engine, sf::RenderWindow* window) {
    
    // TODO test camera
 //   engine->camera.view.move(0, 10);
    engine->camera.view.setSize(1280,720);
    engine->camera.view.setCenter(1280/2, 720/2);
    window->setView(engine->camera.view);
    
    
    window->clear (sf::Color::Red);
    window->draw (engine->pitch_grass);
    window->draw (engine->pitch_lines);
    sort_sprites (*engine->sprites);
    for (int i = 0; i < engine->used_sprites; ++i) {
        window->draw (engine->sprites[i]->sprite);
    }
    window->display();
}

void frame (MatchEngine* engine, sf::RenderWindow* window, const float dt) {
    handle_input (engine, window);
    simulate (engine, dt);
    draw (engine, window);
}

void attach_controller (MatchEngine* engine, const int controller, const int player) {
    engine->controller_assignments[controller] = player;
}

void detatch_controller (MatchEngine* engine, const int controller) {
    engine->controller_assignments[controller] = -1;
}

}// namespace game
}// namespace ss


