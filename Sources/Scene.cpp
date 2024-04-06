#include "Scene.h"

#include "Input.h"
#include "Time.h"
#include "Utils.h"

#include <SFML/Graphics.hpp>
#include <assert.h>

namespace
{

using namespace SC;

Scene::TileMatrix initTiles(const Scene* scene, float padding, int indexOffset = 0)
{
    assert(scene);

    using Row = std::vector<std::shared_ptr<Tile>>;

    const int clockSize = scene->clockSize();
    const sf::Vector2f origin = getWindowOrigin(scene->resolution());
    const sf::Vector2f offset(
        origin.x - Tile::size * clockSize - Tile::outlineThickness,
        origin.y - Tile::size * clockSize - Tile::outlineThickness
    );

    std::vector<Row> tiles;
    tiles.reserve(clockSize);

    for (int r = 0; r < clockSize; ++r)
    {
        Row row;
        row.reserve(clockSize);

        for (int c = 0; c < clockSize; ++c)
        {
            const sf::Vector2f pos = sf::Vector2f(c * Tile::size + padding, r * Tile::size + padding) + offset;
            const sf::Vector2i index(r + indexOffset, c + indexOffset);
            row.push_back(std::make_shared<Tile>(origin, origin - pos, scene, index));
        }

        tiles.push_back(std::move(row));
    }

    return tiles;
}

Scene::TileMatrix initUpperTiles(const Scene* scene) { return std::move(initTiles(scene, Tile::outlineThickness)); }

Scene::TileMatrix initLowerTiles(const Scene* scene)
{
    return std::move(initTiles(scene, Tile::outlineThickness + Tile::size * scene->clockSize(), scene->clockSize()));
}

std::weak_ptr<Tile>
    tryGetTile(const Scene::TileMatrix& upperTiles, const Scene::TileMatrix& lowerTiles, int row, int col)
{
    assert(upperTiles.size() == lowerTiles.size());
    assert(upperTiles.size() == upperTiles[0].size());
    assert(lowerTiles.size() == lowerTiles[0].size());

    const int size = upperTiles.size();

    const bool isUpperTile = row < size;

    if (!isUpperTile)
    {
        row -= size;
        col -= size;
    }

    if (row >= 0 && row < size && col >= 0 && col < size)
        return isUpperTile ? upperTiles[row][col] : lowerTiles[row][col];

    return {};
}

} // namespace

namespace SC
{

Scene::Scene(sf::RenderWindow& window, int clockSize) :
    m_window(window),
    m_clockSize(clockSize),
    m_rotation(45.f),
    m_upperTiles(std::move(initUpperTiles(this))),
    m_lowerTiles(std::move(initLowerTiles(this)))
{
    assert(clockSize > 0);
}

sf::Vector2u Scene::resolution() const { return m_window.getSize(); }

sf::Vector2f Scene::rotatedPosition(const sf::Vector2f& pos) const
{
    const sf::Vector2f origin = getWindowOrigin(resolution());
    const sf::Vector2f relPos = pos - origin;

    float angle = 0.f;

    if (relPos.x == 0.f)
        angle = relPos.y < 0.f ? -90.f : 90.f;
    else
    {
        const float radian = std::atanf(relPos.y / relPos.x);
        angle = radianToDegree(relPos.x > 0 ? radian : radian - M_PI);
    }

    const float oldAngle = angle;

    if (angle < -180.f)
        angle += 360.f;
    else if (angle > 180.f)
        angle -= 360.f;

    const float angleRot = angle - m_rotation;
    const float radianRot = degreeToRadian(angleRot);
    const float magnitude = std::sqrt(std::pow(relPos.x, 2) + std::pow(relPos.y, 2));
    const sf::Vector2f relPosRot(magnitude * std::cos(radianRot), magnitude * std::sin(radianRot));

    return relPosRot + origin;
}

std::weak_ptr<Tile> Scene::adjacentTile(const sf::Vector2i& tileIndex, Tile::Adjacency adjacency) const
{
    Tile::Adjacency rotatedAdj = static_cast<Tile::Adjacency>(
        (static_cast<int>(adjacency) - (static_cast<int>(std::round(m_rotation / 45.f) - 1)) + 8) % 8
    );

    switch (rotatedAdj)
    {
    case Tile::Adjacency::Top:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x - 1, tileIndex.y - 1);
    case Tile::Adjacency::TopRight:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x - 1, tileIndex.y);
    case Tile::Adjacency::Right:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x - 1, tileIndex.y + 1);
    case Tile::Adjacency::BottomRight:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x, tileIndex.y + 1);
    case Tile::Adjacency::Bottom:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x + 1, tileIndex.y + 1);
    case Tile::Adjacency::BottomLeft:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x + 1, tileIndex.y);
    case Tile::Adjacency::Left:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x + 1, tileIndex.y - 1);
    case Tile::Adjacency::TopLeft:
        return tryGetTile(m_upperTiles, m_lowerTiles, tileIndex.x, tileIndex.y - 1);
    }

    return {};
}

