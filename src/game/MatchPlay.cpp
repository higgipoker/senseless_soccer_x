#include "MatchPlay.hpp"
#include "Match.hpp"

namespace ss {
namespace game {
void MatchPlay::start() {

}

void MatchPlay::step() {

}

void MatchPlay::stop() {

}

bool MatchPlay::finished() {
    return false;
}

void MatchPlay::changeState (Match& context) {
    // TODO: test
    context.current_state = &context.playstate;
}
}
}
