#pragma once
#include <array>

namespace ss {

const int MAX_ANIMATION_FRAMES = 32;

class Sprite;

struct AnimationDefinition {
    int entity_id = 0;
    bool loop = false;
    int frames_per_frame = 0;
    int number_frames = 0;
    std::array<int, MAX_ANIMATION_FRAMES> frames;
};

class SpriteAnimation {
public:
    void init (const AnimationDefinition& def);
    void start();
    void update();
    void stop();
    AnimationDefinition anim_def;
    bool running = false;
    int act_frame = 0;


protected:
    int ticks = 0;
    int current_frame = 0;
};
}
