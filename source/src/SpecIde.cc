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
        if (*it == "--help" || *it == "-h")
        {
            cout << "--help, -h:    Show this help." << endl << endl;
            cout << "Model selection:" << endl;
            cout << "--48           Spectrum 48K. (Default)" << endl;
            cout << "--128          Spectrum 128K." << endl;
            cout << "--plus2        Spectrum +2." << endl << endl;
            cout << "ULA version:" << endl;
            cout << "--issue2       Spectrum 48K Issue 2." << endl;
            cout << "--issue3       Spectrum 48K Issue 3. (Default)" << endl << endl;
            cout << "Other hardware:" << endl;
            cout << "--kempston     Enable kempston joystick. (Default)" << endl;
            cout << "--nokempston   Disable kempston joystick." << endl << endl;

            screen.done = true;
        }

        // Model selection.
        if (*it == "--issue2")
            screen.spectrum.ula.ulaVersion = 0;

        if (*it == "--issue3")
            screen.spectrum.ula.ulaVersion = 1;

        if (*it == "--48")
        {
            screen.set128K(false);
            screen.spectrum.loadRoms(0);
            useDefaultModel = false;

        }

        if (*it == "--128")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(1);
            screen.spectrum.set128K();
            useDefaultModel = false;
            screen.spectrum.ula.ulaVersion = 1;
        }

        if (*it == "--plus2")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(2);
            screen.spectrum.setPlus2();
            useDefaultModel = false;
            screen.spectrum.ula.ulaVersion = 1;
        }

        // I'm putting both set and unset flags in case I implement loading
        // default parameters from a config file, which would override the
        // emulator's defaults.
        if (*it == "--kempston")
            screen.spectrum.kempston = true;

        if (*it == "--nokempston")
            screen.spectrum.kempston = false;

        if (*it == "--notapesound")
            screen.spectrum.buzzer.tapeSound = false;

        if (*it == "--tapesound")
            screen.spectrum.buzzer.tapeSound = true;

        if (*it == "--nosound")
        {
            screen.spectrum.buzzer.playSound = false;
            screen.spectrum.psg.playSound = false;
        }

        if (*it == "--sound")
        {
            screen.spectrum.buzzer.playSound = true;
            screen.spectrum.psg.playSound = true;
        }

        if (*it == "--psg")
            screen.spectrum.psg.playSound = true;

        if (*it == "--nopsg")
            screen.spectrum.psg.playSound = false;

        if (*it == "--acb")
            screen.stereo = 1;

        if (*it == "--abc")
            screen.stereo = 2;

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
        screen.set128K(false);
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
