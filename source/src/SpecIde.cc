#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Screen.h"

#include "config.h"

#ifdef USE_BOOST_THREADS
#include <boost/chrono/include.hpp>
#include <boost/thread.hpp>
using namespace boost::this_thread;
using namespace boost::chrono;
#else
#include <chrono>
#include <thread>
using namespace std::this_thread;
using namespace std::chrono;
#endif

using namespace std;

int main(int argc, char* argv[])
{
    cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR;
    cout << "." << SPECIDE_VERSION_MINOR << endl;

    // The Screen class is now actually more of a "console".
    // We create the instance, and load the given tape (if any).
    Screen screen(2, true);
    bool useDefaultModel = true;

    vector<string> params(argv, argv + argc);
    vector<string> tapes;
    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        // Model selection.
        if (*it == "--issue2")
            screen.spectrum.ula.ulaVersion = 0;

        if (*it == "--issue3")
            screen.spectrum.ula.ulaVersion = 1;

        if (*it == "--48")
        {
            screen.buzzer.set128K(false);
            screen.spectrum.loadRoms(0);
            useDefaultModel = false;

        }

        if (*it == "--128")
        {
            screen.buzzer.set128K(true);
            screen.spectrum.loadRoms(1);
            screen.spectrum.set128K();
            useDefaultModel = false;
        }

        if (*it == "--plus2")
        {
            screen.buzzer.set128K(true);
            screen.spectrum.loadRoms(2);
            screen.spectrum.setPlus2();
            useDefaultModel = false;
        }

        // I'm putting both set and unset flags in case I implement loading
        // default parameters from a config file, which would override the
        // emulator's defaults.
        if (*it == "--kempston")
            screen.spectrum.kempston = true;

        if (*it == "--nokempston")
            screen.spectrum.kempston = false;

        if (*it == "--notapesound")
            screen.buzzer.tapeSound = false;

        if (*it == "--tapesound")
            screen.buzzer.tapeSound = true;

        if (*it == "--nosound")
            screen.buzzer.playSound = false;

        if (*it == "--sound")
            screen.buzzer.playSound = true;

        if (*it == "--psg")
            screen.buzzer.hasPsg = true;

        if (*it == "--nopsg")
            screen.buzzer.hasPsg = false;

        if (*it == "--psglinear")
            screen.spectrum.psg.setVolumeLevels(false);

        // SD1 was a protection device used in Camelot Warriors. It simply
        // forced bit 5 low for any port read, if the device didn't force
        // this bit high.
        if (*it == "--sd1")
        {
            screen.spectrum.idle = 0xDF;
            screen.spectrum.ula.inMask = 0x9F;
        }

        if (it->find('.') != string::npos)
            tapes.push_back(*it);
    }

    if (useDefaultModel)
    {
        screen.buzzer.set128K(false);
        screen.spectrum.loadRoms(0);
    }

    for (vector<string>::iterator it = tapes.begin(); it != tapes.end(); ++it)
        screen.tape.load(*it);

    steady_clock::time_point tick = steady_clock::now();
    // This is faster than "while(true)".
    for(;;)
    {
        // Update Spectrum hardware.
        screen.clock();

        // Update the screen.
        if (screen.update())
        {
            sleep_until(tick + milliseconds(20));
            tick = steady_clock::now();

            if (screen.done)
                return 0;
        }
    }
}

// vim: et:sw=4:ts=4
