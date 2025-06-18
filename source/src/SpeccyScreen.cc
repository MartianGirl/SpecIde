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
#include "KeyBinding.h"
#include "SNAFile.h"
#include "Z80File.h"
#include "Utils.h"

#include <SFML/System.hpp>

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
    loadFont("ZXSpectrum.ttf");

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
    cout << "Model: " << options["model"];
    if (options["model"] == "issue2"
            || options["model"] == "issue3"
            || options["model"] == "48sp") {
        spectrum.z80.lateTimings = (options["z80timings"] == "late");
        cout << " (" << ((options["z80timings"] == "late") ? "Late" : "Early") << " timings)";
    }
    cout << endl;

    // Select joystick interface.
    cout << "Joystick interface: ";
    if (options["joystick"] == "kempston_new") {
        spectrum.joystick = JoystickType::KEMPSTON_NEW;
        cout << "Kempston Joystick (New, address mask 0x00E0)" << endl;
        cout << "   (Second joystick is mapped to Sinclair 1)" << endl;
    } else if (options["joystick"] == "kempston") {
        spectrum.joystick = JoystickType::KEMPSTON_OLD;
        cout << "Kempston Joystick (Old, address mask 0x0020)" << endl;
        cout << "   (Second joystick is mapped to Sinclair 1)" << endl;
    } else if (options["joystick"] == "fuller") {
        spectrum.joystick = JoystickType::FULLER;
        psgSound = true;
        cout << "Fuller Joystick" << endl;
        cout << "   (Second joystick is mapped to Sinclair 1)" << endl;
    } else if (options["joystick"] == "cursor") {
        spectrum.joystick = JoystickType::CURSOR;
        cout << "Protek/AGF Cursor Joystick" << endl;
        cout << "   (Second joystick is mapped to Kempston)" << endl;
    } else {
        spectrum.joystick = JoystickType::SINCLAIR;
        cout << "Sinclair 1 Joystick" << endl;
        cout << "   (Second joystick is mapped to Sinclair 2)" << endl;
    }
    pad = (options["pad"] == "yes");
    cout << "Map game pad extra buttons to keys: " << options["pad"] << endl;

    // Sound settings.
    tapeSound = (options["tapesound"] != "no");
    cout << "Play tape sound: " << options["tapesound"] << endl;
    soundEnabled = (options["sound"] != "no");
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

    if (options["covox"] == "mono") {
        spectrum.covoxMode = Covox::MONO;
    } else if (options["covox"] == "stereo") {
        spectrum.covoxMode = Covox::STEREO;
    } else if (options["covox"] == "czech") {
        spectrum.covoxMode = Covox::CZECH;
    } else if (options["covox"] == "soundrive1") {
        spectrum.covoxMode = Covox::SOUNDRIVE1;
    } else if (options["covox"] == "soundrive2") {
        spectrum.covoxMode = Covox::SOUNDRIVE2;
    } else {
        spectrum.covoxMode = Covox::NONE;
    }
    cout << "Covox type: " << options["covox"] << endl;
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
    if (options["scanmode"] == "interlace") {
        doubleScanMode = true;
        spectrum.ula.scanlines = 1;
        spectrum.ula.yInc = 2;
    } else if (options["scanmode"] == "average") {
        doubleScanMode = false;
        spectrum.ula.scanlines = 2;
        spectrum.ula.yInc = 1;
    } else if (options["scanmode"] == "scanlines") {
        doubleScanMode = true;
        spectrum.ula.scanlines = 3;
        spectrum.ula.yInc = 2;
    } else {
        doubleScanMode = false;
        spectrum.ula.scanlines = 0;
        spectrum.ula.yInc = 1;
    }
    cout << "Scan mode: " << options["scanmode"] << endl;

    if (options["display"] == "bw") {
        spectrum.ula.setPalette(ULA_PALETTE_BW);
    } else if (options["display"] == "green") {
        spectrum.ula.setPalette(ULA_PALETTE_GREEN);
    } else if (options["display"] == "amber") {
        spectrum.ula.setPalette(ULA_PALETTE_AMBER);
    } else {
        spectrum.ula.setPalette(ULA_PALETTE_COLOUR);
    }
    cout << "Display type: " << options["display"] << endl;

    lBorder = 12;
    rBorder = 4;
    tBorder = 0;
    bBorder = 0;

    xSize = ULA::X_SIZE;
    ySize = ULA::Y_SIZE / (doubleScanMode ? 1 : 2);
    texture(xSize, ySize);

    loadFiles();

    wide = false;
    reopenWindow(fullscreen);
    setFullScreen(fullscreen);
    spectrum.ula.tapeSound = tapeSound;
    spectrum.ula.playSound = soundEnabled;
    spectrum.psgPlaySound(psgSound && soundEnabled);
}

