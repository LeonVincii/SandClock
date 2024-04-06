#include "Input.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <assert.h>

namespace SC
{

Input& Input::instance()
{
    static Input input;
    return input;
}

void Input::init(const sf::RenderWindow* window)
{
    assert(window);

    if (m_window)
        return;

    m_window = window;
}

bool Input::isMousePressed(sf::Mouse::Button button) const { return sf::Mouse::isButtonPressed(button); }

sf::Vector2f Input::mousePosition() const
{
    assert(m_window);

    const sf::Vector2i pos = sf::Mouse::getPosition(*m_window);

    return { static_cast<float>(pos.x), static_cast<float>(pos.y) };
}

bool Input::isKeyPressed(sf::Keyboard::Key key) const { return sf::Keyboard::isKeyPressed(key); }

Input::Input() :
    m_window(nullptr)
{
}

} // namespace SC
