#pragma once
#include "../input/controller_tools.hpp"
#include "../math/vector.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>


namespace ss {
namespace drawables {

static constexpr float LEFT_STICK_X = 132;
static constexpr float LEFT_STICK_Y = 104;
static constexpr float RIGHT_STICK_X = 326;
static constexpr float RIGHT_STICK_Y = 184;

struct AnalogStick {
    void init (const sf::Vector2f& pos) {
        position = pos;

        // init outer circle
        outer.setOrigin ({outer.getLocalBounds().width / 2, outer.getLocalBounds().height / 2});
        outer.setPosition (position);
        outer.setFillColor (sf::Color::Black);
        outer.setOutlineColor (sf::Color::Black);
        outer.setOutlineThickness (2);

        // init inner circle
        inner.setOrigin ({inner.getLocalBounds().width / 2, inner.getLocalBounds().height / 2});
        inner.setPosition (position);
        inner.setFillColor (sf::Color::White);

        // init origin
        origin.setOrigin ({origin.getLocalBounds().width / 2, origin.getLocalBounds().height / 2});
        origin.setPosition (position);
        origin.setFillColor (sf::Color::White);
    }
    void setPosition (const sf::Vector2f& pos) {
        outer.setPosition (pos + position);
        inner.setPosition (pos + position);
        origin.setPosition (pos + position);

    }
    void update (const sf::Vector3f& stick_vector) {
        if (greater_than_or_equal (vec_magnitude2d (stick_vector), 0.9f)) {
            outer.setFillColor (sf::Color::Red);
        } else {
            outer.setFillColor (sf::Color::Black);
        }

        sf::Vector3f normalized = vec_normalized (stick_vector);
        sf::Vector2f dir{normalized.x, normalized.y};
        sf::Vector2f end = inner.getPosition()  + dir * AnalogStick::VECTOR_LENGTH * vec_magnitude2d (stick_vector);

        line[0].position = inner.getPosition();
        line[1].position = end;

        origin.setPosition (line[1].position);
    }
    sf::CircleShape outer{30, 30};
    sf::CircleShape inner{15, 15};
    sf::CircleShape origin{32, 32};
    sf::Vector2f position;

    sf::VertexArray line {sf::LineStrip, 2};
    static constexpr float VECTOR_LENGTH = 25;
};

class DrawableGamepad : public sf::Drawable {
public:
    DrawableGamepad();
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override;
    ControllerState* controller_state = nullptr;
    void setPosition (const sf::Vector2f& pos);
    void update();
private:
    AnalogStick left_stick;
    AnalogStick right_stick;
    sf::RectangleShape bg;
    sf::Texture bg_texture;
    sf::Texture thumbstick_texture;
    void draw_analog_stick (const AnalogStick& stick, const sf::Vector3f& stick_vector, sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Transformable transformable;

};
} // namespace drawables
} // namespace drawables