void SpeccyScreen::loadFiles() {

    for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
        switch (guessFileType(*it)) {
            case FileTypes::FILETYPE_TZX:
                spectrum.tape.loadTzx(*it);
                break;

            case FileTypes::FILETYPE_PZX:
                spectrum.tape.loadPzx(*it);
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

            case FileTypes::FILETYPE_Z80:
                {
                    Z80File snap;
                    snap.load(*it);
                    cout << "Loading Z80 snapshot: " << *it << endl;
                    if (snap.parse()) {
                        spectrum.loadState(snap.state);
                    }
                }
                break;

            case FileTypes::FILETYPE_SNA:
                {
                    SNAFile snap;
                    snap.load(*it);
                    cout << "Loading SNA snapshot: " << *it << endl;
                    if (snap.parse()) {
                        spectrum.loadState(snap.state);
                    }
                }
                break;

            case FileTypes::FILETYPE_PAL:
                {
                    vector<uint32_t> palette;
                    loadPalette(*it, palette);
                    if (palette.size() == ULA_PALETTE_SIZE) {
                        for (size_t ii = 0; ii < ULA_PALETTE_SIZE; ++ii) {
                            spectrum.ula.palette[ULA_PALETTE_CUSTOM][ii] = palette[ii];
                        }

                        spectrum.ula.setPalette(ULA_PALETTE_CUSTOM);
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

    while (!done) {
        Clock clock;
        Time frameTime = clock.getElapsedTime(); // Next frame time
        Time spentTime; // Actual time elapsed
        Time delayTime; // Delay time to adjust emulation pace

        while (!done && !menu) {
            // Run a complete frame.
            pollEvents();

            spectrum.run();
            if (SoundChannel::getChannel().commit()) {
                SoundChannel::getChannel().play();
            }

            update();

            if (!syncToVideo) {
                frameTime += microseconds(spectrum.frame);
#ifndef DO_NOT_SLEEP
                spentTime = clock.getElapsedTime();
                delayTime = frameTime - spentTime;
                if (timerStep) {
                    delayTime -= delayTime % milliseconds(timerStep); // Adjust to timer step.
                }
                sleep(delayTime);
#endif
                while (clock.getElapsedTime() < frameTime); // Active wait for the remainder.
            }
        }

        while (!done && menu) {
            // Menu thingy
            updateMenu();

            if (!syncToVideo) {
                sleep(microseconds(20000));
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
        snprintf(str, 64, "SpecIde [%s(%s)] [%03u%%]",
                SPECIDE_BUILD_DATE, SPECIDE_BUILD_COMMIT, percent);
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
    text.setFont(guiFont);
    text.setFillColor(Color::Black);

    stringstream ss;
    ss << "Function keys:" << endl;
    ss << "F1:    This help." << endl;
    ss << "F2:    Fullscreen." << endl;
    ss << "F3:    Save DSK file to disk." << endl;
    ss << "S-F3:  Create empty DSK image." << endl;
    ss << "F4:    Select next disk image." << endl;
    ss << "S-F4:  Select previous disk image." << endl;
    ss << "F5:    Reset." << endl;
    ss << "F6:    Clear SAVE buffer." << endl;
    ss << "S-F6:  Add FlashTAP to SAVE buffer." << endl;
    ss << "F7:    Write SAVE buffer to disk." << endl;
    ss << "S-F7:  Use SAVE buffer as FlashTAP." << endl;
    ss << "F8:    Toggle FlashTAP on/off." << endl;
    ss << "S-F8:  Toggle PSG: AY-3-8912/YM-2149." << endl;
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

    done = true;
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
    spectrum.ula.setEarLevel(spectrum.tape.level, spectrum.tape.playing);
}

void SpeccyScreen::rewindTape(bool toCounter) {

    spectrum.tape.rewind(toCounter ? spectrum.tape.counter : 0);
}

void SpeccyScreen::toggleTapeSound() {

    spectrum.ula.tapeSound = tapeSound = !tapeSound;
}

void SpeccyScreen::toggleSound() {

    spectrum.ula.playSound = soundEnabled = !soundEnabled;
    spectrum.psgPlaySound(psgSound & soundEnabled);
}

void SpeccyScreen::togglePsgType() {

    aychip = !aychip;
    spectrum.psgChip(aychip);
}

void SpeccyScreen::toggleFlashTap() {

    spectrum.tape.rewind();
    spectrum.flashTap = !spectrum.flashTap;
    cout << "FlashTAP: " << (spectrum.flashTap ? "yes" : "no") << endl;
}

void SpeccyScreen::joystickHorizontalAxis(uint_fast32_t id, bool l, bool r) {

    switch (id) {
        case 0:
            // Joystick #0 is mapped to Kempston/Fuller/Cursor/Sinclair Port 1
            switch (spectrum.joystick) {
                case JoystickType::KEMPSTON_OLD:
                case JoystickType::KEMPSTON_NEW:
                    spectrum.kempstonData &= 0xFC;
                    if (l) {
                        spectrum.kempstonData |= 0x02;
                    } else if (r) {
                        spectrum.kempstonData |= 0x01;
                    }
                    break;
                case JoystickType::FULLER:
                    spectrum.fullerData |= 0x0C;
                    if (l) {
                        spectrum.fullerData &= 0xFB;
                    } else if (r) {
                        spectrum.fullerData &= 0xF7;
                    }
                    break;
                case JoystickType::CURSOR:
                    mapKeyJoystickAxis(CURS, MOVE_L, MOVE_R, l, r);
                    break;
                default:
                    mapKeyJoystickAxis(SJS1, MOVE_L, MOVE_R, l, r);
                    break;
            }
            break;
        case 1:
            switch (spectrum.joystick) {
                case JoystickType::KEMPSTON_OLD:
                case JoystickType::KEMPSTON_NEW:
                case JoystickType::FULLER:
                    // Map secondary joystick to Sinclair Port 1.
                    mapKeyJoystickAxis(SJS1, MOVE_L, MOVE_R, l, r);
                    break;
                case JoystickType::CURSOR:
                    // Map secondary joystick to Kempston.
                    spectrum.kempstonData &= 0xFC;
                    if (l) {
                        spectrum.kempstonData |= 0x02;
                    } else if (r) {
                        spectrum.kempstonData |= 0x01;
                    }
                    break;
                default:
                    // Map secondary joystick to Sinclair Port 2.
                    mapKeyJoystickAxis(SJS2, MOVE_L, MOVE_R, l, r);
                    break;
            }
            break;
        default:
            break;
    }
}

void SpeccyScreen::joystickVerticalAxis(uint_fast32_t id, bool u, bool d) {

    switch (id) {
        case 0:
            // Joystick #0 is mapped to Kempston/Fuller/Cursor/Sinclair Port 1
            switch (spectrum.joystick) {
                case JoystickType::KEMPSTON_OLD:
                case JoystickType::KEMPSTON_NEW:
                    spectrum.kempstonData &= 0xF3;
                    if (u) {
                        spectrum.kempstonData |= 0x08;
                    } else if (d) {
                        spectrum.kempstonData |= 0x04;
                    }
                    break;
                case JoystickType::FULLER:
                    spectrum.fullerData |= 0x03;
                    if (u) {
                        spectrum.fullerData &= 0xFE;
                    } else if (d) {
                        spectrum.fullerData &= 0xFD;
                    }
                    break;
                case JoystickType::CURSOR:
                    mapKeyJoystickAxis(CURS, MOVE_U, MOVE_D, u, d);
                    break;
                default:
                    mapKeyJoystickAxis(SJS1, MOVE_U, MOVE_D, u, d);
                    break;
            }
            break;
        case 1:
            switch (spectrum.joystick) {
                case JoystickType::KEMPSTON_OLD:
                case JoystickType::KEMPSTON_NEW:
                case JoystickType::FULLER:
                    // Map secondary joystick to Sinclair Port 1.
                    mapKeyJoystickAxis(SJS1, MOVE_U, MOVE_D, u, d);
                    break;
                case JoystickType::CURSOR:
                    // Map secondary joystick to Kempston.
                    spectrum.kempstonData &= 0xF3;
                    if (u) {
                        spectrum.kempstonData |= 0x08;
                    } else if (d) {
                        spectrum.kempstonData |= 0x04;
                    }
                    break;
                default:
                    // Map secondary joystick to Sinclair Port 2.
                    mapKeyJoystickAxis(SJS2, MOVE_U, MOVE_D, u, d);
                    break;
            }
            break;
        default:
            break;
    }
}

void SpeccyScreen::joystickButtonPress(uint_fast32_t id, uint_fast32_t button) {

    if (pad) {
        button += 4;
        while (button > 9) {
            button -= 2;
        }
    } else {
        button = 4;  // Interpret every button as default.
    }

    switch (id) {
        case 0:
            if (button < FIRE_1) {
                switch (spectrum.joystick) {
                    case JoystickType::KEMPSTON_NEW:
                    case JoystickType::KEMPSTON_OLD:
                        spectrum.kempstonData |= 0x10;
                        break;
                    case JoystickType::FULLER:
                        spectrum.fullerData &= 0x7F;
                        break;
                    case JoystickType::CURSOR:
                        pressKeyJoystickButton(CURS, FIRE_0);
                        break;
                    default:
                        pressKeyJoystickButton(SJS1, FIRE_0);
                        break;
                }
            } else {
                pressKeyJoystickButton(SJS1, button);
            }
            break;
        case 1:
            if (button < FIRE_1) {
                switch (spectrum.joystick) {
                    case JoystickType::KEMPSTON_NEW:
                    case JoystickType::KEMPSTON_OLD:
                    case JoystickType::FULLER:
                        pressKeyJoystickButton(SJS1, FIRE_0);
                        break;
                    case JoystickType::CURSOR:
                        spectrum.kempstonData |= 0x10;
                        break;
                    default:
                        pressKeyJoystickButton(SJS2, FIRE_0);
                        break;
                }
            } else {
                pressKeyJoystickButton(SJS2, button);
            }
            break;
        default:
            break;
    }
}

void SpeccyScreen::joystickButtonRelease(uint_fast32_t id, uint_fast32_t button) {

    if (pad) {
        button += 4;
        while (button > 9) {
            button -= 2;
        }
    } else {
        button = 4;  // Interpret every button as default.
    }

    switch (id) {
        case 0:
            if (button < FIRE_1) {
                switch (spectrum.joystick) {
                    case JoystickType::KEMPSTON_NEW:
                    case JoystickType::KEMPSTON_OLD:
                        spectrum.kempstonData &= 0xEF;
                        break;
                    case JoystickType::FULLER:
                        spectrum.fullerData |= 0x80;
                        break;
                    case JoystickType::CURSOR:
                        releaseKeyJoystickButton(CURS, FIRE_0);
                        break;
                    default:
                        releaseKeyJoystickButton(SJS1, FIRE_0);
                        break;
                }
            } else {
                releaseKeyJoystickButton(SJS1, button);
            }
            break;
        case 1:
            if (button < FIRE_1) {
                switch (spectrum.joystick) {
                    case JoystickType::KEMPSTON_NEW:
                    case JoystickType::KEMPSTON_OLD:
                    case JoystickType::FULLER:
                        releaseKeyJoystickButton(SJS1, FIRE_0);
                        break;
                    case JoystickType::CURSOR:
                        spectrum.kempstonData &= 0xEF;
                        break;
                    default:
                        releaseKeyJoystickButton(SJS2, FIRE_0);
                        break;
                }
            } else {
                releaseKeyJoystickButton(SJS2, button);
            }
            break;
        default:
            break;
    }
}

void SpeccyScreen::keyPress(Keyboard::Scancode key) {

    try {
        InputMatrixPosition pos = zxSingleKeys.at(key);
        spectrum.ula.keys[pos.row] &= ~pos.key;
        return;
    } catch (out_of_range const& oor) {}

    try {
        InputMatrixPosition pos = zxCapsKeys.at(key);
        spectrum.ula.keys[pos.row] &= ~pos.key;
        spectrum.ula.keys[7] &= 0xFE;   // Press Caps Shift
        ++capsShiftPresses;
        return;
    } catch (out_of_range const& oor) {}

    try {
        InputMatrixPosition pos = zxSymbolKeys.at(key);
        spectrum.ula.keys[pos.row] &= ~pos.key;
        spectrum.ula.keys[0] &= 0xFD;   // Press Symbol Shift
        ++symbShiftPresses;
    } catch (out_of_range const& oor) {}
}

void SpeccyScreen::keyRelease(Keyboard::Scancode key) {

    try {
        InputMatrixPosition pos = zxSingleKeys.at(key);
        spectrum.ula.keys[pos.row] |= pos.key;
        return;
    } catch (out_of_range const& oor) {}

    try {
        InputMatrixPosition pos = zxCapsKeys.at(key);
        spectrum.ula.keys[pos.row] |= pos.key;
        if (--capsShiftPresses == 0) {
            spectrum.ula.keys[7] |= 0x01;   // Press Caps Shift
        }
        return;
    } catch (out_of_range const& oor) {}

    try {
        InputMatrixPosition pos = zxSymbolKeys.at(key);
        spectrum.ula.keys[pos.row] |= pos.key;
        if (--symbShiftPresses == 0) {
            spectrum.ula.keys[0] |= 0x02;   // Press Symbol Shift
        }
    } catch (out_of_range const& oor) {}
}

float SpeccyScreen::getPixelClock() {

    static float baseClocks[6] {
        BASE_CLOCK_48,  // 48K issue 2 (ULA 5C)
        BASE_CLOCK_48,  // 48K issue 3 (ULA 6C)
        BASE_CLOCK_128, // 128K toastrack (ULA 7K)
        BASE_CLOCK_128, // 128K +2 (Amstrad 40056)
        BASE_CLOCK_128, // 128K +2A/+3 (Amstrad 40057)
        BASE_CLOCK_48   // Pentagon
    };
    return 2 * baseClocks[spectrum.ula.ulaVersion] / 1000000.0;
}

void SpeccyScreen::mapKeyJoystickAxis(uint_fast32_t type,
        uint_fast32_t indexA, uint_fast32_t indexB, bool a, bool b) {
    spectrum.ula.keys[spectrumKeyJoystick[type][indexA].row] |= spectrumKeyJoystick[type][indexA].key;
    spectrum.ula.keys[spectrumKeyJoystick[type][indexB].row] |= spectrumKeyJoystick[type][indexB].key;
    if (a) {
        spectrum.ula.keys[spectrumKeyJoystick[type][indexA].row] &= ~spectrumKeyJoystick[type][indexA].key;
    } else if (b) {
        spectrum.ula.keys[spectrumKeyJoystick[type][indexB].row] &= ~spectrumKeyJoystick[type][indexB].key;
    }
}

void SpeccyScreen::pressKeyJoystickButton(uint_fast32_t type, uint_fast32_t button) {
    spectrum.ula.keys[spectrumKeyJoystick[type][button].row] &= ~spectrumKeyJoystick[type][button].key;
}

void SpeccyScreen::releaseKeyJoystickButton(uint_fast32_t type, uint_fast32_t button) {
    spectrum.ula.keys[spectrumKeyJoystick[type][button].row] |= spectrumKeyJoystick[type][button].key;
}
// vim: et:sw=4:ts=4
