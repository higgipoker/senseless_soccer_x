#include "Sprite.hpp"
#include <cassert>

namespace ss {
void Sprite::setFrame (const int frame) {
    assert (frame < MAX_FRAMES);
    setTextureRect (frames[frame]);
}
}
