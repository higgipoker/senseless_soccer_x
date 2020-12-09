#include "engine.h"
#include "match_resources.hpp"
#include "player_animations.h"
#include "../graphics/Sprite.hpp"
#include "../sfml/sfml_tools.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

namespace ss {
namespace engine {

static void sort_sprites (sprite::Sprite* sprites) {

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
            player::handle_input (&engine->players[engine->controller_assignments[i]], engine->controllers[i].state);
        }
    }
}

static void simulate (MatchEngine* engine, const float dt) {
    // palyers
    for (int i = 0; i < engine->used_players; ++i) {
        player::simulate (&engine->players[i], engine, dt);
    }
    //ball::simulate(engine->ball);
}

static void draw (MatchEngine* engine, sf::RenderWindow* window) {
    window->clear (sf::Color::Red);
    window->draw (engine->pitch);

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


