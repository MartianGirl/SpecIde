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

#include "SpeccyScreen.h"
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

SpeccyScreen::SpeccyScreen(map<string, string> o, vector<string> f) : 
    Screen(o, f) {}

SpeccyScreen::~SpeccyScreen() {}

void SpeccyScreen::setup() {

    cout << "Initialising common settings..." << endl;
    Screen::setup();
    spectrum.sync = syncToVideo;

    cout << "Initialising ZX Spectrum..." << endl;
    // Select ROMs and ULA variant.
    if (options["model"] == "issue2") {
        spectrum.setIssue2(RomVariant::ROM_48_EN);
    } else if (options["model"] == "issue3") {
        spectrum.setIssue3(RomVariant::ROM_48_EN);
    } else if (options["model"] == "48sp") {
        spectrum.setIssue3(RomVariant::ROM_48_ES);
    } else if (options["model"] == "128") {
        spectrum.set128K(RomVariant::ROM_128_EN);
    } else if (options["model"] == "128sp") {
        spectrum.set128K(RomVariant::ROM_128_ES);
    } else if (options["model"] == "plus2") {
        spectrum.setPlus2(RomVariant::ROM_PLUS2_EN);
    } else if (options["model"] == "plus2sp") {
        spectrum.setPlus2(RomVariant::ROM_PLUS2_ES);
    } else if (options["model"] == "plus2a") {
        spectrum.setPlus2A(RomVariant::ROM_PLUS3_EN);
    } else if (options["model"] == "plus2asp") {
        spectrum.setPlus2A(RomVariant::ROM_PLUS3_ES);
    } else if (options["model"] == "plus3") {
        spectrum.setPlus3(RomVariant::ROM_PLUS3_EN);
    } else if (options["model"] == "plus3sp") {
        spectrum.setPlus3(RomVariant::ROM_PLUS3_ES);
    } else if (options["model"] == "pentagon") {
        spectrum.setPentagon(RomVariant::ROM_PENTAGON);
    } else {
        options["model"] = "default";
        spectrum.setIssue3(RomVariant::ROM_48_EN);
    }
    cout << "Model: " << options["model"] << endl;

    // Select joystick interface.
    spectrum.kempston = (options["joystick"] == "kempston");
    cout << "Joystick type: " << options["joystick"] << endl;
    pad = (options["pad"] == "yes");
    cout << "Map game pad extra buttons to keys: " << options["pad"] << endl;

    // Sound settings.
    tapeSound = (options["tapesound"] != "no");
    cout << "Play tape sound: " << options["tapesound"] << endl;
    playSound = (options["sound"] != "no");
    cout << "Play sound: " << options["sound"] << endl;

    // Non-standard number of PSGs.
    if (options["forcepsg"] == "yes") {
        psgSound = true;
        spectrum.psgChips = 1;
        cout << "Enable AY interface on 128K ports: " << options["forcepsg"] << endl;
    }

    if (options["stereo"] == "turbo" || options["stereo"] == "turboacb"
            || options["stereo"] == "turboabc") {
        psgSound = true;
        spectrum.psgChips = 2;
        cout << "TurboSound (2 PSGs) active." << endl;
    } else if (options["stereo"] == "turbonext") {
        psgSound = true;
        spectrum.psgChips = 4;
        cout << "Generalized NEXT sound (4 PSGs) active." << endl;
    }

    // Set ACB stereo as default for Pentagon.
    if (options["model"] == "pentagon" && options["stereo"] == "none") {
        options["stereo"] = "acb";
    }

    if (options["stereo"] == "acb") {
        spectrum.stereo = StereoMode::STEREO_ACB;
    } else if (options["stereo"] == "abc") {
        spectrum.stereo = StereoMode::STEREO_ABC;
    } else if (options["stereo"] == "turbo") {
        spectrum.stereo = StereoMode::STEREO_TURBO_MONO;
    } else if (options["stereo"] == "turboabc") {
        spectrum.stereo = StereoMode::STEREO_TURBO_ABC;
    } else if (options["stereo"] == "turboacb") {
        spectrum.stereo = StereoMode::STEREO_TURBO_ACB;
    } else if (options["stereo"] == "turbonext") {
        spectrum.stereo = StereoMode::STEREO_NEXT;
    } else if (options["stereo"] == "mono") {
        spectrum.stereo = StereoMode::STEREO_MONO;
    } else {
        spectrum.stereo = StereoMode::STEREO_MONO;
    }
    cout << "Stereo type: " << options["stereo"] << endl;

    aychip = (options["psgtype"] != "ym");
    spectrum.psgChip(aychip);
    cout << "PSG chip: " << options["psgtype"] << endl;

    // Other stuff.
    spectrum.flashTap = (options["flashtap"] == "yes");
    cout << "FlashTAP: " << options["flashtap"] << endl;

    if (options["sd1"] == "yes") {
        spectrum.idle = 0xDF;
        spectrum.ula.inMask = 0x9F;
        cout << "SD1 dongle: " << options["sd1"] << endl;
    }

    spectrum.z80.zeroByte = options["z80type"] == "cmos" ? 0xFF : 0x00;
    cout << "Z80 type: " << options["z80type"] << endl;

    // Screen settings.
    if (options["scanmode"] == "scanlines") {
        doubleScanMode = true;
        spectrum.ula.scanlines = 1;
        spectrum.ula.yInc = 2;
    } else if (options["scanmode"] == "average") {
        doubleScanMode = false;
        spectrum.ula.scanlines = 2;
        spectrum.ula.yInc = 1;
    } else {
        doubleScanMode = false;
        spectrum.ula.scanlines = 0;
        spectrum.ula.yInc = 1;
    }
    cout << "Scan mode: " << options["scanmode"] << endl;

    lBorder = 8;
    rBorder = 8;
    tBorder = 0;
    bBorder = 0;

    xSize = ULA::X_SIZE;
    ySize = ULA::Y_SIZE / (doubleScanMode ? 1 : 2);
    texture(xSize, ySize);

    loadFiles();

    wide = false;
    reopenWindow(fullscreen);
    setFullScreen(fullscreen);
    setSmooth(smooth);
    spectrum.ula.tapeSound = tapeSound;
    spectrum.ula.playSound = playSound;
    spectrum.psgPlaySound(psgSound && playSound);
}

