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
#include "KeyBinding.h"
#include "Utils.h"

#include "config.h"

#include <SFML/System.hpp>

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
    loadFont("AmstradCPC.ttf");

    cout << "Initialising Amstrad CPC..." << endl;
    // Select model and ROMs.
    if (options["model"] == "cpc464") {
        cpc.set464(RomVariant::ROM_CPC464_EN);
    } else if (options["model"] == "cpc464sp") {
        cpc.set464(RomVariant::ROM_CPC464_ES);
    } else if (options["model"] == "cpc464fr") {
        cpc.set464(RomVariant::ROM_CPC464_FR);
    } else if (options["model"] == "cpc664") {
        cpc.set664(RomVariant::ROM_CPC664_EN);
    } else if (options["model"] == "cpc6128") {
        cpc.set6128(RomVariant::ROM_CPC6128_EN);
    } else if (options["model"] == "cpc6128sp") {
        cpc.set6128(RomVariant::ROM_CPC6128_ES);
    } else if (options["model"] == "cpc6128fr") {
        cpc.set6128(RomVariant::ROM_CPC6128_FR);
    }

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
    soundEnabled = (options["sound"] != "no");
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

    aychip = (options["psgtype"] != "ym");
    cpc.psgChip(aychip);
    cout << "PSG chip: " << options["psgtype"] << endl;

    cpc.z80.zeroByte = options["z80type"] == "cmos" ? 0xFF : 0x00;
    cout << "Z80 type: " << options["z80type"] << endl;

    // Screen settings.
    if (options["scanmode"] == "interlace") {
        doubleScanMode = true;
        cpc.ga.scanlines = 1;
        cpc.ga.yInc = 2;
    } else if (options["scanmode"] == "average") {
        doubleScanMode = false;
        cpc.ga.scanlines = 2;
        cpc.ga.yInc = 1;
    } else if (options["scanmode"] == "scanlines") {
        doubleScanMode = true;
        cpc.ga.scanlines = 3;
        cpc.ga.yInc = 2;
    } else {
        doubleScanMode = false;
        cpc.ga.scanlines = 0;
        cpc.ga.yInc = 1;
    }
    cout << "Scan mode: " << options["scanmode"] << endl;

    if (options["display"] == "bw") {
        cpc.ga.setPalette(CPC_PALETTE_BW);
    } else if (options["display"] == "green") {
        cpc.ga.setPalette(CPC_PALETTE_GREEN);
    } else if (options["display"] == "amber") {
        cpc.ga.setPalette(CPC_PALETTE_AMBER);
    } else {
        cpc.ga.setPalette(CPC_PALETTE_COLOUR);
    }
    cout << "Display type: " << options["display"] << endl;

    xSize = GateArray::X_SIZE;
    ySize = GateArray::Y_SIZE / (doubleScanMode ? 1 : 2);
    texture(xSize, ySize);

    cpc.tape.speed = 1.16;
    loadFiles();

    lBorder = 208;
    rBorder = 40;
    tBorder = 8;
    bBorder = 0;

    wide = true;
    reopenWindow(fullscreen);
    setFullScreen(fullscreen);
    cpc.tapeSound = tapeSound && soundEnabled;
    cpc.psgPlaySound(soundEnabled);
    cpc.setSoundRate(FRAME_TIME_CPC, syncToVideo);
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

            case FileTypes::FILETYPE_PAL:
                {
                    vector<uint32_t> palette;
                    loadPalette(*it, palette);
                    if (palette.size() == CPC_PALETTE_SIZE) {
                        for (size_t ii = 0; ii < CPC_PALETTE_SIZE; ++ii) {
                            cpc.ga.palette[CPC_PALETTE_CUSTOM][ii] = palette[ii];
                        }

                        cpc.ga.setPalette(CPC_PALETTE_CUSTOM);
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

    while (!done) {
        Clock clock;
        Time frameTime = clock.getElapsedTime();    // Next frame time
        Time spentTime; // Actual time spent in emulation
        Time delayTime; // Delay time to adjust emulation pace

        while (!done && !menu) {
            // Run until either we get a new frame, or we get 20ms of emulation.
            pollEvents();

            cpc.run(!syncToVideo);
            if (SoundChannel::getChannel().commit()) {
                SoundChannel::getChannel().play();
            }

            update();

            if (!syncToVideo) {
                frameTime += microseconds(cpc.cycles / 16);
#ifndef DO_NOT_SLEEP
                spentTime = clock.getElapsedTime();
                delayTime = frameTime - spentTime;
                if (timerStep) {
                    delayTime -= delayTime % milliseconds(timerStep); // Adjust to timer step.
                }
                sleep(delayTime);
#endif
                while (clock.getElapsedTime() < frameTime); // Active wait for the remainder.
            } else {
                // If we are syncing with the PC's vertical refresh, we need
                // to get at least 20ms of emulation. If this is the case, we
                // update no matter the status of the screen.
                cpc.setSoundRate(FRAME_TIME_CPC, true);
            }
        }

        while (!done && menu) {
            // Menu thingy
            updateMenu();
            sleep(microseconds(20000));
        }
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
        snprintf(str, 64, "SpecIde [%s(%s)] [%03u%%]",
                SPECIDE_BUILD_DATE, SPECIDE_BUILD_COMMIT, percent);
        window.setTitle(str);
    }
}

void CpcScreen::updateMenu() {

    uint_fast32_t modifier = doubleScanMode ? 2 : 1;

    RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setFillColor(Color(0, 0, 255, 192));
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(Color(255, 255, 0, 192));
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
    text.setFont(guiFont);
    text.setFillColor(Color(255, 255, 0, 192));

    stringstream ss;
    ss << "Function keys:" << endl;
    ss << "F1:    This help." << endl;
    ss << "F2:    Fullscreen." << endl;
    ss << "F3:    Save DSK file to disk." << endl;
    ss << "S-F3:  Create empty DSK image." << endl;
    ss << "F4:    Select next disk image." << endl;
    ss << "S-F4:  Select previous disk image." << endl;
    ss << "F5:    Reset." << endl;
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

void CpcScreen::close() {

    done = true;
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
    cpc.tapeLevel = cpc.tape.level;
}

void CpcScreen::rewindTape(bool toCounter) {

    cpc.tape.rewind(toCounter ? cpc.tape.counter : 0);
}

void CpcScreen::toggleTapeSound() {

    cpc.tapeSound = tapeSound = !tapeSound;
}

void CpcScreen::toggleSound() {

    cpc.tapeSound = soundEnabled = !soundEnabled;
    cpc.psgPlaySound(soundEnabled);
}

void CpcScreen::togglePsgType() {

    aychip = !aychip;
    cpc.psgChip(aychip);
}

void CpcScreen::toggleFlashTap() {

    // FlashTAP is not supported on CPC yet.
}

void CpcScreen::joystickHorizontalAxis(uint_fast32_t id, bool l, bool r) {

    if (id < 2) {
        mapKeyJoystickAxis(id, MOVE_L, MOVE_R, l, r);
    }
}

void CpcScreen::joystickVerticalAxis(uint_fast32_t id, bool u, bool d) {

    if (id < 2) {
        mapKeyJoystickAxis(id, MOVE_U, MOVE_D, u, d);
    }
}

void CpcScreen::joystickButtonPress(uint_fast32_t id, uint_fast32_t button) {

    if (id < 2) {
        button += 4;
        if (button < FIRE_3) {
            pressKeyJoystickButton(id, button);
        }
    }
}

void CpcScreen::joystickButtonRelease(uint_fast32_t id, uint_fast32_t button) {

    if (id < 2) {
        button += 4;
        if (button < FIRE_3) {
            releaseKeyJoystickButton(id, button);
        }
    }
}


void CpcScreen::keyPress(Keyboard::Scancode key) {

    try {
        InputMatrixPosition pos = cpcKeys.at(key);
        cpc.keys[pos.row] &= ~pos.key;
    } catch (out_of_range const& oor) {}
}

void CpcScreen::keyRelease(Keyboard::Scancode key) {

    try {
        InputMatrixPosition pos = cpcKeys.at(key);
        cpc.keys[pos.row] |= pos.key;
    } catch (out_of_range const& oor) {}
}

float CpcScreen::getPixelClock() {

    return static_cast<float>(BASE_CLOCK_CPC) / 1000000.0;
}

void CpcScreen::mapKeyJoystickAxis(uint_fast32_t id,
        uint_fast32_t indexA, uint_fast32_t indexB, bool a, bool b) {
    cpc.keys[cpcJoystick[id][indexA].row] |= cpcJoystick[id][indexA].key;
    cpc.keys[cpcJoystick[id][indexB].row] |= cpcJoystick[id][indexB].key;
    if (a) {
        cpc.keys[cpcJoystick[id][indexA].row] &= ~cpcJoystick[id][indexA].key;
    } else if (b) {
        cpc.keys[cpcJoystick[id][indexB].row] &= ~cpcJoystick[id][indexB].key;
    }
}

void CpcScreen::pressKeyJoystickButton(uint_fast32_t id, uint_fast32_t button) {
    cpc.keys[cpcJoystick[id][button].row] &= ~cpcJoystick[id][button].key;
}

void CpcScreen::releaseKeyJoystickButton(uint_fast32_t id, uint_fast32_t button) {
    cpc.keys[cpcJoystick[id][button].row] |= cpcJoystick[id][button].key;
}
// vim: et:sw=4:ts=4
