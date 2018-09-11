/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Screen.h"

#include "config.h"

using namespace std;

int main(int argc, char* argv[])
{
    vector<string> params(argv, argv + argc);
    vector<string> tapes;

    cout << "SpecIde Version " << SPECIDE_VERSION_MAJOR
        << "." << SPECIDE_VERSION_MINOR << endl << endl;

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        if (*it == "--help" || *it == "-h")
        {
            cout << "Usage: SpecIde [options] [tapefiles]" << endl;
            cout << endl;
            cout << "Supported formats are TAP and TZX." << endl;
            cout << endl;
            cout << "Options:" << endl;
            cout << endl;
            cout << "Model selection options:" << endl;
            cout << "--issue2:          Spectrum 48K, issue 2." << endl;
            cout << "--issue3 | --48:   Spectrum 48K, issue 3. (Default)" << endl;
            cout << "--128:             Spectrum 128K." << endl;
            cout << "--128sp:           Spectrum 128K. (Spanish ROM)" << endl;
            cout << "--plus2            Spectrum +2." << endl;
            cout << "--plus2sp          Spectrum +2. (Spanish ROM)" << endl;
            cout << "--plus2a           Spectrum +2A." << endl;
            cout << "--plus2asp         Spectrum +2A. (Spanish ROM)" << endl;
            // cout << "--plus3           Spectrum +3." << endl;
            // cout << "--plus3sp         Spectrum +3. (Spanish ROM)" << endl;
            cout << endl;
            cout << "Hardware options:" << endl;
            cout << "--kempston:        Map joystick to Kempston interface." << endl;
            cout << "--sinclair:        Map joystick to Sinclair interface. (Default)" << endl;
            cout << "--pad:             Map joystick extra buttons to keys." << endl;
            cout << "--psg:             Emulate AY chip. (Default for 128K models)" << endl;
            cout << "--abc|--acb:       Select ABC|ACB stereo modes." << endl;
            cout << "--sd1:             Emulate Dinamic SD1 dongle. (for Camelot Warriors)" << endl;
            cout << endl;
            cout << "Video options:" << endl;
            cout << "--fullscreen       Start SpecIde in full screen mode." << endl;
            cout << "--scanlines        Render PAL double scan mode." << endl;
            cout << "--average          Render PAL double scan mode, averaging scanlines." << endl;
            cout << "--sync             Synchronize emulation to PC video refresh rate." << endl;
            cout << endl;
            cout << "Emulation options:" << endl;
            cout << "--nosound          Disable all sound." << endl;
            cout << "--notapesound      Disable tape sounds." << endl;
            cout << "--flashtap         Enable ROM traps for LOAD and SAVE." << endl;
            cout << endl;
            cout << "Function keys:" << endl;
            // cout << "F1:                Display menu." << endl;
            cout << "F2:                Turn fullscreen mode on / off." << endl;
            cout << "Shift + F2:        Turn antialiasing on / off." << endl;
            cout << "F5:                Reset emulated Spectrum." << endl;
            cout << "F7:                Append LOAD FlashTAP data to SAVE buffer." << endl;
            cout << "Shift + F7:        Clear SAVE buffer." << endl;
            cout << "F8:                Write SAVE buffer to disk as 'savetapeXX.tap'." << endl;
            cout << "Shift + F8:        Use SAVE buffer as LOAD FlashTAP buffer." << endl;
            cout << "F9:                Turn sound on / off." << endl;
            cout << "Shift + F9:        Turn tape sound on / off." << endl;
            cout << "F10:               Exit emulator." << endl;
            cout << "F11:               Play / stop tape." << endl;
            cout << "Shift + F11:       Set mark in tape." << endl;
            cout << "F12:               Rewind tape to the beginning." << endl;
            cout << "Shift + F12:       Rewind tape to mark." << endl;
            cout << endl;
            exit(0);
        }
    }

    // The Screen class is now actually more of a "console".
    // We create the instance, and load the given tape (if any).
    Screen screen(1);
    bool useDefaultModel = true;

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
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

        if (*it == "--scanlines")
        {
            screen.doubleScanMode = true;
            screen.spectrum.ula.scanlines = 1;
            screen.spectrum.ula.yInc = 2;
        }

        if (*it == "--average")
        {
            screen.doubleScanMode = false;
            screen.spectrum.ula.scanlines = 2;
            screen.spectrum.ula.yInc = 1;
        }

        if (*it == "--fullscreen")
        {
            screen.fullscreen = true;
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

    screen.selectPixBuf();
    screen.reopenWindow(screen.fullscreen);
    screen.setFullScreen(screen.fullscreen);
    screen.run();
}

// vim: et:sw=4:ts=4
