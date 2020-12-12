#include "entity.h"

namespace ss {
namespace engine {

sf::Vector2f position (Movable* movable) {
    switch (movable->type) {
    case Movable::Two_D:
        return movable->movable2.position;
    case Movable::Three_D:
        return {movable->movable3.position.x, movable->movable3.position.y};
        break;
    }

    return {0, 0};
}
}
}
