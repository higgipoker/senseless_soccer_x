#pragma once
#include "../graphics/SpriteAnimation.hpp"
namespace ss {
namespace game {
namespace player_animations {
static const int ID_STAND = 0;
static inline AnimationDefinition animation_stand() {
    AnimationDefinition def;
    def.loop = true;
    def.frames_per_frame = 1;
    def.number_frames = 1;
    def.frames[0] = 0;
    return def;
}
static const int ID_RUN = 1;
static inline AnimationDefinition animation_run() {
    AnimationDefinition def;
    def.loop = true;
    def.frames_per_frame = 8;
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
