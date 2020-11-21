#pragma once
#include "PlayerState.hpp"

namespace ss {
namespace game {
class Player;
class PlayerStateRun : public PlayerState {
public:
    PlayerStateRun (Player& p);
    void start() override;
    void stop() override;
    void update() override;
    bool finished() override;
    void changeState() override;
private:
    
};

}
}
