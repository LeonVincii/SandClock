#pragma once

#include <SFML/System/Vector2.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <vector>

namespace SC
{

inline sf::Vector2f getWindowOrigin(const sf::Vector2u& windowSize)
{
    return sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
}

inline float radianToDegree(float radian) { return radian * 180 / M_PI; }

inline float degreeToRadian(float degree) { return degree * M_PI / 180; }

template <typename T>
T& randomOne(std::vector<T>& ts)
{
    std::srand(std::time(nullptr));
    return ts[std::rand() % ts.size()];
}

} // namespace SC
