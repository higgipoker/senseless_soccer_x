#pragma once
#include "Controller.hpp"
#include "controller_tools.hpp"

namespace ss {
namespace input {
    
const int MAX_GAMEPADS = 8;
   
void load_calibrations(std::map<int,  Calibration>& calibrations);
void load_gamepads(Controller controllers[]); 

}// namespace
}// namespace
