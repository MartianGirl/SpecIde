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

/** FDC765
 *
 * FDC765 implementation.
 *
 * - Operates in DMA/non-DMA mode. I will implement first (only?) non-DMA,
 *   which is needed for Spectrum +3 and Amstrad CPC 6128.
 */

#include <cstdint>

#include "Plus3Disk.h"

enum class FDC765State
{
    FDC765_STATE_IDLE,
    FDC765_STATE_COMMAND,
    FDC765_STATE_EXECUTION,
    FDC765_STATE_RECEIVE,
    FDC765_STATE_TRANSMIT,
    FDC765_STATE_RESULT
};

enum class FDC765Mode
{
    FDC765_MODE_NONE,
    FDC765_MODE_READ,
    FDC765_MODE_WRITE
};

enum class FDC765Access
{
    FDC765_ACCESS_LOAD,
    FDC765_ACCESS_SEEK,
    FDC765_ACCESS_DATA,
    FDC765_ACCESS_UNLOAD,
    FDC765_ACCESS_NONE
};

enum class FDC765Scan
{
    FDC765_SCAN_EQUAL,
    FDC765_SCAN_LOW,
    FDC765_SCAN_HIGH,
    FDC765_SCAN_ERROR
};

uint_fast8_t constexpr SREG_DB0 = 1 << 0;
uint_fast8_t constexpr SREG_DB1 = 1 << 1;
uint_fast8_t constexpr SREG_DB2 = 1 << 2;
uint_fast8_t constexpr SREG_DB3 = 1 << 3;
uint_fast8_t constexpr SREG_CB = 1 << 4;
uint_fast8_t constexpr SREG_EXM = 1 << 5;
uint_fast8_t constexpr SREG_DIO = 1 << 6;
uint_fast8_t constexpr SREG_RQM = 1 << 7;

uint32_t constexpr DELAY_1ms = 1000;     // Clocking at 1.000MHz
uint32_t constexpr SERVICE_MFM = 53;
uint32_t constexpr SERVICE_FM = 104;
uint32_t constexpr BYTE_DELAY = 114;

uint32_t constexpr DATABUFFER_SIZE = 65536;
uint32_t constexpr RESBUFFER_SIZE = 16;
uint32_t constexpr CMDBUFFER_SIZE = 16;

uint32_t constexpr MAX_PLUS3_DRIVES = 2;
uint32_t constexpr NUM_REGS = 4;

class FDC765 {

    public:
        /** Clock frequency in MHz. */
        float clockFrequency = 1;
        /** Status register. */
        uint_fast8_t statusReg = 0x00;

        /** Command buffer. */
        uint8_t cmdBuffer[CMDBUFFER_SIZE];
        /** Result buffer. */
        uint8_t resBuffer[RESBUFFER_SIZE];
        /** Data buffer. */
        uint8_t dataBuffer[DATABUFFER_SIZE];

        /** Command buffer index. */
        uint32_t cmdIndex;
        /** Result buffer index. */
        uint32_t resIndex;
        /** Data buffer index. */
        uint32_t dataIndex;

        /** Command expected length. */
        uint32_t cmdBytes;
        /** Result expected length. */
        uint32_t resBytes;
        /** Data transfer expected length. */
        uint32_t dataBytes;


        /** Byte ready flag. */
        bool byte = false;
        /** Interrupt flag. */
        bool interrupt = false;

        bool multiTrackBit;
        bool mfmModeBit;
        bool skipDeletedBit;
        bool useDeletedDAM;

        /** Flag: ID Address Mark found. */
        bool idmFound;
        /** Flag: Data Address Mark or Deleted Data Address Mark found. */
        bool ddmFound;
        /** Flag: End of Cylinder found. */
        bool eocFound;
        /** Flag: Sector found. */
        bool secFound;
        bool loaded = false;
        bool unload = false;
        bool transfer = false;
        size_t loadTimer = 0;
        size_t unloadTimer = 0;
        size_t serviceTimer = 0;
        size_t byteTimer = 0;
        size_t sectorCopy = 0;

        FDC765State state;
        FDC765Mode mode;
        FDC765Access stage;

        Plus3Disk drive[MAX_PLUS3_DRIVES];
        uint_fast8_t presCylNum[MAX_PLUS3_DRIVES];
        uint_fast8_t sReg[NUM_REGS];

        size_t lastDrive;
        size_t firstSector = 0x00;
        size_t currSector = 0x00;
        size_t lastSector = 0x00;
        size_t stepSector = 0x01;

        uint_fast8_t resTrack = 0x00;
        uint_fast8_t resHead = 0x00;
        uint_fast8_t resSector = 0x00;
        uint_fast8_t resSize = 0x00;

        size_t headUnloadTime;
        size_t stepRateTime;
        size_t headLoadTime;
        bool useDma;

        FDC765() :
            drive{Plus3Disk(true), Plus3Disk(false)},
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
        void updateHeadUnload();

        bool findHoleOp();
        bool readTrackOp();

        void readCmd();
        void readIdCmd();
        void readTrackCmd();
        void writeCmd();
        void formatCmd();
        void specifyCmd();
        void scanCmd();

        uint_fast8_t read();
        void write(uint_fast8_t value);

        uint_fast8_t status();
        uint_fast8_t cmdDrive() { return (cmdBuffer[1] & 0x01); } // 2 drives max.
        uint_fast8_t cmdHead() { return ((cmdBuffer[1] & 0x04) >> 2); }

        void checkDrive();
        void motor(bool status);
        void randomizeSector(std::vector<uint8_t>& buf);
        void appendToDataBuffer(std::vector<uint8_t>& buf);
};

// vim: et:sw=4:ts=4
