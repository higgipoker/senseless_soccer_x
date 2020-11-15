#pragma once
#include "InputDevice.hpp"

namespace ss {
//
//
//
class Gamepad : public InputDevice {
 public:
  Gamepad();
  void update() override;

 protected:
  int sf_joystick_index = 0;
  float thumbstick_threshold = 50.0f;
};
}  // namespace Senseless
