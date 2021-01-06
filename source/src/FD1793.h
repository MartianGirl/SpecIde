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

constexpr uint_fast8_t SREG_DB0 = 1 << 0;
constexpr uint_fast8_t SREG_DB1 = 1 << 1;
constexpr uint_fast8_t SREG_DB2 = 1 << 2;
constexpr uint_fast8_t SREG_DB3 = 1 << 3;
constexpr uint_fast8_t SREG_CB = 1 << 4;
constexpr uint_fast8_t SREG_EXM = 1 << 5;
constexpr uint_fast8_t SREG_DIO = 1 << 6;
constexpr uint_fast8_t SREG_RQM = 1 << 7;

constexpr size_t DELAY_1ms = 875;     // Clocking at 1.000MHz
constexpr size_t SERVICE_MFM = 46;
constexpr size_t SERVICE_FM = 91;
constexpr size_t BYTE_DELAY = 100;

class FD1793 {

    public:
        uint_fast8_t trackReg;
        uint_fast8_t sectorReg;
        uint_fast8_t dataBuffer[65536];

        FD1793State state;
        FD1793Mode mode;
        FD1793Access stage;

        TRDosDrive drive[4];

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
