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
 * SpeccyScreen
 *
 * ZX Spectrum Screen implementation.
 *
 * This class emulates a CRT connected to a ZX Spectrum computer. This class
 * is responsible for the settings that are specific to ZX Spectrum machines,
 * and for the menu interface for those.
 */

#include "Screen.h"
#include "SoundDefs.h"
#include "Spectrum.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <queue>
#include <vector>

class SpeccyScreen : public Screen {

    public:
        /**
         * Constructor.
         *
         * This constructor takes a map of options and values. The configuration
         * of the emulation will be based on these parameters.
         *
         * @param opt A map<string, string> of name-value pairs.
         */
        SpeccyScreen(std::map<std::string, std::string> o,
                std::vector<std::string> f);

        /**
         * Destructor.
         */
        ~SpeccyScreen();

        /** Emulated ZX Spectrum. */
        Spectrum spectrum;

        /** Number of double keys pressing Caps Shift */
        uint32_t capsShiftPresses = 0;
        /** Number of double keys pressing Symbol Shift */
        uint32_t symbShiftPresses = 0;

        /**
         * Initialise the Screen object with the parameters received on construction.
         *
         * This function initialises only the specific part for a ZX Spectrum.
         */
        void setup();
        
        /**
         * Run the ZX Spectrum emulation.
         */
        void run();

        /**
         * Exit the ZX Spectrum emulation.
         */
        void close();

        /**
         * Reset the emulator.
         */
        void reset();

        /**
         * Load tape and disk files.
         */
        void loadFiles();

        /**
         * Update screen after each frame.
         */
        void update();

        /**
         * Draw the menu.
         */
        void updateMenu();

        /**
         * Create empty disk.
         */
        void createEmptyDisk();

        /**
         * Save current disk.
         */
        void saveDisk();

        /**
         * Select previous disk from disk list.
         */
        void selectPreviousDisk();

        /**
         * Select next disk.
         */
        void selectNextDisk();

        /**
         * Append load data to save tape.
         */
        void appendLoadTape();

        /**
         * Clear save tape.
         */
        void clearSaveTape();

        /**
         * Write save tape to disk.
         */
        void writeSaveTape();

        /**
         * Toggle save tape or load tape to load data from it.
         */
        void selectSaveTape();

        /**
         * Reset tape counter.
         *
         * This sets a mark in the tape so we can rewind to this point.
         */
        void resetTapeCounter();

        /**
         * Start or Stop tape.
         */
        void startStopTape();

        /**
         * Rewind tape.
         *
         * @param toCounter Rewinds tape to the previously set mark.
         */
        void rewindTape(bool toCounter);

        /**
         * Toggle sound on/off.
         */
        void toggleSound();

        /**
         * Toggle tape sound on/off.
         */
        void toggleTapeSound();

        /**
         * Toggle PSG type.
         */
        void togglePsgType();

        /**
         * Toggle Flash loading of tapes.
         */
        void toggleFlashTap();

        /**
         * Move joystick horizontal axis.
         *
         * @param id Joystick that emits the event.
         * @param l Activate left bit.
         * @param r Activate right bit.
         */
        void joystickHorizontalAxis(uint_fast32_t id, bool l, bool r);

        /**
         * Move joystick vertical axis.
         *
         * @param id Joystick that emits the event.
         * @param u Activate up bit.
         * @param d Activate down bit.
         */
        void joystickVerticalAxis(uint_fast32_t id, bool u, bool d);

        /**
         * Press joystick button.
         *
         * @param id Joystick that emits the event.
         * @param button Joystick button to press.
         */
        void joystickButtonPress(uint_fast32_t id, uint_fast32_t button);

        /**
         * Release joystick button.
         *
         * @param id Joystick that emits the event.
         * @param button Joystick button to release.
         */
        void joystickButtonRelease(uint_fast32_t id, uint_fast32_t button);

        /**
         * Press key.
         *
         * @param key Key to press.
         */
        void keyPress(sf::Keyboard::Scancode key);

        /**
         * Release key.
         *
         * @param key Key to release.
         */
        void keyRelease(sf::Keyboard::Scancode key);

        /**
         * Get pixel clock.
         *
         * @return Pixel clock, in kHz.
         */
        float getPixelClock();

        /**
         * Map a joystick move event to its key binding.
         *
         * @param type The keybinding table entry (SJS1, SJS2, Cursor)
         * @param indexA The entry in the table for direction A (e.g. DOWN)
         * @param indexB The entry in the table for direction B (e.g. UP)
         * @param a Direction A status.
         * @param b Direction B status.
         */
        void mapKeyJoystickAxis(uint_fast32_t type,
                uint_fast32_t indexA, uint_fast32_t indexB, bool a, bool b);

        void pressKeyJoystickButton(uint_fast32_t type, uint_fast32_t button);

        void releaseKeyJoystickButton(uint_fast32_t type, uint_fast32_t button);
};
// vim: et:sw=4:ts=4
