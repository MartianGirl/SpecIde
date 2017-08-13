#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Spectrum.h"
#include "Screen.h"
#include "Buzzer.h"
#include "Tape.h"

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

constexpr size_t CLOCK_FREQ = 7000000;
constexpr size_t SAMPLE_RATE = 44100;
constexpr size_t SAMPLE_SKIP = CLOCK_FREQ / SAMPLE_RATE;

int main(int argc, char* argv[])
{
    bool fullscreen = false;

    cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR;
    cout << "." << SPECIDE_VERSION_MINOR << endl;

    Tape tape;
    if (argc == 2)
    {
        tape.load(argv[1]);
    }

    // Create a Spectrum
    Spectrum spectrum;

    // Create a screen and "connect" it to the Spectrum ULA.
    Screen screen(2);
    screen.setRgbaInput(&spectrum.ula.rgba);
    screen.setHSyncInput(&spectrum.ula.hSync);
    screen.setVSyncInput(&spectrum.ula.vSync);
    screen.setHBlankInput(&spectrum.ula.hBlank);
    screen.setVBlankInput(&spectrum.ula.vBlank);
    screen.setKeyboardPort(spectrum.ula.keys);
    screen.setJoystickPort(&spectrum.joystick);

    cout << "Opening sound at " << SAMPLE_RATE << " kHz." << endl;
    cout << "Sampling each " << SAMPLE_SKIP << " cycles." << endl;

    Buzzer buzzer;
    buzzer.open(&spectrum.ula.ioPortOut, &spectrum.ula.tapeIn, SAMPLE_RATE);
    buzzer.play();

    steady_clock::time_point tick = steady_clock::now();
    // This is faster than "while(true)".
    for(;;)
    {
        // Update Spectrum hardware.
        spectrum.clock();
        spectrum.ula.tapeIn = tape.advance();
        buzzer.sample();

        if (screen.update())
        {
            sleep_until(tick + milliseconds(20));
            tick = steady_clock::now();
        }
        // screen.update();

        if (screen.done)
        {
            buzzer.stop();
            return 0;
        }

        if (screen.reset)
        {
            spectrum.reset();
            screen.reset = false;
        }

        if (screen.rewind)
        {
            tape.rewind();
            screen.rewind = false;
        }

        if (screen.play)
        {
            tape.play();
            screen.play = false;
        }

        if (screen.fullscreen)
        {
            fullscreen = !fullscreen;
            screen.setFullScreen(fullscreen);
            screen.fullscreen = false;
        }
    }
}

// vim: et:sw=4:ts=4
