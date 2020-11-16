#include "Sprite.hpp"
#include <cassert>

namespace ss {
void Sprite::init (const SpriteDefinition& def) {
    assert (def.total_frames <= MAX_FRAMES);
    setTexture (def.texture);

    int x           = def.texture_origin.x;
    int y           = def.texture_origin.y;
    int col         = 0;
    int frame_count = 0;
    while (frame_count < def.total_frames) {
        frames[frame_count].left   = x;
        frames[frame_count].top    = y;
        frames[frame_count].width  = def.frame_width;
        frames[frame_count].height = def.frame_height;

        // next col
        x += def.frame_width;

        // next row
        if (++col == def.spritesheet_cols) {
            x = 0;
            y += def.frame_height;
            col = 0;
        }
        ++frame_count;
    }
    setTextureRect (frames[0]);

    // default set origin to center
    setOrigin (static_cast<float> (def.frame_width / 2), static_cast<float> (def.frame_height / 2));
}
void Sprite::setFrame (const int frame) {
    assert (frame < MAX_FRAMES);
    setTextureRect (frames[frame]);
}
}
