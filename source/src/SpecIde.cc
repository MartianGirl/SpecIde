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

    vector<string> params(argv, argv + argc);
    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        if (*it == "--kempston")
        {
            screen.spectrum.kempston = true;
        }

        if (*it == "--issue2")
        {
            screen.spectrum.ula.ulaVersion = 0;
        }

        if (*it == "--noTapeSound")
        {
            screen.buzzer.tapeSound = false;
        }

        // SD1 was a protection device used in Camelot Warriors. It simply
        // forced bit 5 low for any port read, if the device didn't force
        // this bit high.
        if (*it == "--sd1")
        {
            screen.spectrum.idle = 0xDF;
            screen.spectrum.ula.inMask = 0x9F;
        }

        if (it->find('.') != string::npos)
        {
            screen.tape.load(*it);
        }
    }

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
