#pragma once
#include "widget.h"

namespace ss {
class Animation {
public:
    Animation (Widget& w) : widget (w) {
    }
    virtual ~Animation() = default;
    virtual void update() = 0;
protected:
    Widget& widget;
};
}