void SpeccyScreen::loadFiles() {

    for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
        switch (guessFileType(*it)) {
            case FileTypes::FILETYPE_TZX:
                spectrum.tape.loadTzx(*it);
                break;

            case FileTypes::FILETYPE_TAP:
                spectrum.tape.loadTap(*it);
                break;

            case FileTypes::FILETYPE_CSW:
                spectrum.tape.loadCsw(*it);
                break;

            case FileTypes::FILETYPE_DSK:
                {
                    DSKFile dsk;
                    dsk.load(*it);

                    if (dsk.validFile) {
                        spectrum.fdc765.drive[0].images.push_back(dsk);
                        spectrum.fdc765.drive[0].imageNames.push_back(*it);
                        spectrum.fdc765.drive[0].disk = true;
                    }
                }
                break;

            default:
                cout << "ZX Spectrum does not support this file type: " << *it << endl;
                break;
        }
    }
}

void SpeccyScreen::run() {

    steady_clock::time_point tick = steady_clock::now();
    steady_clock::time_point frame;
    steady_clock::time_point wakeup;

    while (!done) {
        while (!done && !menu) {
            // Run a complete frame.
            spectrum.run();

            // Delay starting playing sound (if disabled) until first frame
            // is complete
            spectrum.playSound(true);

            // Update the screen.
            // These conditions cannot happen at the same time:
            // - HSYNC and VSYNC only happen during the blanking interval.
            // - VSYNC happens at the end of blanking interval. (0x140)
            // - HSYNC happens at the beginning of HSYNC interval. (0x170-0x178)
            // If not blanking, draw.
            update();

            if (!syncToVideo) {
                // By not sleeping until the next frame is due, we get some
                // better adjustment
#ifdef USE_BOOST_THREADS
                frame = tick + boost::chrono::microseconds(spectrum.frame);
                wakeup = tick + boost::chrono::microseconds(18000);
#else
                frame = tick + std::chrono::microseconds(spectrum.frame);
                wakeup = tick + std::chrono::microseconds(18000);
#endif
#ifndef DO_NOT_SLEEP
                sleep_until(wakeup);
#endif
                while ((tick = steady_clock::now()) < frame);
            }

            pollEvents();
            pollCommands();
        }

        // Disable sound for menus
        spectrum.playSound(false);

        while (!done && menu) {
            // Menu thingy
            updateMenu();

            if (!syncToVideo) {
#ifdef USE_BOOST_THREADS
                sleep_until(tick + boost::chrono::microseconds(spectrum.frame));
#else
                sleep_until(tick + std::chrono::microseconds(spectrum.frame));
#endif
                tick = steady_clock::now();
            }
        }
    }
}

