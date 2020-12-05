#include "controller_tools.hpp"
#include "../utils/utils.h"
#include <iostream>

#include <cassert>

namespace ss {
namespace calibration {
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
        } else if (entry.key == "left_rest_min") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].at_rest_left.min = {    static_cast<float> (atof (tokens[0].c_str())),
                                                     static_cast<float> (atof (tokens[1].c_str()))
                                                };
        } else if (entry.key == "left_rest_max") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].at_rest_left.max = {    static_cast<float> (atof (tokens[0].c_str())),
                                                     static_cast<float> (atof (tokens[1].c_str()))
                                                };
        } else if (entry.key == "left_ext_min") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].extremities_left.min = {    static_cast<float> (atof (tokens[0].c_str())),
                                                         static_cast<float> (atof (tokens[1].c_str()))
                                                    };
        } else if (entry.key == "left_ext_max") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].extremities_left.max = {    static_cast<float> (atof (tokens[0].c_str())),
                                                         static_cast<float> (atof (tokens[1].c_str()))
                                                    };
        } else if (entry.key == "right_rest_min") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].at_rest_right.min = {    static_cast<float> (atof (tokens[0].c_str())),
                                                      static_cast<float> (atof (tokens[1].c_str()))
                                                 };
        } else if (entry.key == "right_rest_max") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].at_rest_right.max = {    static_cast<float> (atof (tokens[0].c_str())),
                                                      static_cast<float> (atof (tokens[1].c_str()))
                                                 };
        } else if (entry.key == "right_ext_min") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].extremities_right.min = {    static_cast<float> (atof (tokens[0].c_str())),
                                                          static_cast<float> (atof (tokens[1].c_str()))
                                                     };
        } else if (entry.key == "right_ext_max") {
            auto tokens = utils::split_string (entry.value, ',');
            calibrations[id].extremities_right.max = {    static_cast<float> (atof (tokens[0].c_str())),
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
        files::write_line (file, std::to_string (cali.first) + ":" + "left_rest_min:" + std::to_string (cali.second.at_rest_left.min.x) + ", " +
                           std::to_string (cali.second.at_rest_left.min.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "left_rest_max:" + std::to_string (cali.second.at_rest_left.max.x) + ", " +
                           std::to_string (cali.second.at_rest_left.max.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "left_extt_min:" + std::to_string (cali.second.extremities_left.min.x) + ", " +
                           std::to_string (cali.second.at_rest_left.min.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "left_extt_max:" + std::to_string (cali.second.extremities_left.max.x) + ", " +
                           std::to_string (cali.second.at_rest_left.max.y));
        
        files::write_line (file, std::to_string (cali.first) + ":" + "right_rest_min:" + std::to_string (cali.second.at_rest_right.min.x) + ", " +
                           std::to_string (cali.second.at_rest_left.min.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "right_rest_max:" + std::to_string (cali.second.at_rest_right.max.x) + ", " +
                           std::to_string (cali.second.at_rest_left.max.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "right_extt_min:" + std::to_string (cali.second.extremities_right.min.x) + ", " +
                           std::to_string (cali.second.at_rest_left.min.y));
        files::write_line (file, std::to_string (cali.first) + ":" + "right_extt_max:" + std::to_string (cali.second.extremities_right.max.x) + ", " +
                           std::to_string (cali.second.at_rest_left.max.y));
    }

    return true;
}

}
}
