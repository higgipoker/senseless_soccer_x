#include "camera.h"
#include <iostream>

namespace ss {
namespace engine {
    
void attach_to (Camera* camera, Movable* movable) {
    camera->target = movable;
}

void update (Camera* camera) {
    if (camera->target) {
        camera->movable.movable2.position = position (camera->target);
    }
    camera->view.setCenter ({camera->movable.movable2.position.x, camera->movable.movable2.position.y});

    if (camera->view.getCenter().x - camera->view.getSize().x / 2 < 0) {
        camera->view.setCenter (camera->view.getSize().x/2, camera->view.getCenter().y);
    }
}
}// namespace
}// namespace
