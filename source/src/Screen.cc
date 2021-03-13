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

#include "Screen.h"
#include "SoundDefs.h"
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

Screen::Screen(size_t scale) :
    w(688 * scale), h(576 * scale),
    window(),
    modes(sf::VideoMode::getFullscreenModes()),
    bestMode(sf::VideoMode::getDesktopMode()),
    skip(ULA_CLOCK_48 / SAMPLE_RATE),
    pulse(0), sample(skip),
    fullscreen(false), doubleScanMode(false), smooth(false),
    aychip(true),
    syncToVideo(false),
    scale(scale),
    xSize(360), ySize(625),
    delay(FRAME_TIME_48),
    pad(false),
    flashTap(false) {

    cout << "Selected Full Screen Mode: " << bestMode.width
        << "x" << bestMode.height << "-" << bestMode.bitsPerPixel << endl;
    adjust();

    // Create a texture.
    // For Amstrad CPC and Timex computers, the texture should be at least 720px wide.
    texture(xSize, ySize);

    // Load the ZX font, for the menu.
    vector<string> fontPaths;
    string fontName("ZXSpectrum.ttf");
    char* pHome = getenv(SPECIDE_HOME_ENV);

    fontPaths.push_back("");
    if (pHome != nullptr) {
#if (SPECIDE_ON_UNIX==1)
        string home(pHome);
        home += string("/") + string(SPECIDE_CONF_DIR) + string("/font/");
        fontPaths.push_back(home);

        string fontHome(pHome);
        fontHome += string("/.fonts/truetype/");
        fontPaths.push_back(fontHome);
#else
        string home(pHome);
        home += string("\\") + string(SPECIDE_CONF_DIR) + string("\\font\\");
        fontPaths.push_back(home);
#endif
    }
#if (SPECIDE_ON_UNIX==1)
    fontPaths.push_back("/usr/local/share/fonts/truetype/");
    fontPaths.push_back("/usr/share/fonts/truetype/");
#endif

    size_t j = 0;
    bool success = false;
    do {
        string font = fontPaths[j] + fontName;
        cout << "Trying font: " << font << endl;
        success = zxFont.loadFromFile(font);
        ++j;
    } while (!success && j < fontPaths.size());

    if (!success) {
        cout << "Could not load menu font." << endl;
        assert(false);
    }

    // ULA parameters should go in an ZX Spectrum specific init function.
    spectrum.ula.xSize = xSize;
    spectrum.ula.ySize = ySize;

    channel.open(2, SAMPLE_RATE);
}

void Screen::run() {

    steady_clock::time_point tick = steady_clock::now();
    steady_clock::time_point frame;
    steady_clock::time_point wakeup;

    for (;;) {
        for (;;) {
            // Update Spectrum hardware
            clock();

            if (spectrum.ula.vSync) {
                // Update the screen
                update();

                // Delay starting playing sound (if disabled) until first frame
                // is complete
                if (!streaming) {
                    streaming = true;
                    channel.play();
                }

                if (!syncToVideo) {
                    // By not sleeping until the next frame is due, we get some
                    // better adjustment
#ifdef USE_BOOST_THREADS
                    frame = tick + boost::chrono::microseconds(delay);
                    wakeup = tick + boost::chrono::microseconds(18000);
#else
                    frame = tick + std::chrono::microseconds(delay);
                    wakeup = tick + std::chrono::microseconds(18000);
#endif
#ifndef DO_NOT_SLEEP
                    sleep_until(wakeup);
#endif
                    while ((tick = steady_clock::now()) < frame);
                }

                if (done || menu) break;
            } else if (spectrum.ula.keyPoll) {
                pollEvents();
                pollCommands();
            }
        }

        // Disable sound for menus
        channel.stop();
        streaming = false;
        if (done) return;

        for (;;) {
            // Menu thingy
            updateMenu();

            if (!syncToVideo) {
#ifdef USE_BOOST_THREADS
                sleep_until(tick + boost::chrono::microseconds(delay));
#else
                sleep_until(tick + std::chrono::microseconds(delay));
#endif
                tick = steady_clock::now();
            }
            if (done) return;
            if (!menu) break;
        }
    }
}

