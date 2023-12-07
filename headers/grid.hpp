#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef GRID_HPP
#define GRID_HPP

class Grid : public sf::Drawable, public sf::Transformable
{
    const float width = 30, height = 20;
    const unsigned int len = 40;
    const sf::Vector2u tileSize= sf::Vector2u (len, len);
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:

    bool load(const std::string& tileset, const int* tiles);

    sf::Vector2f getSnapPoint (sf::Vector2f pos, const int* tiles);
};

#endif
