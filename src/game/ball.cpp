#include "ball.h"
#include "engine.h"

namespace ss {
namespace ball {
    
void init (Ball* ball) {
    ball->movable.type = engine::Movable::Three_D;
}

void update (Ball* ball, engine::MatchEngine* engine) {
}

void simulate (Ball* ball, const float dt) {

}

} // namespace
} // namespace
