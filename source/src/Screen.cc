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

#include "Screen.h"
#include "config.h"

#include <cfenv>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

Screen::Screen(map<string, string> o, vector<string> f) :
    options(o),
    files(f) {}

#if (SPECIDE_SDL2==1)
#else
using namespace sf;

Screen::~Screen() {

    SoundChannel::getChannel().close();
    window.close();
}

void Screen::loadFont(string const& fontName) {

    vector<string> fontPaths;
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
        success = guiFont.loadFromFile(font);
        ++j;
    } while (!success && j < fontPaths.size());

    if (!success) {
        cout << "Could not load menu font." << endl;
        assert(false);
    }
}

void Screen::chooseVideoMode() {

    modes = sf::VideoMode::getFullscreenModes();
    bestMode = sf::VideoMode::getDesktopMode();

    cout << "Selected Full Screen Mode: ";
    cout << bestMode.width << "x" << bestMode.height << "-" << bestMode.bitsPerPixel << endl;
    adjustViewPort();
}

void Screen::reopenWindow(bool fs) {

    char str[64];
    snprintf(str, 64, "SpecIde [%s(%s)]",
            SPECIDE_BUILD_DATE, SPECIDE_BUILD_COMMIT);

    if (fs) {
        window.create(bestMode, str, sf::Style::Fullscreen);
    } else {
        window.create(
                sf::VideoMode(static_cast<sf::Uint32>(w), static_cast<sf::Uint32>(h)),
                str, sf::Style::Close | sf::Style::Titlebar);
    }
}

void Screen::setFullScreen(bool fs) {

    fullscreen = fs;

    uint_fast32_t displayedScans = 270;
    uint_fast32_t totalScans = 312;

    float pixelRatio = 14.75 / getPixelClock();
    yScale = static_cast<float>(bestMode.height) / static_cast<float>(displayedScans);
    xScale = pixelRatio * yScale;

    uint_fast32_t yModifier = doubleScanMode ? 2 : 1;
    uint_fast32_t xModifier = wide ? 2 : 1;
    yScale /= yModifier;
    xScale /= xModifier;

    uint_fast32_t start = (totalScans - displayedScans) / 2;
    uint_fast32_t lines = displayedScans;

    if (fs) {
        // Use best mode available.
        fesetround(FE_TONEAREST);

        // Adjust depending on the vertical scale.
        xOffset = (bestMode.width - xScale * (xSize - lBorder - rBorder)) / 2;
        yOffset = 0;

        cout << "Using scale " << fixed << setprecision(3) << yScale << " " << xScale << endl;

        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(lBorder, (start + tBorder) * yModifier,
                    xSize - lBorder - rBorder, (lines - bBorder) * yModifier));
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(xScale, yScale));
    } else {
        // In this case we want to have the same windows size for double scan modes and
        // single scan modes. 588 displayable lines are assumed.
        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(lBorder, (start + tBorder) * yModifier,
                    xSize - lBorder - rBorder, (lines - bBorder) * yModifier));
        scrSprite.setPosition(0, 0);
        scrSprite.setScale(Vector2f(2 * pixelRatio / xModifier * static_cast<float>(scale),
                    2 * static_cast<float>(scale) / yModifier));
    }

    window.setVerticalSyncEnabled(syncToVideo);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);
    window.setJoystickThreshold(0.5);
}

void Screen::texture(uint_fast32_t x, uint_fast32_t y) {

    cout << "Allocating texture..." << endl;
    if (!scrTexture.create(static_cast<Uint32>(x), static_cast<Uint32>(y))) {
        assert(false);
    }
    scrTexture.setRepeated(false);
    scrTexture.setSmooth(true);
}
#endif

void Screen::setup() {

    scale = getScale();
    w *= scale;
    h *= scale;

    chooseVideoMode();

    fullscreen = (options["fullscreen"] == "yes");
    cout << "Full screen mode: " << options["fullscreen"] << endl;

#if (SPECIDE_ON_MACOS == 0)
    syncToVideo = (options["sync"] == "yes");
    cout << "Sync to video: " << options["sync"] << endl;
#endif

    SoundChannel::getChannel().open(2, SAMPLE_RATE);
    SoundChannel::getChannel().setSleepInterval(getNumber("soundsleep", 10));
    cout << "Initialized " << 2 << " channels ";
    cout << "at " << SAMPLE_RATE << " Hz." << endl;
}

uint32_t Screen::getNumber(string const& key, uint32_t value) {

    if (options.find(key) != options.end()) {
        try {
            value = stoi(options[key]);
        } catch (invalid_argument &ia) {
            cout << "Invalid " << key << " value: '" << options[key] << "' - " << ia.what() << endl;
            cout << "Using value " << value << " for option " << key << endl;
        }
    }
    return value;
}

uint32_t Screen::getScale() {

    uint32_t s = 1;
    try {
        s = stoi(options["scale"]);
    } catch (invalid_argument &ia) {
        cout << "Invalid scale value: '" << options["scale"] << "' - " << ia.what() << endl;
    }

    if (s < 1) {
        s = 1;
    } else if (s > 10) {
        s = 10;
    }
    cout << "Selected scale factor: " << s << endl;
    return s;
}

