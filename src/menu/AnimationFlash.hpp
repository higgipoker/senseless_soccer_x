#pragma once
#include "Animation.hpp"

namespace ss {
class AnimationFlash : public Animation {
public:
    void start() {
        Animation::start();
        on = true;
        widget->onHighlight();
    }
    void update() {
        if (!running) return;
        if (++ticks >= ticks_per_flash) {
            ticks = 0;
            on = !on;
            if (on) widget->onHighlight();
            else widget->onUnHighlight();
        }
    }
    void stop() {
        Animation::stop();
        if (widget) {
            widget->onUnHighlight();
        }
    }

private:
    bool on = true;
    int ticks = 0;
    int ticks_per_flash = 10;
};
}
