#pragma once

/** FDC
 *
 * FDC765 implementation.
 *
 * - Operates in DMA/non-DMA mode. I will implement first (only?) non-DMA,
 *   which is needed for Spectrum +3 and Amstrad CPC 6128.
 */

#include "DSKFile.h"

#include <cassert>
#include <cstdint>
// #include <cstdio>

using namespace std;

enum class FDCState
{
    FDC_STATE_IDLE,
    FDC_STATE_COMMAND,
    FDC_STATE_EXECUTION,
    FDC_STATE_RECEIVE,
    FDC_STATE_RESULT
};

enum class FDCMode
{
    FDC_MODE_NONE,
    FDC_MODE_READ,
    FDC_MODE_WRITE
};

constexpr uint_fast8_t SREG_DB0 = 1 << 0;
constexpr uint_fast8_t SREG_DB1 = 1 << 1;
constexpr uint_fast8_t SREG_DB2 = 1 << 2;
constexpr uint_fast8_t SREG_DB3 = 1 << 3;
constexpr uint_fast8_t SREG_CB = 1 << 4;
constexpr uint_fast8_t SREG_EXM = 1 << 5;
constexpr uint_fast8_t SREG_DIO = 1 << 6;
constexpr uint_fast8_t SREG_RQM = 1 << 7;


constexpr uint_fast32_t DELAY_10us = 70;    // @ 7MHz

class DiskDrive
{
    public:
        DiskDrive() {}

        bool track0 = true;
        bool fault = false;
        bool disk = false;
        bool writeprot = false;
        bool present = false;
        bool ready = false;

        int cylinder = 0;
        int sector = 0;

        DSKFile image;

        void readTrack(uint_fast8_t* cmd, uint_fast8_t* res, uint_fast8_t* data)
        {

        }
        void specifySpeed(uint_fast8_t cmd);

        uint_fast8_t senseStatus()
        {
            return (track0 ? 0x10 : 0x00)
                | (ready ? 0x20 : 0x00)
                | (writeprot ? 0x40 : 0x00)
                | (fault ? 0x80 : 0x00);        // Don't really know...
        }

        void writeSector(uint_fast8_t* cmd, uint_fast8_t* res);
        void readSector(uint_fast8_t* cmd, uint_fast8_t* res);

        void recalibrate()
        {
            // Maybe put here a delay.
            if (cylinder < 78)
            {
                cylinder = 0;
                track0 = true;
            }
            else
            {
                cylinder -= 77;
            }
        }

        void writeDeleted(uint_fast8_t* cmd, uint_fast8_t* res);

        uint_fast8_t readId(uint_fast8_t* cmd, uint_fast8_t* res, uint_fast8_t* data)
        {
            uint_fast8_t head = (cmd[1] & 0x04) >> 2;
            if (disk)
            {
                if (cylinder < image.numTracks)
                {
                    if (head < image.numSides)
                    {
                        int index = (image.numSides * cylinder) + head;
                        if (sector < image.tracks[index].numSectors)
                        {
                            data[0] = image.tracks[index].sectors[sector].sectorId;
                            res[0] = cmd[1] & 0x07;
                            res[1] = image.tracks[index].sectors[sector].fdcStatusReg1;
                            res[2] = image.tracks[index].sectors[sector].fdcStatusReg2;
                            res[3] = cylinder;
                            res[4] = head;
                            res[5] = sector;
                            res[6] = image.tracks[index].sectors[sector].sectorLength;
                            ++sector;
                        }
                    }
                }
            }
        }

        void readDeleted(uint_fast8_t* cmd, uint_fast8_t* res);
        void formatTrack(uint_fast8_t* cmd, uint_fast8_t* res);
        void seekTrack(uint_fast8_t* cmd, uint_fast8_t* res);
        void scanEqual(uint_fast8_t* cmd, uint_fast8_t* res);
        void scanLow(uint_fast8_t* cmd, uint_fast8_t* res);
        void scanHigh(uint_fast8_t* cmd, uint_fast8_t* res);
};

class FDC
{
    public:
        uint_fast8_t statusReg = 0x00;
        uint_fast8_t sReg[4];

        uint_fast8_t presCylinder = 0;

        uint_fast8_t cmdBuffer[16];
        uint_fast8_t resBuffer[16];
        uint_fast8_t dataBuffer[16384];

        unsigned int cmdIndex;
        unsigned int resIndex;
        unsigned int rdDataIndex;
        unsigned int wrDataIndex;

        unsigned int cmdBytes;
        unsigned int resBytes;

        bool byte = false;
        bool interrupt = false;

        bool multiTrackBit;
        bool mfmModeBit;
        bool skipDeletedBit;

        FDCState state;
        FDCMode mode;

        DiskDrive drive[4];

        uint_fast32_t delay = 0;

        uint_fast8_t headUnloadTime;
        uint_fast8_t stepRateTime;
        uint_fast8_t headLoadTime;
        bool useDma;

        FDC() {}