void Screen::clock() {

    static double remaining = 0;

    if (flashTap) {
        checkTapeTraps();
    }

    spectrum.clock();

    if (tape.playing) {
        if (!pulse--) {
            spectrum.ula.tapeIn = tape.advance() | 0x80;
            pulse = tape.sample;
        }
    } else {
        spectrum.ula.tapeIn &= 0x7F;
    }

    // Generate sound
    if (!(--sample)) {
        sample = skip;
        remaining += tail;
        if (remaining >= 1.0) {
            ++sample;
            remaining -= 1.0;
        }

        spectrum.sample();
        channel.push(spectrum.l, spectrum.r);
    }
}

void Screen::update() {

    // These conditions cannot happen at the same time:
    // - HSYNC and VSYNC only happen during the blanking interval.
    // - VSYNC happens at the end of blanking interval. (0x140)
    // - HSYNC happens at the beginning of HSYNC interval. (0x170-0x178)
    // If not blanking, draw.

    spectrum.ula.vSync = false;

    scrTexture.update(reinterpret_cast<Uint8*>(doubleScanMode ?
                spectrum.ula.pixelsX2 : spectrum.ula.pixelsX1));
    window.clear(Color::Black);
    window.draw(scrSprite);
    window.display();

    if (tape.pulseData.size()) {
        char str[64];
        unsigned int percent = 100 * tape.pointer / tape.pulseData.size();
        snprintf(str, 64, "SpecIde %d.%d.%d [%03u%%]",
                SPECIDE_VERSION_MAJOR,
                SPECIDE_VERSION_MINOR,
                SPECIDE_VERSION_TWEAK,
                percent);
        window.setTitle(str);
    }

    tape.is48K = spectrum.set48;
}

void Screen::updateMenu() {

    RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setFillColor(Color::White - Color(0, 0, 0, 64));
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(Color::Black);
    rectangle.setSize(Vector2f(324, 232));
    if (fullscreen) {
        rectangle.setPosition(xOffset + 6 * sScale, yOffset + 6 * sScale);
        rectangle.setScale(Vector2f(sScale, sScale));
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
        text.setCharacterSize(static_cast<uint32_t>(8 * sScale));
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
            if (event.key.code == Keyboard::F1) {
                menu = false;
            }
        }
    }
}

void Screen::reopenWindow(bool fs) {

    char str[64];
    snprintf(str, 64, "SpecIde %d.%d.%d [NO TAPE]",
            SPECIDE_VERSION_MAJOR,
            SPECIDE_VERSION_MINOR,
            SPECIDE_VERSION_TWEAK);

    window.close();

    if (fs) {
        window.create(bestMode, str, sf::Style::Fullscreen);
    } else {
        window.create(
                sf::VideoMode(static_cast<sf::Uint32>(w), static_cast<sf::Uint32>(h)),
                str, sf::Style::Close | sf::Style::Titlebar);
    }
}

