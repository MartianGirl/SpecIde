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

enum class FD1793State {
    FD1793_STATE_IDLE,
    FD1793_STATE_COMMAND,
    FD1793_STATE_EXECUTION,
    FD1793_STATE_RECEIVE,
    FD1793_STATE_TRANSMIT,
    FD1793_STATE_RESULT
};

enum class FD1793Mode {
    FD1793_MODE_NONE,
    FD1793_MODE_READ,
    FD1793_MODE_WRITE
};

enum class FD1793Access {
    FD1793_ACCESS_LOAD,
    FD1793_ACCESS_SEEK,
    FD1793_ACCESS_DATA,
    FD1793_ACCESS_UNLOAD,
    FD1793_ACCESS_NONE
};

enum class FD1793Command {
    RESTORE,
    SEEK,
    STEP,
    STEP_IN,
    STEP_OUT,
    READ_SECTOR,
    WRITE_SECTOR,
    READ_ADDRESS,
    READ_TRACK,
    WRITE_TRACK,
    FORCE_INTERRUPT,
    INVALID
};


/** Status Reg b0 - BUSY (All types). */
uint8_t constexpr STATUS_BUSY = 1 << 0;

/** Status Reg b1 - INDEX (Type I). */
uint8_t constexpr STATUS_INDEX = 1 << 1;
/** Status Reg b1 - DATA_REQUEST (Type II & Type III). */
uint8_t constexpr STATUS_DRQ = 1 << 1;

/** Status Reg b2 - TRACK 0 (Type I). */
uint8_t constexpr STATUS_TRACK0 = 1 << 2;
/** Status Reg b2 - LOST DATA (Type II & Type III). */
uint8_t constexpr STATUS_LOSTDATA = 1 << 2;

/** Status Reg b3 - CRC ERROR (All Types, if reading). */
uint8_t constexpr STATUS_CRCERROR = 1 << 3;

/** Status Reg b4 - SEEK ERROR (Type I). */
uint8_t constexpr STATUS_SEEKERROR = 1 << 4;
/** Status Reg b4 - RECORD NOT FOUND (Type II & Type III). */
uint8_t constexpr STATUS_RNF = 1 << 4;

/** Status Reg b5 - HEAD LOADED (Type I). */
uint8_t constexpr STATUS_HEADLOADED = 1 << 5;
/** Status Reg b5 - RECORD TYPE (Type II, Type III, reading). */
uint8_t constexpr STATUS_RECORDTYPE = 1 << 5;
/** Status Reg b5 - WRITE_FAULT (Type II, Type III, writing). */
uint8_t constexpr STATUS_WRITEFAULT = 1 << 5;

/** Status Reg b6 - WRITE PROTECT (All types). */
uint8_t constexpr STATUS_WRITEPROT = 1 << 6;

/** Status Reg b7 - NOT READY (All types). */
uint8_t constexpr STATUS_NOTREADY = 1 << 7;

uint32_t constexpr DELAY_1ms = 875;     // Clocking at 1.000MHz
uint32_t constexpr SERVICE_MFM = 46;
uint32_t constexpr SERVICE_FM = 91;
uint32_t constexpr BYTE_DELAY = 100;

uint_fast32_t MAX_TRDOS_DRIVES = 4;

class FD1793 {

    public:
        /** Track number of the current read/write head position. */
        uint_fast8_t trackReg;
        /** Address of the desired sector position. */
        uint_fast8_t sectorReg;
        /** Device status information. */
        uint_fast8_t statusReg;
        /** Command presently being executed. */
        uint_fast8_t commandReg;
        uint_fast8_t systemReg;
        uint_fast8_t dataBuffer[65536];

        bool commandNew = false;

        FD1793State state = FD1793State::FD1793_STATE_IDLE;
        FD1793Mode mode;
        FD1793Access stage;
        FD1793Command command = FD1793Command::INVALID;

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

        void checkCommand();

        void prepareType1();
        void prepareType2();
        void prepareType3();
        void prepareType4();
};

// vim: et:sw=4:ts=4
