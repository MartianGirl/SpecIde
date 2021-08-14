/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2021.
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

#include "CpcScreen.h"
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

#include <cfenv>
#include <cmath>

using namespace std;
using namespace sf;

CpcScreen::CpcScreen(map<string, string> o, vector<string> f) :
    Screen(o, f) {}

CpcScreen::~CpcScreen() {}

void CpcScreen::setup() {

    cout << "Initialising common settings..." << endl;
    Screen::setup();

    cout << "Initialising Amstrad CPC..." << endl;
    // Select model and ROMs.
    if (options["model"] == "cpc464") {
        cpc.set464();
    } else if (options["model"] == "cpc664") {
        cpc.set664();
    } else if (options["model"] == "cpc6128") {
        cpc.set6128();
    } // Default model is ZX Spectrum 48K Issue 3...

    uint_fast32_t crtc = 0;
    if (!options["crtc"].empty()) {
        try {
            crtc = stoi(options["crtc"]);
        } catch (invalid_argument &ia) {
            cout << "Invalid CRTC type: '" << options["crtc"] << "' - " << ia.what() << endl;
            options["crtc"] = "0";
        }
    }
    cout << "CRTC type: " << crtc << endl;
    cpc.ga.crtc.type = crtc;

    // Select joystick options.
    pad = (options["pad"] == "yes");
    cout << "Map game pad extra buttons to keys: " << options["pad"] << endl;

    // Sound settings.
    tapeSound = (options["tapesound"] != "no");
    cout << "Play tape sound: " << options["tapesound"] << endl;
    playSound = (options["sound"] != "no");
    cout << "Play sound: " << options["sound"] << endl;

    if (options["stereo"] == "acb") {
        cpc.stereo = StereoMode::STEREO_ACB;
    } else if (options["stereo"] == "abc") {
        cpc.stereo = StereoMode::STEREO_ABC;
    } else if (options["stereo"] == "mono") {
        cpc.stereo = StereoMode::STEREO_MONO;
    } else {
        cpc.stereo = StereoMode::STEREO_MONO;
    }
    cout << "Stereo type: " << options["stereo"] << endl;

    cpc.z80.zeroByte = options["z80type"] == "cmos" ? 0xFF : 0x00;
    cout << "Z80 type: " << options["z80type"] << endl;

    // Screen settings.
    if (options["scanmode"] == "scanlines") {
        doubleScanMode = true;
        cpc.ga.scanlines = 1;
        cpc.ga.yInc = 2;
    } else if (options["scanmode"] == "average") {
        doubleScanMode = false;
        cpc.ga.scanlines = 2;
        cpc.ga.yInc = 1;
    } else {
        doubleScanMode = false;
        cpc.ga.scanlines = 0;
        cpc.ga.yInc = 1;
    }
    cout << "Scan mode: " << options["scanmode"] << endl;

    xSize = GateArray::X_SIZE;
    ySize = GateArray::Y_SIZE / (doubleScanMode ? 1 : 2);
    texture(xSize, ySize);

    loadFiles();

    h = 560;
    w = 776;

    lBorder = 192;
    rBorder = 224;
    tBorder = 8;
    bBorder = 0;

    wide = true;
    reopenWindow(fullscreen);
    setFullScreen(fullscreen);
    setSmooth(smooth);
    cpc.tapeSound = tapeSound && playSound;
    cpc.psgPlaySound(playSound);
    cpc.setSoundRate(FRAME_TIME_CPC, syncToVideo);
    cpc.skipCycles = cpc.skip;
}

void CpcScreen::loadFiles() {

    for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
        switch (guessFileType(*it)) {
            case FileTypes::FILETYPE_CDT:
                cpc.tape.loadCdt(*it);
                break;

            case FileTypes::FILETYPE_CSW:
                cpc.tape.loadCsw(*it);
                break;

            case FileTypes::FILETYPE_DSK:
                {
                    DSKFile dsk;
                    dsk.load(*it);

                    if (dsk.validFile) {
                        cpc.fdc765.drive[0].images.push_back(dsk);
                        cpc.fdc765.drive[0].imageNames.push_back(*it);
                        cpc.fdc765.drive[0].disk = true;
                    }
                }
                break;

            default:
                cout << "Amstrad CPC does not support this file type: " << *it << endl;
                break;
        }
    }
}