void Screen::setFullScreen(bool fs) {

    size_t suggestedScans = doubleScanMode ? suggestedScansDouble : suggestedScansSingle;
    size_t xModifier = doubleScanMode ? 2 : 1;
    size_t yModifier = doubleScanMode ? 1 : 2;
    size_t totalScans = doubleScanMode ? 625 : 312;

    if (fs) {
        // Use best mode available.
        fesetround(FE_TONEAREST);
        xScale = bestMode.width / static_cast<float>(xSize);
        yScale = nearbyintf(bestMode.height / static_cast<float>(suggestedScans));

        // Adjust depending on the vertical scale.
        sScale = yScale;
        xOffset = (bestMode.width - (xModifier * xSize * sScale)) / 2;
        yOffset = 0;

        cout << "XScale: " << fixed << setprecision(3) << xScale << " ";
        cout << "YScale: " << fixed << setprecision(3) << yScale << endl;
        cout << "Using scale " << fixed << setprecision(3) << sScale << endl;

        size_t start = (totalScans - suggestedScans) / 2;
        size_t lines = bestMode.height;

        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(8, static_cast<uint_fast32_t>(start),
                    static_cast<uint_fast32_t>(xSize - 8), static_cast<uint_fast32_t>(lines)));
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(xModifier * sScale, sScale));
    } else {
        // In this case we want to have the same windows size for double scan modes and
        // single scan modes. 588 displayable lines are assumed.
        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(8, 16 / yModifier,
                    static_cast<uint_fast32_t>(xSize - 8), 588 / yModifier));
        scrSprite.setPosition(0, 0);
        scrSprite.setScale(Vector2f(2 * static_cast<float>(scale), yModifier * static_cast<float>(scale)));
    }

    window.setVerticalSyncEnabled(syncToVideo);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);
    window.setJoystickThreshold(0.5);
}

void Screen::setSmooth(bool sm) {

    scrTexture.setSmooth(sm);
}

void Screen::setTapeSound(bool value) {

    tapeSound = value;
    spectrum.ula.tapeSound = !tape.playing || tapeSound;
}

