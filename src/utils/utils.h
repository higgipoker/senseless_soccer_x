#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace ss {
namespace utils {
std::vector<std::string> split_string (const std::string& str, const char delim) {
    std::stringstream ss (str);
    std::string item;
    std::vector<std::string> tokens;

    while (getline (ss, item, delim)) {
        tokens.push_back (item);
    }
    return tokens;
}
}
}
