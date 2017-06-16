#include <chrono>
#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "Spectrum.h"
#include "Screen.h"

#include "config.h"

using namespace std;

int main()
{
    using namespace std::this_thread;
    using namespace std::chrono;

    cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR;
    cout << "." << SPECIDE_VERSION_MINOR << endl;

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

    high_resolution_clock::time_point tick = high_resolution_clock::now();
    high_resolution_clock::time_point tock = high_resolution_clock::now();
    stringstream ss;

    size_t busyTime;
    // This is faster than "while(true)".
    for(;;)
    {
        if (screen.done)
            return 0;

        spectrum.clock();
        if (screen.update())
        {
            tock = high_resolution_clock::now();
            sleep_until(tick + milliseconds(20));
            busyTime = duration_cast<microseconds>(tock - tick).count() / 200;
            ss << "SpecIDE - " << busyTime << "%";
            screen.window.setTitle(ss.str());
            ss.str("");
            tick = high_resolution_clock::now();
        }
    }
}

// vim: et:sw=4:ts=4
