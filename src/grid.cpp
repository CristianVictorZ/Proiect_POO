#include "../headers/grid.hpp"

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}

bool Grid::load(const std::string& tileset, const int* tiles)
{
    if (!m_tileset.loadFromFile(tileset))
        return false;

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            int tileNumber = tiles[i + j * ((int) width)];

            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}

sf::Vector2f Grid::getSnapPoint (sf::Vector2f pos, const int* tiles)
{
    if(pos.y > len*height)
        return pos;
    int tileNumber = tiles[(int) (pos.x / len) + ((int) (pos.y / len)) * ((int) width)];
    if(tileNumber != 1)
        return pos;
    sf::Vector2i auxPoint = sf::Vector2i((int) pos.x/len, (int) pos.y/len);
    sf::Vector2f snapPoint = sf::Vector2f((float) (auxPoint.x*len + len/2), (float) (auxPoint.y*len + len/2));
    return snapPoint;
}
