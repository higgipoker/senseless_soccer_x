#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
namespace ss {

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool init (const sf::RenderTexture& tex, const sf::IntRect texture_rect, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height, const int blank_tile = -1);
private:
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::VertexArray m_vertices;
    sf::Texture texture;
};

} // namespace senseless