void Screen::adjustViewPort() {

    uint32_t divider = 0;
    do {
        ++divider;
        suggestedScansSingle = bestMode.height / divider;
    } while (suggestedScansSingle > 288); // Aim for 288 visible rasters.
    cout << "Selected " << suggestedScansSingle << " scans for single scan mode." << endl;

    divider = 0;
    do {
        ++divider;
        suggestedScansDouble = bestMode.height / divider;
    } while (suggestedScansDouble > 576); // Aim for 576 visible rasters.
    cout << "Selected " << suggestedScansDouble << " scans for double scan mode." << endl;
}

FileTypes Screen::guessFileType(string const& fileName) {

    // Parse the file name, find the extension. We'll decide what to do
    // based on this.
    size_t dot = fileName.find_last_of('.');
    string extension;
    if (dot != string::npos) {
        // Get the extension in lowercase characters.
        extension = fileName.substr(dot);
        for (size_t ii = 0; ii < extension.size(); ++ii) {
            extension[ii] = tolower(extension[ii]);
        }
    }

    if (extension == ".tzx") {
        return FileTypes::FILETYPE_TZX;
    } else if (extension == ".cdt") {
        return FileTypes::FILETYPE_CDT;
    } else if (extension == ".tap") {
        return FileTypes::FILETYPE_TAP;
    } else if (extension == ".pzx") {
        return FileTypes::FILETYPE_PZX;
    } else if (extension == ".dsk") {
        return FileTypes::FILETYPE_DSK;
    } else if (extension == ".csw") {
        return FileTypes::FILETYPE_CSW;
    } else if (extension == ".z80") {
        return FileTypes::FILETYPE_Z80;
    } else if (extension == ".sna") {
        return FileTypes::FILETYPE_SNA;
    } else if (extension == ".pal") {
        return FileTypes::FILETYPE_PAL;
    } else {
        return FileTypes::FILETYPE_ERR;
    }
}

void Screen::pollEvents() {

    Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                close();
                break;

            case Event::KeyPressed:
                switch (event.key.scancode) {
                    case Keyboard::Scan::Menu:  // fall-through
                    case Keyboard::Scan::F1:    // Show menu
                        menu = true;
                        break;
                    case Keyboard::Scan::F2:    // Window/Fullscreen
                        fullscreen = !fullscreen;
                        reopenWindow(fullscreen);
                        setFullScreen(fullscreen);
                        break;
                    case Keyboard::Scan::F3:    // Save DSK to disk
                        if (event.key.shift) {
                            createEmptyDisk();
                        } else {
                            saveDisk();
                        }
                        break;
                    case Keyboard::Scan::F4:    // Select DSK from list
                        if (event.key.shift) {
                            selectPreviousDisk();
                        } else {
                            selectNextDisk();
                        }
                        break;
                    case Keyboard::Scan::F5:    // Reset Computer
                        reset();
                        break;
                    case Keyboard::Scan::F6:    // Clear save data
                        if (event.key.shift) {
                            appendLoadTape();
                        } else {
                            clearSaveTape();
                        }
                        break;
                    case Keyboard::Scan::F7:
                        if (event.key.shift) {
                            selectSaveTape();   // Use save data to load
                        } else {
                            writeSaveTape();    // Write save data
                        }
                        break;
                    case Keyboard::Scan::F8:
                        if (event.key.shift) {
                            togglePsgType();    // Toggle PSG chip type
                        } else {
                            toggleFlashTap();   // Toggle flashloading of tapes
                        }
                        break;
                    case Keyboard::Scan::F9:    // Toggle sound ON/OFF
                        if (event.key.shift) {
                            toggleTapeSound();
                        } else {
                            toggleSound();
                        }
                        break;
                    case Keyboard::Scan::F10:   // Quit
                        done = true;
                        break;
                    case Keyboard::Scan::F11:   // Play/Stop tape
                        if (event.key.shift) {
                            resetTapeCounter();
                        } else {
                            startStopTape();
                        }
                        break;
                    case Keyboard::Scan::F12:
                        if (event.key.shift) {
                            rewindTape(true);
                        } else {
                            rewindTape(false);
                        }
                        break;

                    default:
                        keyPress(event.key.scancode);
                        break;
                }
                break;

            case Event::KeyReleased:
                keyRelease(event.key.scancode);
                break;

            case Event::JoystickMoved:
                switch (event.joystickMove.axis) {
                    case Joystick::X:
                    case Joystick::U:
                    case Joystick::PovX:
                        if (event.joystickMove.position < -34.0) {
                            joystickHorizontalAxis(event.joystickMove.joystickId, true, false);
                        } else if (event.joystickMove.position > 34.0) {
                            joystickHorizontalAxis(event.joystickMove.joystickId, false, true);
                        } else {
                            joystickHorizontalAxis(event.joystickMove.joystickId, false, false);
                        }
                        break;

                    case Joystick::Y:
                    case Joystick::V:
                    case Joystick::PovY:
                        if (event.joystickMove.position < -34.0) {
                            joystickVerticalAxis(event.joystickMove.joystickId, true, false);
                        } else if (event.joystickMove.position > 34.0) {
                            joystickVerticalAxis(event.joystickMove.joystickId, false, true);
                        } else {
                            joystickVerticalAxis(event.joystickMove.joystickId, false, false);
                        }
                        break;

                    default:
                        break;
                }
                break;

            case Event::JoystickButtonPressed:
                joystickButtonPress(event.joystickButton.joystickId, event.joystickButton.button);
                break;

            case Event::JoystickButtonReleased:
                joystickButtonRelease(event.joystickButton.joystickId, event.joystickButton.button);
                break;

            default:
                break;
        }
    }
}
// vim: et:sw=4:ts=4