void Screen::pollEvents() {

    spectrum.ula.keyPoll = false;

    Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                channel.stop();
                done = true;
                break;

            case Event::KeyPressed:
                switch (event.key.code) {
                    case Keyboard::F1:  // Show menu
                        menu = true;
                        break;
                    case Keyboard::F2:  // Window/Fullscreen
                        if (event.key.shift) {
                            smooth = !smooth;
                            setSmooth(smooth);
                        } else {
                            fullscreen = !fullscreen;
                            reopenWindow(fullscreen);
                            setFullScreen(fullscreen);
                        }
                        break;
                    case Keyboard::F3:  // Save DSK to disk
                        if (event.key.shift) {
                            spectrum.fdc765.drive[0].emptyDisk();
                        } else {
                            spectrum.fdc765.drive[0].saveDisk();
                        }
                        break;
                    case Keyboard::F4:  // Select DSK from list
                        if (event.key.shift) {
                            spectrum.fdc765.drive[0].prevDisk();
                        } else {
                            spectrum.fdc765.drive[0].nextDisk();
                        }
                        break;
                    case Keyboard::F5:  // Reset Spectrum
                        spectrum.reset();
                        break;
                    case Keyboard::F6:  // Clear save data
                        if (event.key.shift) {
                            tape.appendLoadData();
                        } else {
                            tape.clearSaveData();
                        }
                        break;
                    case Keyboard::F7:  // Write save data
                        if (event.key.shift) {
                            tape.useSaveData = !tape.useSaveData;
                            tape.selectTapData();
                        } else {
                            tape.writeSaveData();
                        }
                        break;
                    case Keyboard::F8:  // PSG chip type
                        aychip = !aychip;
                        spectrum.psgChip(aychip);
                        break;
                    case Keyboard::F9:  // Toggle sound ON/OFF
                        if (event.key.shift) {
                            setTapeSound(!tapeSound);
                        } else {
                            playSound = !playSound;
                            spectrum.ula.playSound = playSound;
                            spectrum.psgPlaySound(psgSound && playSound);
                        }
                        break;
                    case Keyboard::F10: // Quit
                        done = true;
                        break;
                    case Keyboard::F11: // Play/Stop tape
                        if (event.key.shift) {
                            tape.resetCounter();
                        } else {
                            tape.play();
                            setTapeSound(tapeSound);
                        }
                        break;
                    case Keyboard::F12:
                        if (event.key.shift) {
                            tape.rewind(tape.counter);
                        } else {
                            tape.rewind();
                        }
                        break;

                        // Scan Spectrum keyboard
                    default:
                        scanKeys(event);
                        for (size_t ii = 0; ii < 8; ++ii) {
                            spectrum.ula.keys[ii] &= keyboardMask[ii];
                        }
                        break;
                }
                break;

            case Event::KeyReleased:
                scanKeys(event);
                for (size_t ii = 0; ii < 8; ++ii) {
                    spectrum.ula.keys[ii] |= ~keyboardMask[ii];
                }
                break;

            case Event::JoystickMoved:
                switch (event.joystickMove.axis) {
                    case Joystick::X:
                    case Joystick::U:
                    case Joystick::PovX:
                        if (spectrum.kempston) {
                            spectrum.joystick &= 0xFC;
                            if (event.joystickMove.position < -34.0) {
                                spectrum.joystick |= 0x02;
                            } else if (event.joystickMove.position > 34.0) {
                                spectrum.joystick |= 0x01;
                            }
                        } else {
                            spectrum.ula.keys[3] |= 0x018;
                            if (event.joystickMove.position < -34.0) {
                                spectrum.ula.keys[3] &= 0xEF;
                            } else if (event.joystickMove.position > 34.0) {
                                spectrum.ula.keys[3] &= 0xF7;
                            }
                        }
                        break;

                    case Joystick::Y:
                    case Joystick::V:
                    case Joystick::PovY:
                        if (spectrum.kempston) {
                            spectrum.joystick &= 0xF3;
                            if (event.joystickMove.position < -34.0) {
                                spectrum.joystick |= 0x08;
                            } else if (event.joystickMove.position > 34.0) {
                                spectrum.joystick |= 0x04;
                            }
                        } else {
                            spectrum.ula.keys[3] |= 0x06;
                            if (event.joystickMove.position < -34.0) {
                                spectrum.ula.keys[3] &= 0xFD;
                            } else if (event.joystickMove.position > 34.0) {
                                spectrum.ula.keys[3] &= 0xFB;
                            }
                        }
                        break;

                    default:
                        break;
                }
                break;

            case Event::JoystickButtonPressed:
                if (pad) {
                    switch (event.joystickButton.button) {
                        case 1: // Emulate press 'B'
                            spectrum.ula.keys[0] &= 0xEF; break;
                        case 2: // Emulate press 'N'
                            spectrum.ula.keys[0] &= 0xF7; break;
                        case 3: // Emulate press 'V'
                            spectrum.ula.keys[7] &= 0xEF; break;
                        case 4: // Emulate press 'C'
                            spectrum.ula.keys[7] &= 0xF7; break;
                        case 5: // Emulate press 'X'
                            spectrum.ula.keys[7] &= 0xFB; break;
                        case 6: // Emulate press 'G'
                            spectrum.ula.keys[6] &= 0xEF; break;
                        case 7: // Emulate press 'H'
                            spectrum.ula.keys[1] &= 0xEF; break;
                        default:    // Other buttons map to the joystick.
                            if (spectrum.kempston) {
                                spectrum.joystick |= 0x10;
                            } else {
                                spectrum.ula.keys[3] &= 0xFE;   // '0'
                            }
                            break;
                    }
                } else {
                    if (spectrum.kempston) {
                        spectrum.joystick |= 0x10;
                    } else {
                        spectrum.ula.keys[3] &= 0xFE;
                    }
                }
                break;

            case Event::JoystickButtonReleased:
                if (pad) {
                    switch (event.joystickButton.button) {
                        case 1: // Emulate release 'B'
                            spectrum.ula.keys[0] |= 0x10; break;
                        case 2: // Emulate release 'N'
                            spectrum.ula.keys[0] |= 0x08; break;
                        case 3: // Emulate release 'V'
                            spectrum.ula.keys[7] |= 0x10; break;
                        case 4: // Emulate release 'C'
                            spectrum.ula.keys[7] |= 0x08; break;
                        case 5: // Emulate release 'X'
                            spectrum.ula.keys[7] |= 0x04; break;
                        case 6: // Emulate release 'G'
                            spectrum.ula.keys[6] |= 0x10; break;
                        case 7: // Emulate release 'H'
                            spectrum.ula.keys[1] |= 0x10; break;
                        default:    // Other buttons map to the joystick.
                            if (spectrum.kempston) {
                                spectrum.joystick &= 0xEF;
                            } else {
                                spectrum.ula.keys[3] |= 0x01;
                            }
                            break;
                    }
                } else {
                    if (spectrum.kempston) {
                        spectrum.joystick &= 0xEF;
                    } else {
                        spectrum.ula.keys[3] |= 0x01;
                    }
                }
                break;

            default:
                break;
        }
    }
}