void SpeccyScreen::update() {

    scrTexture.update(reinterpret_cast<Uint8*>(doubleScanMode ?
                spectrum.ula.pixelsX2 : spectrum.ula.pixelsX1));
    window.clear(Color::Black);
    window.draw(scrSprite);
    window.display();

    if (spectrum.tape.pulseData.size()) {
        char str[64];
        unsigned int percent = 100 * spectrum.tape.pointer / spectrum.tape.pulseData.size();
        snprintf(str, 64, "SpecIde %d.%d.%d [%03u%%]",
                SPECIDE_VERSION_MAJOR,
                SPECIDE_VERSION_MINOR,
                SPECIDE_VERSION_TWEAK,
                percent);
        window.setTitle(str);
    }
}

void SpeccyScreen::updateMenu() {

    uint_fast32_t modifier = doubleScanMode ? 2 : 1;

    RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setFillColor(Color::White - Color(0, 0, 0, 64));
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(Color::Black);
    rectangle.setSize(Vector2f(324, 232));
    if (fullscreen) {
        rectangle.setPosition(xOffset + 6 * modifier * yScale, yOffset + 6 * modifier * yScale);
        rectangle.setScale(Vector2f(modifier * yScale, modifier * yScale));
    } else {
        // Windowed mode already applies a x2 scale factor, so single scan windows are the same size
        // as double scan ones.
        rectangle.setPosition(12 * scale, 12 * scale);
        rectangle.setScale(Vector2f(2 * static_cast<float>(scale), 2 * static_cast<float>(scale)));
    }

    Text text;
    text.setFont(zxFont);
    text.setFillColor(Color::Black);

    stringstream ss;
    ss << "Function keys:" << endl;
    ss << "F1:    This help." << endl;
    ss << "F2:    Fullscreen." << endl;
    ss << "S-F2:  Antialiasing." << endl;
    ss << "F3:    Save DSK file to disk." << endl;
    ss << "S-F3:  Create empty DSK image." << endl;
    ss << "F4:    Select next disk image." << endl;
    ss << "S-F4:  Select previous disk image." << endl;
    ss << "F5:    Reset." << endl;
    ss << "F6:    Clear SAVE buffer." << endl;
    ss << "S-F6:  Add FlashTAP to SAVE buffer." << endl;
    ss << "F7:    Write SAVE buffer to disk." << endl;
    ss << "S-F7:  Use SAVE buffer as FlashTAP." << endl;
    ss << "F8:    Toggle PSG: AY-3-8912/YM-2149." << endl;
    ss << "F9:    Sound on / off." << endl;
    ss << "S-F9:  Tape sound on / off." << endl;
    ss << "F10:   Exit emulator." << endl;
    ss << "F11:   Play / stop tape." << endl;
    ss << "S-F11: Set mark in tape." << endl;
    ss << "F12:   Rewind tape to start." << endl;
    ss << "S-F12: Rewind tape to mark." << endl;
    ss << endl;
    text.setString(ss.str());

    if (fullscreen) {
        text.setPosition(xOffset + 36, yOffset + 36);
        text.setCharacterSize(static_cast<uint32_t>(8 * modifier * yScale));
    } else {
        text.setPosition(36, 36);
        text.setCharacterSize(static_cast<uint32_t>(16 * scale));
    }

    window.clear(Color::Black);
    window.draw(scrSprite);
    window.draw(rectangle);
    window.draw(text);
    window.display();

    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::KeyPressed) {
            switch (event.key.code) {
                case Keyboard::Menu:    // fall-through
                case Keyboard::F1:
                    menu = false;
                    break;

                default:
                    break;
            }
        }
    }
}

