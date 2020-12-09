#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
namespace ss {
namespace graphics{
class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool init (const sf::RenderTexture&     tex,                // texture with tileset graphics 
               const sf::IntRect            texture_rect,       // location within the texture 
               sf::Vector2u                 tileSize,           // tile dimensions   
               const int*                   tiles,              // the tilemad definition array 
               unsigned int                 width,              // tiles wide 
               unsigned int                 height,             // tiles high 
               const int                    blank_tile = -1     // id of "blank" tile
              );
private:
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::VertexArray m_vertices;
    sf::Texture texture;
};
}
} // namespace senseless