        void clock()
        {
            if (delay)
            {
                --delay;
                statusReg &= SREG_RQM;
                return;
            }

            switch (state)
            {
                case FDCState::FDC_STATE_IDLE:
                    statusReg = SREG_RQM;
                    if (byte)
                    {
                        // byte = false;
                        printf("Command: ");
                        if (checkCommand())
                        {
                            printf("Expecting %x bytes.\n", cmdBytes);
                            state = FDCState::FDC_STATE_COMMAND;
                        }
                        else
                            reset();
                    }
                    break;

                case FDCState::FDC_STATE_COMMAND:
                    statusReg = SREG_RQM | SREG_CB;
                    if (byte)
                    {
                        byte = false;
                        if (cmdIndex == cmdBytes)
                        {
                            decodeParameters();
                            switch (mode)
                            {
                                case FDCMode::FDC_MODE_READ:
                                case FDCMode::FDC_MODE_NONE:
                                    state = FDCState::FDC_STATE_EXECUTION;
                                    break;

                                case FDCMode::FDC_MODE_WRITE:
                                    state = FDCState::FDC_STATE_RECEIVE;
                                    break;

                                default:
                                    reset();
                            }
                        }
                        else if (cmdIndex > cmdBytes)
                            reset();
                    }
                    break;

                case FDCState::FDC_STATE_EXECUTION:
                    statusReg = SREG_EXM | SREG_CB;
                    execute();
                    break;

                case FDCState::FDC_STATE_RECEIVE:
                    statusReg = SREG_RQM | SREG_CB | SREG_EXM;
                    if (byte)
                    {
                        byte = false;
                    }
                    break;

                case FDCState::FDC_STATE_RESULT:
                    statusReg = SREG_RQM | SREG_DIO | SREG_CB;
                    if (byte)
                    {
                        // Maybe put a delay here?
                        byte = false;
                        if (resIndex >= resBytes)
                            reset();
                    }
                    break;
            }
        }

        bool checkCommand()
        {
            switch (cmdBuffer[0] & 0x1F)
            {
                case 0x02:  // Read Track (Diagnostic)
                    printf("Read Track (Diagnostic).\n");
                    cmdBytes = 9;   // 02+MF+SK    HU TR HD SC SZ NM GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD NM SZ
                    mode = FDCMode::FDC_MODE_READ;
                    return true;

                case 0x03:  // Specify SPD/DMA
                    printf("Specify SPD/DMA.\n");
                    cmdBytes = 3;   // 03          XX YY
                    resBytes = 0;   //
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x04:  // Sense drive status
                    printf("Sense drive status.\n");
                    cmdBytes = 2;   // 04          HU
                    resBytes = 1;   //             S3
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x05:  // Write sector(s)
                    printf("Write sector(s).\n");
                    cmdBytes = 9;   // 05+MT+MF    HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x06:  // Read sector(s)
                    printf("Read sector(s).\n");
                    cmdBytes = 9;   // 06+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_READ;
                    return true;

                case 0x07:  // Recalibrate and seek physical track 0
                    printf("Recalibrate and seek physical track 0.\n");
                    cmdBytes = 2;   // 07          HU
                    resBytes = 0;
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x08:  // Sense interrupt status
                    printf("Sense interrupt status.\n");
                    cmdBytes = 1;   // 08
                    resBytes = 2;   //             S0 TP
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x09:  // Write deleted sector(s)
                    printf("Write deleted sector(s).\n");
                    cmdBytes = 9;   // 09+MT+MF    HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x0A:  // Read ID
                    printf("Read ID.\n");
                    cmdBytes = 2;   // 0A+MF       HU
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x0C:  // Read deleted sector(s)
                    printf("Read deleted sector(s).\n");
                    cmdBytes = 9;   // 0C+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_READ;
                    return true;

                case 0x0D:  // Format track
                    printf("Format track.\n");
                    cmdBytes = 6;   // 0D+MF       HU SZ NM GP FB
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x0F:  // Seek track N
                    printf("Seek track.\n");
                    cmdBytes = 3;   // 0F          HU TP
                    resBytes = 0;
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x11:  // Scan equal
                    printf("Scan equal.\n");
                    cmdBytes = 9;   // 11+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x19:  // Scan low or equal
                    printf("Scan low or equal.\n");
                    cmdBytes = 9;   // 19+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x1D:  // Scan high or equal
                    printf("Scan high or equal.\n");
                    cmdBytes = 9;   // 1D+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                default:
                    printf("Wrong command.\n");
                    cmdBytes = 0;
                    resBytes = 0;
                    mode = FDCMode::FDC_MODE_NONE;
                    return false;
            }
        }

