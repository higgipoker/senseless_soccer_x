#include "input.hpp"
#include "../files/files.h"

namespace ss {
namespace input {

void load_calibrations (std::map<int,  Calibration>& calibrations) {
    files::File file;
    files::open (file, files::working_dir() + "/data/calibration.cfg");
    load_from_file (calibrations, file);
    files::close (file);
}

void load_gamepads (Controller controllers[]) {
    std::map<int, Calibration> calibrations;
    load_calibrations(calibrations);
    for (int i = 0; i < MAX_GAMEPADS; ++i) {
        controllers[i].setId (i);

        // check for a calibration profile
        sf::Joystick::Identification id = sf::Joystick::getIdentification (i);

        for (auto& cali : calibrations) {
            if (cali.second.name == id.name && cali.second.vendor_id == id.vendorId && cali.second.product_id == id.productId) {
                std::cout << "found calibration for " << id.name.toAnsiString() << std::endl;
               controllers[i].calibrate (&cali.second);
            }
        }
    }
}
}// namespace
}// namespace
