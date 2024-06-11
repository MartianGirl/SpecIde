/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2024.
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

#include <cstdint>
#include <map>
#include <string>
#include <vector>

/**
 * Display Emulator's License text.
 */
void displayLicense();

/**
 * Display command line help text.
 */
void displayHelp();

/**
 * Get list of directories that will be searched for ROM files.
 *
 * @return vector<string> with the list of directories.
 */
std::vector<std::string> getRomDirs();

/**
 * Check if a ZX Spectrum is emulated.
 *
 * @return true if the selected model is a ZX Spectrum.
 */
bool isSpectrum(std::string const& model);

/**
 * Check if an Amstrad CPC is emulated.
 *
 * @return true if the selected model is an Amstrad CPC.
 */
bool isCpc(std::string const& model);

/**
 * Read the options from the config file.
 *
 * options map<string, string> of name-value pairs where the options
 *         will be stored.
 */
void readOptions(std::map<std::string, std::string>& options);

/**
 * Option name-value pair structure.
 */
struct Option {

    /** Option name. */
    std::string name;
    /** Option value. */
    std::string value;

    /**
     * Constructor
     *
     * @param n Option name.
     * @param v Option value.
     */
    Option(std::string n = "", std::string v = "") : name(n), value(v) {}
};

// vim: et:sw=4:ts=4
