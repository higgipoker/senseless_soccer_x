#pragma once
#include <SFML/Graphics/Drawable.hpp>

namespace ss {
class Entity;
class Renderable : public sf::Drawable{
public:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
    sf::RenderStates render_states;
};
} // namespace ss
