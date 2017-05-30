#include <iostream>
#include <iomanip>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Spectrum.h"
#include "Screen.h"

#include "config.h"

int main()
{
    std::cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR;
    std::cout << "." << SPECIDE_VERSION_MINOR << std::endl;

    // Create a Spectrum
    Spectrum spectrum;

    // Create a screen and "connect" it to the Spectrum ULA.
    Screen screen(2);
    screen.setRgbaInput(&spectrum.ula.rgba);
    screen.setVSyncInput(&spectrum.ula.vSync);
    screen.setHSyncInput(&spectrum.ula.hSync);
    screen.setBlankInput(&spectrum.ula.blank);

    // This is faster than "while(true)".
    for(;;)
    {
        sf::Event event;
        while (screen.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                return 0;
            }
        }

        spectrum.clock();
        if (spectrum.z80.a == 0x0038)
            std::cout << "Keyboard interrupt attended." << std::endl;
        if (spectrum.ula.ulaRead)
            std::cout << "Keyboard data provided." << std::endl;
        screen.update();
    }
}

// vim: et:sw=4:ts=4