void SpeccyScreen::close() {

    spectrum.playSound(false);
    done = true;
}

void SpeccyScreen::focus(bool hasFocus) {

    spectrum.ula.pollKeys = hasFocus;
}

void SpeccyScreen::createEmptyDisk() {

    spectrum.fdc765.drive[0].emptyDisk();
}

void SpeccyScreen::saveDisk() {

    spectrum.fdc765.drive[0].saveDisk();
}

void SpeccyScreen::selectPreviousDisk() {

    spectrum.fdc765.drive[0].prevDisk();
}

void SpeccyScreen::selectNextDisk() {

    spectrum.fdc765.drive[0].nextDisk();
}

void SpeccyScreen::reset() {

    spectrum.reset();
}

void SpeccyScreen::appendLoadTape() {

    spectrum.tape.appendLoadData();
}

void SpeccyScreen::clearSaveTape() {

    spectrum.tape.clearSaveData();
}

void SpeccyScreen::writeSaveTape() {

    spectrum.tape.writeSaveData();
}

void SpeccyScreen::selectSaveTape() {

    spectrum.tape.useSaveData = !spectrum.tape.useSaveData;
    spectrum.tape.selectTapData();
}

void SpeccyScreen::resetTapeCounter() {

    spectrum.tape.resetCounter();
}

void SpeccyScreen::startStopTape() {

    spectrum.tape.play();
    spectrum.ula.tapeSound = tapeSound;
}

void SpeccyScreen::rewindTape(bool toCounter) {

    spectrum.tape.rewind(toCounter ? spectrum.tape.counter : 0);
}

void SpeccyScreen::toggleTapeSound() {

    spectrum.ula.tapeSound = tapeSound = !tapeSound;
}

void SpeccyScreen::toggleSound() {

    spectrum.ula.playSound = playSound = !playSound;
    spectrum.psgPlaySound(psgSound & playSound);
}

void SpeccyScreen::togglePsgType() {

    aychip = !aychip;
    spectrum.psgChip(aychip);
}

void SpeccyScreen::joystickHorizontalAxis(bool l, bool r) {

    if (spectrum.kempston) {
        spectrum.kempstonData &= 0xFC;
        if (l) {
            spectrum.kempstonData |= 0x02;
        } else if (r) {
            spectrum.kempstonData |= 0x01;
        }
    } else {
        spectrum.ula.sinclairData &= 0xFC;
        if (l) {
            spectrum.ula.sinclairData |= 0x02;
        } else if (r) {
            spectrum.ula.sinclairData |= 0x01;
        }
    }
}

void SpeccyScreen::joystickVerticalAxis(bool u, bool d) {

    if (spectrum.kempston) {
        spectrum.kempstonData &= 0xF3;
        if (u) {
            spectrum.kempstonData |= 0x08;
        } else if (d) {
            spectrum.kempstonData |= 0x04;
        }
    } else {
        spectrum.ula.sinclairData &= 0xF3;
        if (u) {
            spectrum.ula.sinclairData |= 0x08;
        } else if (d) {
            spectrum.ula.sinclairData |= 0x04;
        }
    }
}

void SpeccyScreen::joystickButtonPress(uint_fast32_t button) {

    if (pad) {
        button += 4;
    } else {
        button = 4;  // Interpret every button as default.
    }

    if (spectrum.kempston && button < 5) {
        spectrum.kempstonData |= 1 << button;
    } else {
        spectrum.ula.sinclairData |= 1 << button;
    }
}

void SpeccyScreen::joystickButtonRelease(uint_fast32_t button) {

    if (pad) {
        button += 4;
    } else {
        button = 4;  // Interpret every button as default.
    }

    if (spectrum.kempston && button < 5) {
        spectrum.kempstonData &= ~(1 << button);
    } else {
        spectrum.ula.sinclairData &= ~(1 << button);
    }
}
// vim: et:sw=4:ts=4
