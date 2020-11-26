#pragma once
#include <iostream>
#include <filesystem>
#include <string>

namespace ss {
namespace resources {
inline static std::string gfx_folder() {
    static std::filesystem::path path (std::filesystem::current_path());
    return path.string() + "/gfx/";
}
inline static std::string font_folder() {
    static std::filesystem::path path (std::filesystem::current_path());
    return path.string() + "/fonts/";
}
}
}
