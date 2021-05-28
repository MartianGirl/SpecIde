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

    window.close();
}

void Screen::loadFont() {

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

void Screen::setFullScreen(bool fs, bool wide) {

    fullscreen = fs;

    uint_fast32_t suggestedScans = doubleScanMode ? suggestedScansDouble : suggestedScansSingle;
    uint_fast32_t wModifier = wide ? 1 : 2;
    uint_fast32_t xModifier = doubleScanMode ? wModifier : 1;
    uint_fast32_t yModifier = doubleScanMode ? 1 : 2;
    uint_fast32_t totalScans = doubleScanMode ? 625 : 312;

    if (fs) {
        // Use best mode available.
        fesetround(FE_TONEAREST);
        xScale = bestMode.width / static_cast<float>(xSize);
        yScale = nearbyintf(bestMode.height / static_cast<float>(suggestedScans));

        // Adjust depending on the vertical scale.
        xOffset = (bestMode.width - (xModifier * xSize * yScale * wModifier / 2)) / 2;
        yOffset = 0;

        cout << "XScale: " << fixed << setprecision(3) << xScale << " ";
        cout << "YScale: " << fixed << setprecision(3) << yScale << endl;
        cout << "Using scale " << fixed << setprecision(3) << yScale << endl;

        uint_fast32_t start = (totalScans - suggestedScans) / 2;
        uint_fast32_t lines = bestMode.height;

        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(8, static_cast<uint_fast32_t>(start),
                    static_cast<uint_fast32_t>(xSize - 8), static_cast<uint_fast32_t>(lines)));
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(xModifier * wModifier * yScale / 2, yScale));
    } else {
        // In this case we want to have the same windows size for double scan modes and
        // single scan modes. 588 displayable lines are assumed.
        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(8, 16 / yModifier,
                    static_cast<uint_fast32_t>(xSize - 8), 588 / yModifier));
        scrSprite.setPosition(0, 0);
        scrSprite.setScale(Vector2f(wModifier * static_cast<float>(scale), yModifier * static_cast<float>(scale)));
    }

    window.setVerticalSyncEnabled(syncToVideo);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);
    window.setJoystickThreshold(0.5);
}

void Screen::setSmooth(bool sm) {

    scrTexture.setSmooth(sm);
}

void Screen::texture(uint_fast32_t x, uint_fast32_t y) {

    cout << "Allocating texture..." << endl;
    if (!scrTexture.create(static_cast<Uint32>(x), static_cast<Uint32>(y))) {
        assert(false);
    }
    scrTexture.setRepeated(false);
    scrTexture.setSmooth(false);
}
#endif

void Screen::setup() {

    scale = getScale();
    w *= scale;
    h *= scale;

    loadFont();
    chooseVideoMode();

    fullscreen = (options["fullscreen"] == "yes");
    cout << "Full screen mode: " << options["fullscreen"] << endl;

    smooth = (options["antialias"] == "yes");
    cout << "Antialiasing: " << options["antialias"] << endl;

    syncToVideo = (options["sync"] == "yes");
    cout << "Sync to video: " << options["sync"] << endl;
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
    } while (suggestedScansSingle > 304); // 312 - 8 VBlank lines.
    cout << "Selected " << suggestedScansSingle << " scans for single scan mode." << endl;

    divider = 0;
    do {
        ++divider;
        suggestedScansDouble = bestMode.height / divider;
    } while (suggestedScansDouble > 608); // 624 - 16 VBlank lines.
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
        return FileTypes::FILETYPE_TZX;
    } else if (extension == ".tap") {
        return FileTypes::FILETYPE_TAP;
    } else if (extension == ".dsk") {
        return FileTypes::FILETYPE_DSK;
    } else if (extension == ".csw") {
        return FileTypes::FILETYPE_CSW;
    } else {
        return FileTypes::FILETYPE_ERR;
    }
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
