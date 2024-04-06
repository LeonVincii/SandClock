#include "Input.h"
#include "Scene.h"
#include "Time.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace
{

constexpr float fixedUpdateInterval = .1f;

void handleEvent(sf::RenderWindow& window, sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::Closed:
        window.close();
        break;
    default:
        // Other events are not support yet.
        break;
    }
}

} // namespace

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sand Clock", sf::Style::Default ^ sf::Style::Resize);

    INPUT.init(&window);

    SC::Scene scene(window, 8);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
            handleEvent(window, event);

        TIME.update();

        window.clear();

        scene.update();

        if (TIME.fixedElapsed().asSeconds() >= fixedUpdateInterval || TIME.fixedElapsed() == sf::Time::Zero)
        {
            scene.fixedUpdate();
            TIME.resetFixedElapsed();
        }

        window.display();
    }

    return 0;
}
