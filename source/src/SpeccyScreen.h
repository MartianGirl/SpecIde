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
         * Poll window interface events.
         */
        void pollEvents();
};
// vim: et:sw=4:ts=4
