#include "TileMap.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace ss {
namespace graphics {
    
bool TileMap::init (const sf::RenderTexture& tex, const sf::IntRect texture_rect, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height, const int blank_tile) {
    texture = tex.getTexture();

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType (sf::Quads);
    m_vertices.resize (width * height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j) {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            if (tileNumber == blank_tile) continue;   // blank

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[ (i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f (i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f ( (i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f ( (i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f (i * tileSize.x, (j + 1) * tileSize.y);

            // find its position in the tileset texture
            int col = tileNumber % (texture_rect.width / tileSize.x);
            int row = tileNumber / (texture_rect.width / tileSize.x);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f (texture_rect.left +  col * tileSize.x, texture_rect.top + row * tileSize.y);
            quad[1].texCoords = sf::Vector2f (texture_rect.left + (col + 1) * tileSize.x, texture_rect.top + row * tileSize.y);
            quad[2].texCoords = sf::Vector2f (texture_rect.left + (col + 1) * tileSize.x, texture_rect.top + (row + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f (texture_rect.left + col * tileSize.x, texture_rect.top + (row + 1) * tileSize.y);
        }

    return true;
}

void TileMap::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &texture;

    // draw the vertex array
    target.draw (m_vertices, states);
}
}
} // namespace senseless

