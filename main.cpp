#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/Game.hpp"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main()
{
    ///Initializare window.
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Game Window", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(100);

    ///Initializare joc.
    Game game;
    int rezultat = game.run(window);
    if(!rezultat)
        std::cout<<"Eroare!";

    return 0;
}
