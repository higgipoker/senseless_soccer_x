#pragma once
#include <SFML/Graphics/Font.hpp>
#include <filesystem>
#include <iostream>

namespace ss {
namespace global {

static inline std::string graphicsPath() {
    std::filesystem::path path (std::filesystem::current_path());
    return path.string() + "/gfx/";
}

static inline std::string fontPath() {
    std::filesystem::path path (std::filesystem::current_path());
    return path.string() + "/fonts/";
}

static inline void log(const std::string& message){
   std::cout << message << std::endl;
}

class Resources {
public:
    static void load() {
        if (!font_std.loadFromFile (global::fontPath() + "swos2.ttf")) {
            std::cout << "could not load font: " << global::fontPath() + "swos2.ttf" << std::endl;
        }
    }
    static inline sf::Font font_std;
};

}
}
