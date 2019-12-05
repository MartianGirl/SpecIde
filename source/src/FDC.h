/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2019.
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

/** FDC
 *
 * FDC765 implementation.
 *
 * - Operates in DMA/non-DMA mode. I will implement first (only?) non-DMA,
 *   which is needed for Spectrum +3 and Amstrad CPC 6128.
 */

#include <cstdint>

#include "DiskDrive.h"

enum class FDCState
{
    FDC_STATE_IDLE,
    FDC_STATE_COMMAND,
    FDC_STATE_EXECUTION,
    FDC_STATE_RECEIVE,
    FDC_STATE_TRANSMIT,
    FDC_STATE_RESULT
};

enum class FDCMode
{
    FDC_MODE_NONE,
    FDC_MODE_READ,
    FDC_MODE_WRITE
};

enum class FDCAccess
{
    FDC_ACCESS_LOAD,
    FDC_ACCESS_SEEK,
    FDC_ACCESS_DATA,
    FDC_ACCESS_UNLOAD,
    FDC_ACCESS_NONE
};

enum class FDCScan
{
    FDC_SCAN_EQUAL,
    FDC_SCAN_LOW,
    FDC_SCAN_HIGH,
    FDC_SCAN_ERROR
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

class FDC {

    public:
        uint_fast8_t statusReg = 0x00;

        uint_fast8_t cmdBuffer[16];
        uint_fast8_t resBuffer[16];
        uint_fast8_t dataBuffer[65536];

        unsigned int cmdIndex;
        unsigned int resIndex;
        unsigned int dataIndex;

        unsigned int cmdBytes;
        unsigned int resBytes;
        unsigned int dataBytes;

        bool byte = false;
        bool interrupt = false;

        bool multiTrackBit;
        bool mfmModeBit;
        bool skipDeletedBit;
        bool useDeletedDAM;

        bool idmFound;
        bool ddmFound;
        bool eocFound;
        bool secFound;
        bool loaded = false;
        bool unload = false;
        bool transfer = false;
        size_t loadTimer = 0;
        size_t unloadTimer = 0;
        size_t serviceTimer = 0;
        size_t byteTimer = 0;
        size_t sectorCopy = 0;

        FDCState state;
        FDCMode mode;
        FDCAccess stage;

        DiskDrive drive[2];
        uint_fast8_t presCylNum[2];
        uint_fast8_t sReg[4];

        size_t lastDrive;
        size_t firstSector = 0x00;
        size_t currSector = 0x00;
        size_t lastSector = 0x00;

        size_t resTrack = 0x00;
        size_t resHead = 0x00;
        size_t resSector = 0x00;
        size_t resSize = 0x00;

        size_t headUnloadTime;
        size_t stepRateTime;
        size_t headLoadTime;
        bool useDma;

        FDC() :
            drive{DiskDrive(true), DiskDrive(false)},
            presCylNum{0, 0}
        {}

        void clock();
        void reset();
        void checkCommand();
        void setup();
        void execute();

        bool seekForReadOp();
        bool seekForWriteOp();
        bool readOp();
        void setResultBytesOp();
        bool readRegularDataOp();
        bool readDeletedDataOp();
        bool readIdOp();
        bool headLoadOp();
        bool findHoleOp();
        bool readTrackOp();

        void readCmd();
        void readIdCmd();
        void readTrackCmd();
        void writeCmd();
        void specifyCmd();

        uint_fast8_t read();
        void write(uint_fast8_t value);

        uint_fast8_t status() { return statusReg; }
        size_t cmdDrive() { return (cmdBuffer[1] & 0x01); } // 2 drives max.
        size_t cmdHead() { return ((cmdBuffer[1] & 0x04) >> 2); }

        void checkDrive();
        void motor(bool status);
        void randomizeSector(std::vector<uint8_t>& buf);
};

// vim: et:sw=4:ts=4
