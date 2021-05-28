/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2020.
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

/** FD1793
 *
 * WD1793 implementation.
 *
 * This is the controller used in BetaDisk interface.
 */

#include <cstdint>

#include ".h"

enum class FD1793State {

    FD1793_STATE_IDLE,
    FD1793_STATE_COMMAND,
    FD1793_STATE_EXECUTION,
    FD1793_STATE_RECEIVE,
    FD1793_STATE_TRANSMIT,
    FD1793_STATE_RESULT
};

enum class FD1793Mode
{
    FD1793_MODE_NONE,
    FD1793_MODE_READ,
    FD1793_MODE_WRITE
};

enum class FD1793Access
{
    FD1793_ACCESS_LOAD,
    FD1793_ACCESS_SEEK,
    FD1793_ACCESS_DATA,
    FD1793_ACCESS_UNLOAD,
    FD1793_ACCESS_NONE
};

enum class FD1793Scan
{
    FD1793_SCAN_EQUAL,
    FD1793_SCAN_LOW,
    FD1793_SCAN_HIGH,
    FD1793_SCAN_ERROR
};

uint32_t constexpr DELAY_1ms = 875;     // Clocking at 1.000MHz
uint32_t constexpr SERVICE_MFM = 46;
uint32_t constexpr SERVICE_FM = 91;
uint32_t constexpr BYTE_DELAY = 100;

uint_fast32_t MAX_TRDOS_DRIVES = 2;

class FD1793 {

    public:
        uint_fast8_t trackReg;
        uint_fast8_t sectorReg;
        uint_fast8_t statusReg;
        uint_fast8_t commandReg;
        uint_fast8_t systemReg;
        uint_fast8_t dataBuffer[65536];

        FD1793State state;
        FD1793Mode mode;
        FD1793Access stage;

        TRDosDrive drive[MAX_TRDOS_DRIVES];
        uint_fast8_t presCylNum[MAX_TRDOS_DRIVES];

        FD1793() :
            drive{TRDosDrive(true), TRDosDrive(false), TRDosDrive(false), TRDosDrive(false)},
            presCylNum{0, 0, 0, 0}
        {}

        void clock();
        void reset();

        uint_fast8_t read(uint_fast8_t addr);
        void write(uint_fast8_t addr, uint_fast8_t value);
};

// vim: et:sw=4:ts=4
