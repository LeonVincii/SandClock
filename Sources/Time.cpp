#include "Time.h"

namespace SC
{

Time& Time::instance()
{
    static Time time;
    return time;
}

void Time::update()
{
    m_elapsed = m_clock.restart();
    m_fixedElapsed += m_elapsed;
}

void Time::resetFixedElapsed() { m_fixedElapsed = sf::Time::Zero; }

Time::Time() :
    m_elapsed(sf::Time::Zero),
    m_fixedElapsed(sf::Time::Zero)
{
}

} // namespace SC
