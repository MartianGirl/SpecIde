#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Spectrum.h"

#include "Screen.h"

#include "config.h"

int main()
{
    std::cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR;
    std::cout << "." << SPECIDE_VERSION_MINOR << std::endl;

    Screen screen(2);
    screen.open();

    Spectrum spectrum;

    bool run = true;
    while (run)
    {
        sf::Event event;
        while (screen.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                run = false;
                break;
            }
        }

        // screen.update();
    }

    return 0;
}

// vim: et:sw=4:ts=4
