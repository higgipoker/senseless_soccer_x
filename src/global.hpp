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

static inline sf::IntRect to_int_rect(const sf::FloatRect& rect){
    return {    static_cast<int>(rect.left),
                static_cast<int>(rect.top),
                static_cast<int>(rect.width),
                static_cast<int>(rect.height)
    };    
}

static inline sf::Vector2u to_int_vector(const sf::Vector2f& vec){
    return {static_cast<unsigned int>(vec.x), static_cast<unsigned int>(vec.y)};
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
