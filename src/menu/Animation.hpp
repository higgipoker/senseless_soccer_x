#pragma once
#include "Widget.hpp"

namespace ss {
class Animation {
public:
    virtual ~Animation() = default;
    void changeTarget(Widget* w) {widget = w;}
    virtual void start(){if(widget)running = true;}
    virtual void update() = 0;
    virtual void stop(){running = false;}
    virtual bool finished(){return !running;}
protected:
    Widget* widget;
    bool running = false;
};
}