void Screen::scanKeys(Event const& event) {

    static bool lshift = false;
    static bool rshift = false;

    for (size_t ii = 0; ii < 8; ++ii) {
        keyboardMask[ii] = 0xFF;
    }

    switch (event.key.code) {
        case Keyboard::B:
            keyboardMask[0] = 0xEF; break;
        case Keyboard::N:
            keyboardMask[0] = 0xF7; break;
        case Keyboard::M:
            keyboardMask[0] = 0xFB; break;
        case Keyboard::LControl:    // Symbol Shift
        case Keyboard::RControl:
            keyboardMask[0] = 0xFD; break;
        case Keyboard::Space:
            keyboardMask[0] = 0xFE; break;

        case Keyboard::Comma:       // Symbol Shift + N
            keyboardMask[0] = 0xF5; break;
        case Keyboard::Period:      // Symbol Shift + M
            keyboardMask[0] = 0xF9; break;
        case Keyboard::Tab:         // Extend Mode = Caps Shift + Symbol Shift
            keyboardMask[0] = 0xFD;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::Escape:      // Break = Caps Shift + Space
            keyboardMask[0] = 0xFE;
            keyboardMask[7] = 0xFE; break;

        case Keyboard::H:
            keyboardMask[1] = 0xEF; break;
        case Keyboard::J:
            keyboardMask[1] = 0xF7; break;
        case Keyboard::K:
            keyboardMask[1] = 0xFB; break;
        case Keyboard::L:
            keyboardMask[1] = 0xFD; break;
        case Keyboard::Return:
            keyboardMask[1] = 0xFE; break;

        case Keyboard::Y:
            keyboardMask[2] = 0xEF; break;
        case Keyboard::U:
            keyboardMask[2] = 0xF7; break;
        case Keyboard::I:
            keyboardMask[2] = 0xFB; break;
        case Keyboard::O:
            keyboardMask[2] = 0xFD; break;
        case Keyboard::P:
            keyboardMask[2] = 0xFE; break;

        case Keyboard::Quote:       // Symbol Shift + P
            keyboardMask[0] = 0xFD;
            keyboardMask[2] = 0xFE; break;

        case Keyboard::Num6:
            keyboardMask[3] = 0xEF; break;
        case Keyboard::Num7:
            keyboardMask[3] = 0xF7; break;
        case Keyboard::Num8:
            keyboardMask[3] = 0xFB; break;
        case Keyboard::Num9:
            keyboardMask[3] = 0xFD; break;
        case Keyboard::Num0:
            keyboardMask[3] = 0xFE; break;

        case Keyboard::Down:        // Caps Shift + 6
            keyboardMask[3] = 0xEF;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::Up:          // Caps Shift + 7
            keyboardMask[3] = 0xF7;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::Right:       // Caps Shift + 8
            keyboardMask[3] = 0xFB;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::Insert:      // Graph Mode: Caps Shift + 9
            keyboardMask[3] = 0xFD;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::BackSpace:   // Delete: Caps Shift + 0
            keyboardMask[3] = 0xFE;
            keyboardMask[7] = 0xFE; break;

        case Keyboard::Num5:
            keyboardMask[4] = 0xEF; break;
        case Keyboard::Num4:
            keyboardMask[4] = 0xF7; break;
        case Keyboard::Num3:
            keyboardMask[4] = 0xFB; break;
        case Keyboard::Num2:
            keyboardMask[4] = 0xFD; break;
        case Keyboard::Num1:
            keyboardMask[4] = 0xFE; break;

        case Keyboard::Left:        // Caps Shift + 5
            keyboardMask[4] = 0xEF;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::Home:        // Inv Video: Caps Shift + 4
            keyboardMask[4] = 0xF7;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::End:         // True Video: Caps Shift + 3
            keyboardMask[4] = 0xFB;
            keyboardMask[7] = 0xFE; break;
        case Keyboard::Delete:   // Edit: Caps Shift + 1
            keyboardMask[4] = 0xFE;
            keyboardMask[7] = 0xFE; break;

        case Keyboard::T:
            keyboardMask[5] = 0xEF; break;
        case Keyboard::R:
            keyboardMask[5] = 0xF7; break;
        case Keyboard::E:
            keyboardMask[5] = 0xFB; break;
        case Keyboard::W:
            keyboardMask[5] = 0xFD; break;
        case Keyboard::Q:
            keyboardMask[5] = 0xFE; break;

        case Keyboard::G:
            keyboardMask[6] = 0xEF; break;
        case Keyboard::F:
            keyboardMask[6] = 0xF7; break;
        case Keyboard::D:
            keyboardMask[6] = 0xFB; break;
        case Keyboard::S:
            keyboardMask[6] = 0xFD; break;
        case Keyboard::A:
            keyboardMask[6] = 0xFE; break;

        case Keyboard::V:
            keyboardMask[7] = 0xEF; break;
        case Keyboard::C:
            keyboardMask[7] = 0xF7; break;
        case Keyboard::X:
            keyboardMask[7] = 0xFB; break;
        case Keyboard::Z:
            keyboardMask[7] = 0xFD; break;
        case Keyboard::LShift:      // Handle Caps Lock (Caps Shift + 2) when
                                    // both Shifts are pressed
            keyboardMask[7] = 0xFE;
            if (event.type == Event::KeyPressed) {
                lshift = true;
                if (rshift) keyboardMask[4] = 0xFD;
            } else if (event.type == Event::KeyReleased) {
                lshift = false;
                if (rshift) keyboardMask[4] = 0xFD;
            }
            break;
        case Keyboard::RShift:
            keyboardMask[7] = 0xFE;
            if (event.type == Event::KeyPressed) {
                rshift = true;
                if (lshift) keyboardMask[4] = 0xFD;
            } else if (event.type == Event::KeyReleased) {
                rshift = false;
                if (lshift) keyboardMask[4] = 0xFD;
            }
            break;

        default:
            break;
    }
}

