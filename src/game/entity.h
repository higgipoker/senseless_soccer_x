#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace ss {
namespace engine {

struct Movable_2d {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f heading;
    sf::Vector2f target_heading;
    sf::Vector2f applied_force;
    float max_speed = 0;
};

struct Movable_3d {
    sf::Vector3f position;
    sf::Vector3f velocity;
    sf::Vector3f acceleration;
    sf::Vector3f target_heading;
    sf::Vector3f applied_force;
};

struct Movable {
    enum {Two_D, Three_D} type = Two_D;

    //union{ // can't have nice things because sfml oo stuff
    Movable_2d movable2;
    Movable_3d movable3;
    // };
};

struct Controllable {
    int input = -1;   
};

void simulate(Movable* movable, const float dt);

sf::Vector2f position (Movable* movable);


} // namespace
} // namespace
