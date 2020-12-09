#include "Sprite.hpp"
#include <cassert>

namespace ss {
namespace sprite {
    
void init (Sprite* sprite, const SpriteDefinition* def) {
    assert (def->total_frames <= MAX_FRAMES);
    sprite->sprite.setTexture(*def->texture);

    int x           = def->texture_origin.x;
    int y           = def->texture_origin.y;
    int col         = 0;
    int frame_count = 0;
    while (frame_count < def->total_frames) {
        sprite->frames[frame_count].left   = x;
        sprite->frames[frame_count].top    = y;
        sprite->frames[frame_count].width  = def->frame_width;
        sprite->frames[frame_count].height = def->frame_height;

        // next col
        x += def->frame_width;

        // next row
        if (++col == def->spritesheet_cols) {
            x = 0;
            y += def->frame_height;
            col = 0;
        }
        ++frame_count;
    }
    sprite->sprite.setTextureRect (sprite->frames[0]);

    // default set origin to center
    sprite->sprite.setOrigin (static_cast<float> (def->frame_width / 2), static_cast<float> (def->frame_height / 2));
}

void set_frame (Sprite* sprite, const int frame) {
    assert (frame < MAX_FRAMES);
    sprite->sprite.setTextureRect (sprite->frames[frame]);
}

void init_animation(Sprite* sprite, const int id, const AnimationDefinition* anim_def){
}

void set_animation(Sprite* sprite, const int id){
//   if(sprite->current_animation){
//     sprite->current_animation->stop();
//   }
//   sprite->current_animation = &sprite->animations[id];
//   sprite->current_animation->start();
}

void animate(Sprite* sprite){
//   if(sprite->current_animation){
//     sprite->current_animation->update();
//     set_frame(sprite, sprite->current_animation->act_frame);
//     if(sprite->current_animation->running == false){
//       sprite->current_animation->stop();
//       sprite->current_animation = nullptr;
//     }
//   }
}

}// namespace
}// namespace