void Screen::texture(size_t x, size_t y) {

    cout << "Allocating texture..." << endl;
    if (!scrTexture.create(static_cast<Uint32>(x), static_cast<Uint32>(y))) {
        assert(false);
    }
    scrTexture.setRepeated(false);
    scrTexture.setSmooth(false);
}

void Screen::adjust() {

    size_t divider = 0;

    do {
        ++divider;
        suggestedScansSingle = bestMode.height / divider;
    } while (suggestedScansSingle > 304); // 312 - 8 VBlank lines.
    cout << "Selected " << suggestedScansSingle
        << " scans for single scan mode." << endl;

    divider = 0;
    do {
        ++divider;
        suggestedScansDouble = bestMode.height / divider;
    } while (suggestedScansDouble > 608); // 624 - 16 VBlank lines.
    cout << "Selected " << suggestedScansDouble
        << " scans for double scan mode." << endl;
}

void Screen::setSoundRate(SoundRate rate) {

    double value = 0;
    switch (rate) {
        case SoundRate::SOUNDRATE_128K:
            value = static_cast<double>(ULA_CLOCK_128) / static_cast<double>(SAMPLE_RATE);
            delay = FRAME_TIME_128;
            break;
        case SoundRate::SOUNDRATE_PENTAGON:
            value = static_cast<double>(ULA_CLOCK_48) / static_cast<double>(SAMPLE_RATE);
            delay = FRAME_TIME_PENTAGON;
            break;
        default:
            value = static_cast<double>(ULA_CLOCK_48) / static_cast<double>(SAMPLE_RATE);
            delay = FRAME_TIME_48;
            break;
    }
    skip = static_cast<uint32_t>(value);
    tail = value - skip;
    sample = skip;
}

