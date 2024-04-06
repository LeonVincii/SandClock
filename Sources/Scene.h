#pragma once

#include "Tile.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

namespace SC
{

class Scene

{
public:
    using TileMatrix = std::vector<std::vector<std::shared_ptr<Tile>>>;

    explicit Scene(sf::RenderWindow& window, int clockSize);

    sf::Vector2u resolution() const;

    int clockSize() const { return m_clockSize; }

    float rotation() const { return m_rotation; }

    sf::Vector2f rotatedPosition(const sf::Vector2f& pos) const;

    std::weak_ptr<Tile> adjacentTile(const sf::Vector2i& tileIndex, Tile::Adjacency adjacency) const;

    void addActiveTileIndex(const sf::Vector2i& index);

    void update();

    void fixedUpdate();

private:
    sf::RenderWindow& m_window;
    int m_clockSize;
    float m_rotation;
    TileMatrix m_upperTiles;
    TileMatrix m_lowerTiles;
    std::vector<sf::Vector2i> m_activeTileIndices;
};

} // namespace SC
