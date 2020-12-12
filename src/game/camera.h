#pragma once
#include "entity.h"
#include <SFML/Graphics/View.hpp>

namespace ss {
namespace engine {

// ********************
// Camera
// ********************
struct Camera {
    sf::View view;
    Movable movable;
    Movable* target = nullptr;
};

void attach_to(Camera* camera, Movable* movable);
void update(Camera* camera);

}// namespace
}// namesapce
