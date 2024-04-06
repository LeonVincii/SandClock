#include "Tile.h"

#include "Input.h"
#include "Scene.h"

namespace
{

const sf::Color gray(25, 25, 25);

} // namespace

namespace SC
{

Tile::Tile(const sf::Vector2f& pos, const sf::Vector2f& origin, const Scene* scene, const sf::Vector2i& index) :
    sf::RectangleShape({ 20.f, 20.f }),
    index(index),
    m_scene(scene),
    m_isActive(false)
{
    setOrigin(origin);
    setPosition(pos);
    setRotation(scene->rotation());
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(-Tile::outlineThickness);
    setIsActive(false);
}

bool Tile::contains(const sf::Vector2f& point) const
{
    const sf::Vector2f posTL = getPosition() - getOrigin();
    const sf::Vector2f posBR = posTL + getSize();

    return posTL.x <= point.x && posTL.y <= point.y && posBR.x >= point.x && posBR.y >= point.y;
}

void Tile::setIsActive(bool isActive)
{
    m_isActive = isActive;

    if (m_isActive)
        setFillColor(sf::Color::Yellow);
    else
        setFillColor(gray);
}

void Tile::update()
{
    if (getRotation() != m_scene->rotation())
        setRotation(m_scene->rotation());
}

} // namespace SC
