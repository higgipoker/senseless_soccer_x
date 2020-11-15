#pragma once

namespace ss {
namespace game {
class Match;
class MatchState {
public:
    virtual void start() = 0;
    virtual void step() = 0;
    virtual void stop() = 0;
    virtual bool finished() = 0;
    virtual void changeState (Match& context) = 0;
protected:
};
}
}
