#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Screen.h"

#include "config.h"

using namespace std;

int main(int argc, char* argv[])
{
    printf("SpecIde Version %d.%d\n",
            SPECIDE_VERSION_MAJOR, SPECIDE_VERSION_MINOR);

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
            printf("Show some help.\n");

            screen.done = true;
        }

        // Model selection.
        if (*it == "--issue2")
        {
            screen.set128K(false);
            screen.spectrum.loadRoms(0);
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(0);
        }

        if (*it == "--issue3" || *it == "--48")
        {
            screen.set128K(false);
            screen.spectrum.loadRoms(0);
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(1);
        }

        if (*it == "--128")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(1);
            screen.spectrum.set128K();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(2);
        }

        if (*it == "--128sp")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(4);
            screen.spectrum.set128K();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(2);
        }

        if (*it == "--plus2")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(2);
            screen.spectrum.setPlus2();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(2);
        }

        if (*it == "--plus2sp")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(5);
            screen.spectrum.setPlus2();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(2);
        }
        
        if (*it == "--plus2a")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(3);
            screen.spectrum.setPlus2A();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(3);
        }

        if (*it == "--plus2asp")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(6);
            screen.spectrum.setPlus2A();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(3);
        }

        if (*it == "--plus3")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(3);
            screen.spectrum.setPlus3();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(3);
        }

        if (*it == "--plus3sp")
        {
            screen.set128K(true);
            screen.spectrum.loadRoms(6);
            screen.spectrum.setPlus3();
            useDefaultModel = false;
            screen.spectrum.ula.setUlaVersion(3);
        }

        // I'm putting both set and unset flags in case I implement loading
        // default parameters from a config file, which would override the
        // emulator's defaults.
        if (*it == "--kempston")
            screen.spectrum.kempston = true;

        if (*it == "--sinclair")
            screen.spectrum.kempston = false;

        if (*it == "--pad")
            screen.pad = true;

        if (*it == "--nopad")
            screen.pad = false;

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
        {
            screen.spectrum.psg.playSound = true;
            screen.spectrum.hasPsg = true;
        }

        if (*it == "--acb")
            screen.stereo = 1;

        if (*it == "--abc")
            screen.stereo = 2;

        if (*it == "--psglinear")
            screen.spectrum.psg.setVolumeLevels(false);

        if (*it == "--flashtap")
            screen.flashTap = true;

        if (*it == "--sync")
        {
            screen.syncToVideo = true;
            screen.window.setVerticalSyncEnabled(true);
        }

        if (*it == "--average")
        {
            screen.spectrum.ula.average = true;
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
            tapes.push_back(*it);
    }

    if (useDefaultModel)
    {
        screen.set128K(false);
        screen.spectrum.loadRoms(0);
        screen.spectrum.ula.setUlaVersion(1);
    }

    for (vector<string>::iterator it = tapes.begin(); it != tapes.end(); ++it)
        screen.tape.load(*it);

    screen.run();
}

// vim: et:sw=4:ts=4
