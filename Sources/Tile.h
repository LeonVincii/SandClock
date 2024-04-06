#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

namespace SC
{

class Scene;

class Tile final : public sf::RectangleShape
{
public:
    inline static constexpr float size = 20.f;
    inline static constexpr float outlineThickness = 2.f;

    enum class Adjacency : int
    {
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft,
    };

    explicit Tile(const sf::Vector2f& pos, const sf::Vector2f& origin, const Scene* scene, const sf::Vector2i& index);

    bool contains(const sf::Vector2f& point) const;

    bool isActive() const { return m_isActive; }
    void setIsActive(bool isActive);

    void update();

    const sf::Vector2i index;

private:
    const Scene* m_scene;
    bool m_isActive;
};

} // namespace SC
