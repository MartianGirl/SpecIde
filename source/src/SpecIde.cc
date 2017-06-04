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

    // Connect the keyboard.
    screen.setKeyboardPort(&spectrum.ula.z80_a, spectrum.ula.keys);

    // This is faster than "while(true)".
    for(;;)
    {
        spectrum.clock();
        screen.update();
        if (screen.done) return 0;
    }
}

// vim: et:sw=4:ts=4
