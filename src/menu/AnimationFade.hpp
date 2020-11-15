#pragma once
#include "Animation.hpp"

namespace ss {

enum class FadeType {Fade_In, Fade_Out};

class AnimationFade : public Animation {
public:
    void setType(const FadeType ft){
       type = ft; 
    }
    void start() {
        Animation::start();
        alpha = type == FadeType::Fade_In ? 0 : 255;
    }
    void update() {
        if (!running) return;
        if (++ticks >= anim_speed) {
            ticks = 0;
            switch (type) {
            case FadeType::Fade_In:
                alpha += alpha_step;
                if(alpha>=255){
                    alpha = 255;
                    running = false;
                }
                break;

            case FadeType::Fade_Out:
                alpha -= alpha_step;
                if(alpha<=0){
                    alpha = 0;
                    running = false;
                }
                break;
            }
            widget->setAlpha(static_cast<sf::Uint8>(alpha));
        }
    }
    void stop() {
        Animation::stop();
        if (widget) {
            widget->onUnHighlight();
        }
    }

private:
    FadeType type = FadeType::Fade_In;
    int ticks = 0;
    int anim_speed = 1;
    int alpha_step = 10;
    int alpha = 0;
};
}

