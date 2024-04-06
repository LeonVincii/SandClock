#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#define INPUT SC::Input::instance()

namespace sf
{

class RenderWindow;

} // namespace sf

namespace SC
{

class Input
{
public:
    static Input& instance();

    void init(const sf::RenderWindow* window);

    bool isMousePressed(sf::Mouse::Button button) const;
    sf::Vector2f mousePosition() const;

    bool isKeyPressed(sf::Keyboard::Key key) const;

private:
    explicit Input();

    const sf::RenderWindow* m_window;
};

} // namespace SC
