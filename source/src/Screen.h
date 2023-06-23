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

#pragma once

/**
 * Screen
 *
 * Base Screen implementation.
 *
 * This class emulates a CRT. It draws an image by scanning the pixels
 * in a zig-zag way, just like a TV screen does. It handles the frame
 * time and synchronization.
 *
 * Depending on which computer is selected, different sets of options
 * will be considered.
 */

#include "CommonDefs.h"
#include "Console.h"

#if (SPECIDE_SDL2==1)
#else
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#endif

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <map>
#include <vector>

class Screen {

    public:
        /**
         * Constructor.
         *
         * This constructor takes a map of options and values. The configuration
         * of the emulation will be based on these parameters.
         *
         * @param opt A map<string, string> of name-value pairs.
         */
        Screen(std::map<std::string, std::string> o,
                std::vector<std::string> f);

        /**
         * Destructor. Closes the window.
         */
        virtual ~Screen();

        /** Map of options as name-value pairs. */
        std::map<std::string, std::string> options;
        /** Vector of file names as strings. */
        std::vector<std::string> files;

#if (SPECIDE_SDL2==1)
#else
        /** SFML Window object. */
        sf::RenderWindow window;
        /** Best video mode. It uses the resolution of the desktop. */
        sf::VideoMode bestMode;
        /** Texture. */
        sf::Texture scrTexture;
        /** Sprite. Contains the texture to be drawn. */
        sf::Sprite scrSprite;
        /** ZX Spectrum font for drawing the menus. */
        sf::Font zxFont;
        /** Vector of available video modes. */
        std::vector<sf::VideoMode> modes;
#endif
        /** Window width. */
        uint32_t w = 704;
        /** Window height. */
        uint32_t h = 540;
        /** Window scale factor. */
        uint32_t scale;

        uint_fast32_t lBorder = 0;
        uint_fast32_t rBorder = 0;
        uint_fast32_t tBorder = 0;
        uint_fast32_t bBorder = 0;

        /** Suggested drawable scans for better fit on single scan modes. */
        uint32_t suggestedScansSingle;
        /** Suggested drawable scans for better fit on double scan modes. */
        uint32_t suggestedScansDouble;

        /** Full screen mode active. */
        bool fullscreen = false;
        /** Sync to video mode active. */
        bool syncToVideo = false;
        /** Use a wide screen mode. */
        bool wide = false;

        /** Sound flag. */
        bool playSound = true;
        /** Tape sound flag. */
        bool tapeSound = true;
        /** PSG sound flag. */
        bool psgSound = true;
        /** Exit emulation flag. */
        bool done = false;
        /** Menu mode flag. */
        bool menu = false;

        /** Type of PSG. */
        bool aychip = true;
        /** Map game pad extra buttons to keys. */
        bool pad = false;

        /** Use interlaced two-pass mode. */
        bool doubleScanMode = false;
        /** Texture horizontal size. */
        uint32_t xSize = 360;
        /** Texture vertical size. */
        uint32_t ySize = 625;
        /** Horizontal offset of the texture in full screen mode. */
        float xOffset;
        /** Vertical offset of the texture in full screen mode. */
        float yOffset;
        /** Horizontal stretch of the texture. */
        float xScale;
        /** Vertical stretch of the texture. */
        float yScale;

        /** Thread for a text console. */
        ConsoleThread console;

        /**
         * Initialise the Screen object with the parameters received on construction.
         *
         * This function initialises only the common part, and then it calls the
         * appropriate function for the selected machine.
         */
        virtual void setup();

        /**
         * Convert the "scale" option into an integer.
         */
        uint32_t getScale();
        
        /**
         * Select the native video mode and find its resolution.
         */
        void chooseVideoMode();

        /**
         * Decide how many lines are going to be drawn, based on the resolution.
         */
        void adjustViewPort();

        /**
         * Load fonts for the menu.
         */
        void loadFont();
        
        /**
         * Create texture for drawing.
         *
         * @param x Texture width.
         * @param y Texture height.
         */
        void texture(uint_fast32_t x, uint_fast32_t y);

        /**
         * Execute the emulation loop.
         */
        virtual void run() = 0;

        /**
         * Exit the emulation loop.
         */
        virtual void close() = 0;

        /**
         * Reset the emulator.
         */
        virtual void reset() = 0;

        /**
         * Create empty disk.
         */
        virtual void createEmptyDisk() = 0;

        /**
         * Save current disk.
         */
        virtual void saveDisk() = 0;

        /**
         * Select previous disk from disk list.
         */
        virtual void selectPreviousDisk() = 0;

        /**
         * Select next disk.
         */
        virtual void selectNextDisk() = 0;

        /**
         * Append load data to save tape.
         */
        virtual void appendLoadTape() = 0;

        /**
         * Clear save tape.
         */
        virtual void clearSaveTape() = 0;

        /**
         * Write save tape to disk.
         */
        virtual void writeSaveTape() = 0;

        /**
         * Toggle save tape or load tape to load data from it.
         */
        virtual void selectSaveTape() = 0;

        /**
         * Reset tape counter.
         *
         * This sets a mark in the tape so we can rewind to this point.
         */
        virtual void resetTapeCounter() = 0;

        /**
         * Start or Stop tape.
         */
        virtual void startStopTape() = 0;

        /**
         * Rewind tape.
         *
         * @param toCounter Rewinds tape to the previously set mark.
         */
        virtual void rewindTape(bool toCounter) = 0;

        /**
         * Toggle sound on/off.
         */
        virtual void toggleSound() = 0;

        /**
         * Toggle tape sound on/off.
         */
        virtual void toggleTapeSound() = 0;

        /**
         * Toggle PSG type.
         */
        virtual void togglePsgType() = 0;

        /**
         * Move joystick horizontal axis.
         *
         * @param l Activate left bit.
         * @param r Activate right bit.
         */
        virtual void joystickHorizontalAxis(bool l, bool r) = 0;

        /**
         * Move joystick vertical axis.
         *
         * @param u Activate up bit.
         * @param d Activate down bit.
         */
        virtual void joystickVerticalAxis(bool u, bool d) = 0;

        /**
         * Press joystick button.
         *
         * @param button Joystick button to press.
         */
        virtual void joystickButtonPress(uint_fast32_t button) = 0;

        /**
         * Release joystick button.
         *
         * @param button Joystick button to release.
         */
        virtual void joystickButtonRelease(uint_fast32_t button) = 0;

        /**
         * Press key.
         *
         * @param key Key to press.
         */
        virtual void keyPress(sf::Keyboard::Scancode key) = 0;

        /**
         * Release key.
         *
         * @param key Key to release.
         */
        virtual void keyRelease(sf::Keyboard::Scancode key) = 0;

        /**
         * Guess file type based on the extension.
         */
        FileTypes guessFileType(std::string const& filename);

        /**
         * Recreate the emulator window after toggling full screen mode.
         *
         * @param fs Reopen the window in full screen or windowed mode.
         */
        void reopenWindow(bool fs);

        /**
         * Change texture parameters for either full screen mode or windowed.
         *
         * @param fs Set parameters for full screen or windowed mode.
         */
        void setFullScreen(bool fs);

        /**
         * Poll window interface events.
         */
        void pollEvents();

        /**
         * Read commands from the text console.
         */
        void pollCommands();

        /**
         * Get pixel clock.
         *
         * @return Pixel clock, in kHz.
         */
        virtual float getPixelClock() = 0;
};
// vim: et:sw=4:ts=4