bool Screen::cpuInRefresh() {

    return (spectrum.z80.state == Z80State::ST_OCF_T4L_RFSH2);
}

void Screen::checkTapeTraps() {

    switch (spectrum.z80.pc.w) {
        case 0x056D:    // LD_START
            if (spectrum.rom48 && cpuInRefresh() && tape.tapData.size()) {
                trapLdStart();
            }
            break;

        case 0x04D1:    // SA_FLAG
            if (spectrum.rom48 && cpuInRefresh()) {
                trapSaBytes();
            }
            break;

        default:
            break;
    }
}

void Screen::writeMemory(uint_fast16_t a, uint_fast8_t d) {

    a &= 0xFFFF;
    if (a > 0x3FFF) { // Don't write ROM.
        spectrum.map[a >> 14][a & 0x3FFF] = d;
    }
}

uint_fast8_t Screen::readMemory(uint_fast16_t a) {

    a &= 0xFFFF;
    return spectrum.map[a >> 14][a & 0x3FFF];
}

void Screen::trapLdStart() {

    // Find first block that matches flag byte (Flag is in AF')
    while (tape.foundTapBlock(spectrum.z80.af_.b.h) == false) {
        tape.nextTapBlock();
    }

    // Get parameters from CPU registers
    uint16_t start = spectrum.z80.ix.w;
    uint16_t bytes = spectrum.z80.de.w;
    uint16_t block = tape.getBlockLength();

    if (block < bytes) {
        // Load error if not enough bytes.
        spectrum.z80.af.b.l &= 0xFE;
    } else {
        block = bytes;
        spectrum.z80.af.b.l |= 0x01;
    }
    spectrum.z80.ix.w = (spectrum.z80.ix.w + block) & 0xFFFF;
    spectrum.z80.de.w -= block;

    // Dump block to memory.
    for (uint_fast16_t ii = 0; ii < block; ++ii) {
        writeMemory(start + ii, tape.getBlockByte(ii + 3));
    }

    // Advance tape
    tape.nextTapBlock();

    // Force RET
    spectrum.z80.decode(0xC9);
    spectrum.z80.startInstruction();

    if (tape.tapPointer == 0) {
        tape.rewind();
    }
}

void Screen::trapSaBytes() {

    uint16_t start = spectrum.z80.ix.w;
    uint16_t bytes = spectrum.z80.de.w + 2;
    uint8_t checksum;

    tape.saveData.push_back(bytes & 0x00FF);
    tape.saveData.push_back((bytes & 0xFF00) >> 8);
    tape.saveData.push_back(spectrum.z80.af.b.h);
    bytes -= 2;

    spectrum.z80.ix.w = (spectrum.z80.ix.w + bytes) & 0xFFFF;
    spectrum.z80.de.w -= bytes;

    checksum = spectrum.z80.af.b.h;
    for (uint_fast16_t ii = 0; ii < bytes; ++ii) {
        uint_fast8_t byte = readMemory(start + ii);
        tape.saveData.push_back(byte);
        checksum ^= byte;
    }
    
    tape.saveData.push_back(checksum);

    // Force RET
    spectrum.z80.decode(0xC9);
    spectrum.z80.startInstruction();
}

void Screen::pollCommands() {

    string cmd = console.console.get();
    if (!cmd.empty()) {
        if (cmd == "exit") {
            done = true;
        } else {
            cout << "Syntax error: " << cmd << endl;
        }
    }
}

// vim: et:sw=4:ts=4
