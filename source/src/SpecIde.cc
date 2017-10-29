#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>

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
    Screen screen;
    if (argc == 2)
    {
        screen.tape.load(argv[1]);
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
        }
        // screen.update();

        if (screen.done)
            return 0;
    }
}

// vim: et:sw=4:ts=4
