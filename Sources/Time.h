#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

#define TIME SC::Time::instance()

namespace SC
{

class Time
{
public:
    static Time& instance();

    sf::Time elapsed() const { return m_elapsed; }
    sf::Time fixedElapsed() const { return m_fixedElapsed; }

    void update();
    void resetFixedElapsed();

private:
    explicit Time();

    sf::Clock m_clock;
    sf::Time m_elapsed;
    sf::Time m_fixedElapsed;
};

} // namespace SC