void Scene::addActiveTileIndex(const sf::Vector2i& index)
{
    m_activeTileIndices.push_back(index);
    sort(
        m_activeTileIndices.begin(), m_activeTileIndices.end(),
        [](const sf::Vector2i& a, const sf::Vector2i& b) { return a.x + a.y < b.x + b.y; }
    );
    m_activeTileIndices.erase(
        unique(m_activeTileIndices.begin(), m_activeTileIndices.end()), m_activeTileIndices.end()
    );
}

void Scene::update()
{
    if (INPUT.isKeyPressed(sf::Keyboard::Key::D) || INPUT.isKeyPressed(sf::Keyboard::Key::Right))
        m_rotation += 90.f * TIME.elapsed().asSeconds();
    else if (INPUT.isKeyPressed(sf::Keyboard::Key::A) || INPUT.isKeyPressed(sf::Keyboard::Key::Left))
        m_rotation -= 90.f * TIME.elapsed().asSeconds();
    else if (INPUT.isKeyPressed(sf::Keyboard::Key::Space) || INPUT.isKeyPressed(sf::Keyboard::Key::Numpad0))
        m_rotation = 45.f;

    if (m_rotation >= 360.f)
        m_rotation -= 360.f;

    auto iterateAndProcessTiles = [this](TileMatrix& tiles)
    {
        for (std::vector<std::shared_ptr<Tile>>& row : tiles)
        {
            for (std::shared_ptr<Tile>& tile : row)
            {
                tile->update();

                if (INPUT.isMousePressed(sf::Mouse::Left) && tile->contains(rotatedPosition(INPUT.mousePosition())))
                {
                    tile->setIsActive(true);
                    addActiveTileIndex(tile->index);
                }

                m_window.draw(*tile.get());
            }
        }
    };

    iterateAndProcessTiles(m_upperTiles);
    iterateAndProcessTiles(m_lowerTiles);
}

void Scene::fixedUpdate()
{
    auto updateTile = [this](const sf::Vector2i& oldIndex, const sf::Vector2i& index)
    {
        if (std::shared_ptr<Tile> oldTile = tryGetTile(m_upperTiles, m_lowerTiles, oldIndex.x, oldIndex.y).lock())
            oldTile->setIsActive(false);
        if (std::shared_ptr<Tile> tile = tryGetTile(m_upperTiles, m_lowerTiles, index.x, index.y).lock())
            tile->setIsActive(true);
    };

    for (sf::Vector2i& index : m_activeTileIndices)
    {
        const sf::Vector2i oldIndex = index;

        std::shared_ptr<Tile> bottom = adjacentTile(index, Tile::Adjacency::Bottom).lock();
        std::shared_ptr<Tile> bottomLeft = adjacentTile(index, Tile::Adjacency::BottomLeft).lock();
        std::shared_ptr<Tile> bottomRight = adjacentTile(index, Tile::Adjacency::BottomRight).lock();

        if (bottom && !bottom->isActive())
        {
            index = bottom->index;
            updateTile(oldIndex, index);
            continue;
        }

        std::vector<std::shared_ptr<Tile>> bottomLeftAndBottomRight = { bottomLeft, bottomRight };
        auto& bottomLeftOrBottomRight = randomOne<std::shared_ptr<Tile>>(bottomLeftAndBottomRight);

        if (bottomLeftOrBottomRight && !bottomLeftOrBottomRight->isActive())
        {
            index = bottomLeftOrBottomRight->index;
            updateTile(oldIndex, index);
            continue;
        }

        auto& theOtherOne = bottomLeftOrBottomRight == bottomLeft ? bottomRight : bottomLeft;
        if (theOtherOne && !theOtherOne->isActive())
        {
            index = theOtherOne->index;
            updateTile(oldIndex, index);
            continue;
        }
    }
}

} // namespace SC