        void decodeParameters()
        {
            // Get special bits from command byte
            multiTrackBit = ((cmdBuffer[0] & 0x80) == 0x80);
            mfmModeBit = ((cmdBuffer[0] & 0x40) == 0x40);
            skipDeletedBit = ((cmdBuffer[0] & 0x20) == 0x20);

            switch (cmdBuffer[0] & 0x1F)
            {
                case 0x02:  // Read Track (Diagnostic)
                    break;

                case 0x03:  // Specify SPD/DMA
                    break;

                case 0x04:  // Sense drive status
                    printf("Sense drive: %d\n"
                            "Sense head: %d\n",
                            (cmdBuffer[1] & 0x03), (cmdBuffer[1] & 0x04) >> 2);
                    break;

                case 0x05:  // Write sector(s)
                    break;

                case 0x06:  // Read sector(s)
                    break;

                case 0x07:  // Recalibrate and seek physical track 0
                    printf("Recalibrate drive: %d\n", (cmdBuffer[1] & 0x03));
                    break;

                case 0x08:  // Sense interrupt status
                    break;

                case 0x09:  // Write deleted sector(s)
                    break;

                case 0x0A:  // Read ID
                    printf("Read ID from drive: %d\n"
                            "Read ID from head: %d\n",
                            (cmdBuffer[1] & 0x03), (cmdBuffer[1] & 0x04) >> 2);
                    break;

                case 0x0C:  // Read deleted sector(s)
                    break;

                case 0x0D:  // Format track
                    break;

                case 0x0F:  // Seek track N
                    break;

                case 0x11:  // Scan equal
                    break;

                case 0x19:  // Scan low or equal
                    break;

                case 0x1D:  // Scan high or equal
                    break;

                default:
                    break;
            }
        }

        void execute()
        {
            switch (cmdBuffer[0] & 0x1F)
            {
                case 0x02:  // Read Track (Diagnostic)
                    break;

                case 0x03:  // Specify SPD/DMA
                    headUnloadTime = cmdBuffer[1] & 0x0F;
                    stepRateTime = (cmdBuffer[1] & 0xF0) >> 4;
                    headLoadTime = cmdBuffer[2] >> 1;
                    useDma = ((cmdBuffer[2] & 0x01) == 0x00);
                    printf("Head Unload Time: %d\n"
                            "Head Load Time: %d\n"
                            "Step Rate Time: %d\n"
                            "Use DMA: %s\n",
                            headUnloadTime, headLoadTime, stepRateTime,
                            useDma ? "TRUE" : "FALSE");
                    reset();
                    break;

                case 0x04:  // Sense drive status
                    sReg[3] = cmdBuffer[1] & 0x07;
                    sReg[3] |= drive[(cmdBuffer[1] & 0x03)].senseStatus();
                    resBuffer[0] = sReg[3];
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                case 0x05:  // Write sector(s)
                    break;

                case 0x06:  // Read sector(s)
                    break;

                case 0x07:  // Recalibrate and seek physical track 0
                    sReg[0] = cmdBuffer[1] & 0x07;
                    drive[(cmdBuffer[1] & 0x03)].recalibrate();
                    presCylinder = 0;
                    if (drive[(cmdBuffer[1] & 0x03)].track0)
                        sReg[0] |= 0x20;    // 00100HUU - NT, SE.
                    else
                        sReg[0] |= 0x70;    // 01110HUU - AT, SE, EC.
                    interrupt = true;
                    reset();
                    break;

                case 0x08:  // Sense interrupt status
                    if (!interrupt)
                        sReg[0] = 0x80;

                    resBuffer[0] = sReg[0];
                    resBuffer[1] = presCylinder;
                    interrupt = false;
                    printf("ST0: %0x\nPCN: %0x\n", resBuffer[0], resBuffer[1]);
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                case 0x09:  // Write deleted sector(s)
                    break;

                case 0x0A:  // Read ID
                    break;

                case 0x0C:  // Read deleted sector(s)
                    break;

                case 0x0D:  // Format track
                    break;

                case 0x0F:  // Seek track N
                    break;

                case 0x11:  // Scan equal
                    break;

                case 0x19:  // Scan low or equal
                    break;

                case 0x1D:  // Scan high or equal
                    break;

                default:
                    break;
            }
        }

        void reset()
        {
            printf("FDC reset or idle.\n");
            state = FDCState::FDC_STATE_IDLE;
            byte = false;
            cmdIndex = resIndex = 0;
            cmdBytes = resBytes = 0;
            rdDataIndex = 0;
            wrDataIndex = 0;
            statusReg = SREG_RQM;
        }

        uint_fast8_t status()
        {

            // Uncomment for debugging
            static uint_fast8_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                printf("FDC status: %02x\n", statusReg);
            }

            return statusReg;
        }

        uint_fast8_t read()
        {
            static uint_fast8_t retval = 0x00;
            static uint_fast8_t wait = 0;

            if (++wait == 5 && byte == false)
            {
                wait = 0;
                retval = resBuffer[resIndex++];
                byte = true;
                printf("Cycle %d: Read byte: %02x\n", wait, retval);
            }

            return retval;
        }

        void write(uint_fast8_t value)
        {
            static uint_fast8_t wait = 0;

            if (++wait == 5 && byte == false)
            {
                wait = 0;
                cmdBuffer[cmdIndex++] = value;
                byte = true;
                printf("Cycle %d: Write byte: %02x\n", wait, value);
            }
        }
};


// vim: et:sw=4:ts=4
