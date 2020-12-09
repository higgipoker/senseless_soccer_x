#include "controller_tools.hpp"
#include "../utils/utils.h"
#include <SFML/Window/Joystick.hpp>
#include <iostream>
#include <cmath>
#include <cassert>

namespace ss {
    
void open_calibration_file(files::File& file){
    
}    
    
bool load_from_file (std::map<int, Calibration>& calibrations, files::File& file) {
    calibrations.clear();
    struct Entry {
        std::string id, key, value;
    } ;
    std::vector<Entry> entries;
    for (auto& line : file.lines) {
        if (line.empty()) continue;
        if (line.length() >= 2 && line.rfind ("//", 0) == 0) continue;
        auto tokens = utils::split_string (line, ':');
        assert (tokens.size() == 3);
        Entry entry{tokens[0], tokens[1], tokens[2]};
        entries.push_back (entry);
    }

    for (auto& entry : entries) {
        int id = atoi (entry.id.c_str());

        if (entry.key == "name") {
            calibrations[id].name = entry.value;
        } else if (entry.key == "product") {
            calibrations[id].product_id = atoi (entry.value.c_str());
        } else if (entry.key == "vendor") {
            calibrations[id].vendor_id = atoi (entry.value.c_str());

        } else if (entry.key == "left_dead_inner") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].left_stick.inner_dead_zone = {    static_cast<float> (atof (tokens[0].c_str())),
                                                               static_cast<float> (atof (tokens[1].c_str()))
                                                          };
        } else if (entry.key == "left_outer") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].left_stick.outer_max = {    static_cast<float> (atof (tokens[0].c_str())),
                                                         static_cast<float> (atof (tokens[1].c_str()))
                                                    };
        } else if (entry.key == "right_dead_inner") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].right_stick.inner_dead_zone = {    static_cast<float> (atof (tokens[0].c_str())),
                                                                static_cast<float> (atof (tokens[1].c_str()))
                                                           };
        } else if (entry.key == "right_outer") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].right_stick.outer_max = {    static_cast<float> (atof (tokens[0].c_str())),
                                                          static_cast<float> (atof (tokens[1].c_str()))
                                                     };
        }
    }
    return true;
}

bool write_to_file (const int id, const Calibration& calibration, files::File& file) {
    // a bit inelegant here, we read in the file, check if a calibratin if this id exists and remove it, add this calibration and
    // write out the file again
    std::map<int, Calibration> calibrations;
    load_from_file (calibrations, file);
    calibrations.erase (id);
    calibrations[id] = calibration;

    files::open (file, file.path, std::fstream::in | std::fstream::out | std::fstream::trunc);

    for (auto& cali : calibrations) {
        files::write_line (file, std::to_string (cali.first) + ":" + "name:" + cali.second.name);
        files::write_line (file, std::to_string (cali.first) + ":" + "product:" + std::to_string (cali.second.product_id));
        files::write_line (file, std::to_string (cali.first) + ":" + "vendor:" + std::to_string (cali.second.vendor_id));

        files::write_line (file, std::to_string (cali.first) + ":" + "left_dead_inner:" + std::to_string (cali.second.left_stick.inner_dead_zone.x) + ", " +
                           std::to_string (cali.second.left_stick.inner_dead_zone.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "left_outer:" + std::to_string (cali.second.left_stick.outer_max.x) + ", " +
                           std::to_string (cali.second.left_stick.outer_max.y));

        files::write_line (file, std::to_string (cali.first) + ":" + "right_dead_inner:" + std::to_string (cali.second.right_stick.inner_dead_zone.x) + ", " +
                           std::to_string (cali.second.left_stick.inner_dead_zone.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "right_outer:" + std::to_string (cali.second.right_stick.outer_max.x) + ", " +
                           std::to_string (cali.second.left_stick.outer_max.y));
    }

    return true;
}
void on_calibration_started (Calibration* cali) {
    cali->calibrating = true;
    cali->left_stick.samples.clear();
    cali->right_stick.samples.clear();

}

void on_calibration_finished (Calibration* cali, int id) {
    cali->calibrating = false;

    //
    // analyze the samples and save
    //
    // left stick
    cali->left_stick.inner_dead_zone = {std::max (cali->left_stick.at_rest_before.x, cali->left_stick.at_rest_after.x), std::max (cali->left_stick.at_rest_before.y, cali->left_stick.at_rest_after.y) };
    cali->left_stick.inner_dead_zone *=2.f; // TODO fix this

    sf::Vector2f biggest {0, 0};
    for (auto& sample : cali->left_stick.samples) {
        if (sample.x > biggest.x) biggest.x = sample.x;
        if (sample.y > biggest.y) biggest.y = sample.y;
    }
    cali->left_stick.outer_max = {biggest.x, biggest.y};
    cali->left_stick.samples.clear();


    // right stick
    biggest = {0,0};
    cali->right_stick.inner_dead_zone = {std::max (cali->right_stick.at_rest_before.x, cali->right_stick.at_rest_after.x), std::max (cali->right_stick.at_rest_before.y, cali->right_stick.at_rest_after.y) };
    cali->right_stick.inner_dead_zone *=2.f; // TODO fix this

    for (auto& sample : cali->right_stick.samples) {
        if (sample.x > biggest.x) biggest.x = sample.x;
        if (sample.y > biggest.y) biggest.y = sample.y;
    }
    cali->right_stick.outer_max = {biggest.x, biggest.y};
    cali->right_stick.samples.clear();


    sf::Joystick::Identification identification = sf::Joystick::getIdentification (id);
    cali->name =  identification.name;
    cali->vendor_id = identification.vendorId;
    cali->product_id = identification.productId;
    files::File file;
    files::open (file, files::working_dir() + "/data/calibration.cfg");
     write_to_file (id, *cali, file);
    files::close (file);

    //
    // calculate ranges based on raw data
    //
    
    std::cout << "successfully calibrated gamepad " << id << std::endl;
}

void on_calibration_cancelled (Calibration* cali) {
    cali->calibrating = false;
    cali->left_stick.samples.clear();
    cali->right_stick.samples.clear();
}


} // namespace
