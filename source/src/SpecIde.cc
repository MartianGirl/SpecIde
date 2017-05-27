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

        spectrum.clock();
        screen.update();
// #ifdef DEBUG
        // std::cout << "Z80 Address bus: " 
            // << std::hex << std::setfill('0') << std::setw(4) 
            // << spectrum.z80.a << std::endl;
// #endif
    }

    return 0;
}

// vim: et:sw=4:ts=4
