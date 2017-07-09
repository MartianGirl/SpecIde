#include <chrono>
#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "Spectrum.h"
#include "Screen.h"
#include "Buzzer.h"
#include "Tape.h"

#include "config.h"

using namespace std;

constexpr size_t CLOCK_FREQ = 7000000;
constexpr size_t SAMPLE_RATE = 44100;
constexpr size_t SAMPLE_SKIP = CLOCK_FREQ / SAMPLE_RATE;

int main(int argc, char* argv[])
{
    using namespace std::this_thread;
    using namespace std::chrono;

    cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR;
    cout << "." << SPECIDE_VERSION_MINOR << endl;

    Tape tape;
    bool tapeUpdate = false;
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

    cout << "Opening sound at " << SAMPLE_RATE << " kHz." << endl;
    cout << "Sampling each " << SAMPLE_SKIP << " cycles." << endl;
    Buzzer buzzer;
    buzzer.open(&spectrum.ula.ioPortOut, &spectrum.ula.tapeIn, SAMPLE_RATE);

    size_t sampleCounter = 0;

    // Connect the keyboard.
    screen.setKeyboardPort(&spectrum.ula.z80_a, spectrum.ula.keys);

    high_resolution_clock::time_point tick = high_resolution_clock::now();
    high_resolution_clock::time_point tock = high_resolution_clock::now();
    stringstream ss;

    size_t busyTime;
    // This is faster than "while(true)".
    buzzer.play();
    for(;;)
    {
        // Update Spectrum hardware.
        spectrum.clock();

        if (tape.playing)
        {
            tapeUpdate = !tapeUpdate;
            if (tapeUpdate)
            {
                tape.advance();
                spectrum.ula.tapeIn = (tape.level & 0x40) | 0x80;
            }
        }

        // Sample sound outputs.
        if (sampleCounter == 0)
            buzzer.sample();
        sampleCounter = (sampleCounter + 1) % SAMPLE_SKIP;

        // Update screen.
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
    }
}

// vim: et:sw=4:ts=4
