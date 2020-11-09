#pragma once
#include "animation.h"

namespace ss {
    class AnimationFlash : public Animation {
    public:
        AnimationFlash(Widget& w) : Animation(w){
        }
        
        void update(){
            if(++ticks >= ticks_per_flash){
                ticks = 0;
                on = !on;
                if(on) widget.onHighlight();
                else widget.onUnHighlight();
            }
        }
        
    private:
        bool on = true;
        int ticks = 0;
        int ticks_per_flash = 20;
    };
}
