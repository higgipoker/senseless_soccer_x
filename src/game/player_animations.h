#pragma once
#include "../graphics/SpriteAnimation.hpp"
namespace ss {
namespace game {
namespace player_animations {
static inline AnimationDefinition animation_run() {
    AnimationDefinition def;
    def.loop = true;
    def.frames_per_frame = 4;
    def.number_frames = 7;
    int f = 7;
    for (int i = 0; i < def.number_frames; ++i) {
        def.frames[i] = f;
        f++;
    }
    return def;
}
}
}
}