void CpcScreen::run() {

    steady_clock::time_point tick = steady_clock::now();
    steady_clock::time_point frame;
    steady_clock::time_point wakeup;

    while (!done) {
        while (!done && !menu) {
            // Run until either we get a new frame, or we get 20ms of emulation.
            cpc.run(!syncToVideo);
            cpc.playSound(true);
            update();

            if (!syncToVideo) {
                uint_fast32_t delay = cpc.cycles / 16;
                uint_fast32_t sleep = delay - (delay % 1000);

                // By not sleeping until the next frame is due, we get some
                // better adjustment
#ifdef USE_BOOST_THREADS
                frame = tick + boost::chrono::microseconds(delay);
                wakeup = tick + boost::chrono::microseconds(sleep);
#else
                frame = tick + std::chrono::microseconds(delay);
                wakeup = tick + std::chrono::microseconds(sleep);
#endif
#ifndef DO_NOT_SLEEP
                sleep_until(wakeup);
#endif
                while ((tick = steady_clock::now()) < frame);
            } else {
                // If we are syncing with the PC's vertical refresh, we need
                // to get at least 20ms of emulation. If this is the case, we
                // update no matter the status of the screen.
                cpc.setSoundRate(FRAME_TIME_CPC, true);
            }

            cpc.scanKeys();
            pollEvents();
            pollCommands();
        }

        cpc.playSound(false);
    }

    while (!done && menu) {

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                menu = false;
            }
        }
#ifdef USE_BOOST_THREADS
        sleep_until(tick + boost::chrono::microseconds(20000));
#else
        sleep_until(tick + std::chrono::microseconds(20000));
#endif
        tick = steady_clock::now();
    }
}

void CpcScreen::update() {

    scrTexture.update(reinterpret_cast<Uint8*>(doubleScanMode ?
                cpc.ga.pixelsX2 : cpc.ga.pixelsX1));
    window.clear(Color::Black);
    window.draw(scrSprite);
    window.display();

    if (cpc.tape.pulseData.size()) {
        char str[64];
        unsigned int percent = 100 * cpc.tape.pointer / cpc.tape.pulseData.size();
        snprintf(str, 64, "SpecIde %d.%d.%d [%03u%%]",
                SPECIDE_VERSION_MAJOR,
                SPECIDE_VERSION_MINOR,
                SPECIDE_VERSION_TWEAK,
                percent);
        window.setTitle(str);
    }
}

void CpcScreen::updateMenu() {
}

void CpcScreen::close() {

    cpc.psgPlaySound(false);
    done = true;
}

void CpcScreen::focus(bool hasFocus) {

    cpc.pollKeys = hasFocus;
}

void CpcScreen::createEmptyDisk() {

    cpc.fdc765.drive[0].emptyDisk();
}

void CpcScreen::saveDisk() {

    cpc.fdc765.drive[0].saveDisk();
}

void CpcScreen::selectPreviousDisk() {

    cpc.fdc765.drive[0].prevDisk();
}

void CpcScreen::selectNextDisk() {

    cpc.fdc765.drive[0].nextDisk();
}

void CpcScreen::reset() {

    cpc.reset();
}

void CpcScreen::appendLoadTape() {
}

void CpcScreen::clearSaveTape() {
}

void CpcScreen::writeSaveTape() {
}

void CpcScreen::selectSaveTape() {
}

void CpcScreen::resetTapeCounter() {

    cpc.tape.resetCounter();
}

void CpcScreen::startStopTape() {

    cpc.tape.play();
    cpc.tapeSound = tapeSound;
}

void CpcScreen::rewindTape(bool toCounter) {

    cpc.tape.rewind(toCounter ? cpc.tape.counter : 0);
}

void CpcScreen::toggleTapeSound() {

    cpc.tapeSound = tapeSound = !tapeSound;
}

void CpcScreen::toggleSound() {

    cpc.tapeSound = playSound = !playSound;
    cpc.psgPlaySound(playSound);
}

void CpcScreen::togglePsgType() {

    aychip = !aychip;
    cpc.psgChip(aychip);
}

void CpcScreen::joystickHorizontalAxis(bool l, bool r) {

    cpc.joystick[0] &= 0xFC;
    if (l) {
        cpc.joystick[0] |= 0x02;
    } else if (r) {
        cpc.joystick[0] |= 0x01;
    }
}

void CpcScreen::joystickVerticalAxis(bool u, bool d) {

    cpc.joystick[0] &= 0xF3;
    if (u) {
        cpc.joystick[0] |= 0x08;
    } else if (d) {
        cpc.joystick[0] |= 0x04;
    }
}

void CpcScreen::joystickButtonPress(uint_fast32_t button) {

    button += 4;
    if (button < 6) {
        cpc.joystick[0] |= 1 << button;
    }
}

void CpcScreen::joystickButtonRelease(uint_fast32_t button) {

    button += 4;
    if (button < 6) {
        cpc.joystick[0] &= ~(1 << button);
    }
}
// vim: et:sw=4:ts=4
