/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
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

#include "DSKFile.h"

#include <algorithm>
#include <cassert>
#include <cstdint>


using namespace std;

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
        DiskDrive(bool ready = false) :
            ready(ready) {}

        bool track0 = true;     // Head is at track 0 (center of the disk)
        bool fault = false;     // Error
        bool disk = false;      // Disk is in drive
        bool writeprot = false; // Disk is write protected
        bool ready = false;     // Drive exists

        size_t cylinder = 0;
        size_t sector = 0;
        size_t hole = 0;

        uint_fast8_t idTrack;
        uint_fast8_t idHead;
        uint_fast8_t idSector;
        uint_fast8_t idSize;
        uint_fast8_t statusReg1;
        uint_fast8_t statusReg2;
        uint_fast16_t length;
        vector<uint8_t> buffer;

        DSKFile image;

        /**
         * Advance disk to next sector.
         */
        void nextSector() { ++sector; }

        /**
         * Seek next track.
         */
        void nextTrack()
        {
            size_t limit = disk ? image.numTracks : 99;
            if (++cylinder > limit)
                cylinder = limit;
            track0 = (cylinder == 0);
        }

        /**
         * Seek previous track.
         */
        void prevTrack()
        {
            if (cylinder > 0)
                --cylinder;
            track0 = (cylinder == 0);
        }

        /**
         * Return disk drive status byte.
         */
        uint_fast8_t senseStatus()
        {
            return (track0 ? 0x10 : 0x00)
                | (ready ? 0x20 : 0x00)
                | (writeprot ? 0x40 : 0x00)
                | (fault ? 0x80 : 0x00);        // Don't really know...
        }

        /**
         * Write sector data to disk.
         *
         * This function writes data from the disk drive buffer to
         * the disk. It also checks if the disk hole has been detected.
         *
         */
        void writeSector(int head)
        {
            if (disk && head < image.numSides)
            {
                size_t tr = (image.numSides * cylinder) + head;
                size_t sc = sector % image.tracks[tr].numSectors;
                if (sc == 0)
                    countHole();

                image.tracks[tr].sectors[sc].data = buffer;
            }
        }

        /**
         * Read sector data into buffer.
         *
         * This function reads the data from the current sector, and
         * copies it to the disk drive buffer. It also checks if the
         * disk hole has been detected.
         */
        void readSector(int head)
        {
            if (disk && head < image.numSides)
            {
                size_t tr = (image.numSides * cylinder) + head;
                size_t sc = sector % image.tracks[tr].numSectors;
                if (sc == 0)
                    countHole();

                idTrack = image.tracks[tr].sectors[sc].track;
                idHead = image.tracks[tr].sectors[sc].side;
                idSector = image.tracks[tr].sectors[sc].sectorId;
                idSize = image.tracks[tr].sectors[sc].sectorSize;
                statusReg1 = image.tracks[tr].sectors[sc].fdcStatusReg1;
                statusReg2 = image.tracks[tr].sectors[sc].fdcStatusReg2;
                buffer = image.tracks[tr].sectors[sc].data;
                length = image.tracks[tr].sectors[sc].sectorLength;
            }
            // Should plan for no disk or wrong head.
        }

        void formatTrack(int head,
                uint_fast8_t trackNumber, uint_fast8_t sideNumber,
                uint_fast8_t sectorSize, uint_fast8_t numSectors,
                uint_fast8_t gapLength, uint_fast8_t fillerByte)
        {
            size_t tr = (image.numSides * cylinder) + head;

            if (tr >= image.tracks.size())
                image.tracks.insert(image.tracks.end(),
                        tr - image.tracks.size() + 1, DSKFile::Track());
            
            image.tracks[tr].trackNumber = trackNumber;
            image.tracks[tr].sideNumber = sideNumber;
            image.tracks[tr].sectorSize = sectorSize;
            image.tracks[tr].numSectors = numSectors;
            image.tracks[tr].gapLength = gapLength;
            image.tracks[tr].fillerByte = fillerByte;
            image.tracks[tr].sectors.assign(numSectors, DSKFile::Track::Sector());
        }

        void formatSector(int head,
                uint_fast8_t idTr, uint_fast8_t idHd,
                uint_fast8_t idSc, uint_fast8_t idSz,
                uint_fast8_t byte)
        {
            if (disk && head < image.numSides)
            {
                size_t tr = (image.numSides * cylinder) + head;
                size_t sc = sector % image.tracks[tr].numSectors;
                if (sc == 0)
                    countHole();

                image.tracks[tr].sectors[sc].track = idTr;
                image.tracks[tr].sectors[sc].side = idHd;
                image.tracks[tr].sectors[sc].sectorId = idSc;
                image.tracks[tr].sectors[sc].sectorSize = idSz;
                image.tracks[tr].sectors[sc].sectorLength = idSz;
                image.tracks[tr].sectors[sc].data.assign(idSz, byte);
            }
        }

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

        /**
         * Increase the counter of hole sightings.
         *
         * Disks have a small hole near the center of the magnetic surface
         * that is used by disk drives for counting entire turns. Operations
         * that must be performed along an entire track finish when the hole
         * has been detected twice, which ensures that a track has been
         * under the writing head from beginning to end.
         *
         */
        void countHole()
        {
            sector = 0;
            ++hole;
        }

};

class FDC
{
    public:
        uint_fast8_t statusReg = 0x00;

        uint_fast8_t cmdBuffer[16];
        uint_fast8_t resBuffer[16];
        uint_fast8_t dataBuffer[16384];

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

        FDCState state;
        FDCMode mode;

        DiskDrive drive[4];
        uint_fast8_t presCylNum[4];
        uint_fast8_t sReg[4];

        size_t lastDrive;

        uint_fast32_t delay = 0;

        size_t headUnloadTime;
        size_t stepRateTime;
        size_t headLoadTime;
        bool useDma;

        FDC() :
            drive{DiskDrive(true), DiskDrive(false), DiskDrive(false), DiskDrive(false)},
            presCylNum{0, 0, 0, 0} {}

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
                        cout << "Command: ";
                        if (checkCommand())
                        {
                            cout << "Expecting " << hex << cmdBytes << " bytes." << endl;
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
                            setup();
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

                case FDCState::FDC_STATE_TRANSMIT:
                    statusReg = SREG_RQM | SREG_DIO | SREG_CB | SREG_EXM;
                    if (byte)
                    {
                        byte = false;
                        if (dataIndex >= dataBytes)
                            state = FDCState::FDC_STATE_RESULT;
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
                    cout << "Read Track (Diagnostic)." << endl;
                    cmdBytes = 9;   // 02+MF+SK    HU TR HD SC SZ NM GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD NM SZ
                    mode = FDCMode::FDC_MODE_READ;
                    return true;

                case 0x03:  // Specify SPD/DMA
                    cout << "Specify SPD/DMA." << endl;
                    cmdBytes = 3;   // 03          XX YY
                    resBytes = 0;   //
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x04:  // Sense drive status
                    cout << "Sense drive status." << endl;
                    cmdBytes = 2;   // 04          HU
                    resBytes = 1;   //             S3
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x05:  // Write sector(s)
                    cout << "Write sector(s)." << endl;
                    cmdBytes = 9;   // 05+MT+MF    HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x06:  // Read sector(s)
                    cout << "Read sector(s)." << endl;
                    cmdBytes = 9;   // 06+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_READ;
                    return true;

                case 0x07:  // Recalibrate and seek physical track 0
                    cout << "Recalibrate and seek physical track 0." << endl;
                    cmdBytes = 2;   // 07          HU
                    resBytes = 0;
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x08:  // Sense interrupt status
                    cout << "Sense interrupt status." << endl;
                    cmdBytes = 1;   // 08
                    resBytes = 2;   //             S0 TP
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x09:  // Write deleted sector(s)
                    cout << "Write deleted sector(s)." << endl;
                    cmdBytes = 9;   // 09+MT+MF    HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x0A:  // Read ID
                    cout << "Read ID." << endl;
                    cmdBytes = 2;   // 0A+MF       HU
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x0C:  // Read deleted sector(s)
                    cout << "Read deleted sector(s)." << endl;
                    cmdBytes = 9;   // 0C+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_READ;
                    return true;

                case 0x0D:  // Format track
                    cout << "Format track." << endl;
                    cmdBytes = 6;   // 0D+MF       HU SZ NM GP FB
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x0F:  // Seek track N
                    cout << "Seek track." << endl;
                    cmdBytes = 3;   // 0F          HU TP
                    resBytes = 0;
                    mode = FDCMode::FDC_MODE_NONE;
                    return true;

                case 0x11:  // Scan equal
                    cout << "Scan equal." << endl;
                    cmdBytes = 9;   // 11+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x19:  // Scan low or equal
                    cout << "Scan low or equal." << endl;
                    cmdBytes = 9;   // 19+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                case 0x1D:  // Scan high or equal
                    cout << "Scan high or equal." << endl;
                    cmdBytes = 9;   // 1D+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    mode = FDCMode::FDC_MODE_WRITE;
                    return true;

                default:
                    cout << "Invalid command." << endl;
                    cmdBytes = 0;
                    resBytes = 0;
                    mode = FDCMode::FDC_MODE_NONE;
                    return false;
            }
        }

        void setup()
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
                    cout << "Sense drive: ";
                    cout << hex << setw(2) << setfill('0') << cmdDrive() << endl;
                    cout << "Sense head: ";
                    cout << hex << setw(2) << setfill('0') << cmdHead() << endl;
                    break;

                case 0x05:  // Write sector(s)
                    drive[cmdDrive()].hole = 0;
                    break;

                case 0x06:  // Read sector(s)
                    cout << "Read sector: ";
                    for (size_t ii = 0; ii < 9; ++ii)
                        cout << hex << setw(2) << setfill('0') << static_cast<size_t>(cmdBuffer[ii]) << " ";
                    cout << endl;
                    drive[cmdDrive()].hole = 0;
                    break;

                case 0x07:  // Recalibrate and seek physical track 0
                    cout << "Recalibrate drive: " << cmdDrive() << endl;
                    break;

                case 0x08:  // Sense interrupt status
                    break;

                case 0x09:  // Write deleted sector(s)
                    break;

                case 0x0A:  // Read ID
                    cout << "Read ID from drive " << cmdDrive() << " - ";
                    cout << "head " << cmdHead() << endl;
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
                    cout << "Head Unload Time: ";
                    cout << hex << setw(2) << setfill('0') << headUnloadTime << endl;
                    cout << "Head Load Time: ";
                    cout << hex << setw(2) << setfill('0') << headLoadTime << endl;
                    cout << "Step Rate Time: ";
                    cout << hex << setw(2) << setfill('0') << stepRateTime << endl;
                    cout << "Use DMA: " << (useDma ? "TRUE" : "FALSE") << endl;
                    reset();
                    break;

                case 0x04:  // Sense drive status
                    sReg[3] = cmdBuffer[1] & 0x07;
                    sReg[3] |= drive[cmdDrive()].senseStatus();
                    resBuffer[0] = sReg[3];
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                case 0x05:  // Write sector(s)
                    break;

                case 0x06:  // Read sector(s)
                    // A set of nine (9) byte words are required to place the
                    // FDC into the Read Data Mode. After the Read Data command
                    // has been issued the FDC loads the head (if it is in the
                    // unloaded state), waits the specified head settling time
                    // (defined in the Specify Command), and begins reading
                    // Address Marks and ID fields. When the current 
                    drive[cmdDrive()].readSector(cmdHead());
                    drive[cmdDrive()].nextSector();
                    sReg[0] = cmdBuffer[1] & 0x07;
                    sReg[1] = drive[cmdDrive()].statusReg1;
                    sReg[2] = drive[cmdDrive()].statusReg2;

                    cout << "Reading... ";
                    cout << "From: " << hex << setw(2) << setfill('0') << static_cast<size_t>(cmdBuffer[4]);
                    cout << " - To: " << hex << setw(2) << setfill('0') << static_cast<size_t>(cmdBuffer[6]);
                    cout << " -- Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(drive[cmdDrive()].idSector) << endl;

                    if (drive[cmdDrive()].idSector == cmdBuffer[4])
                    {
                        // Reset counter of "turns without finding sector."
                        drive[cmdDrive()].hole = 0;

                        // Determine how many bytes to read from this sector.
                        size_t outlen;
                        if (cmdBuffer[5] == 0x00)   // Use SL as sector length.
                            outlen = cmdBuffer[8];
                        else                        // Use SZ as sector length.
                            outlen = 0x80 << cmdBuffer[5];

                        // Double check that the intended data length does
                        // not exceed the ACTUAL data length.
                        if (outlen > drive[cmdDrive()].length)
                        {
                            outlen = drive[cmdDrive()].length;
                            cout << "Warning: Sector only has " << outlen << " bytes." << endl;
                        }

                        if ((sReg[1] & 0x20) == 0x20)   // CRC error in ID.
                            sReg[0] |= 0x40;    // 01000HUU - AT
                        else if ((sReg[2] & 0x20) == 0x20)   // CRC error in Data.
                            sReg[0] |= 0x40;    // 01000HUU - AT
                        else if ((sReg[2] & 0x40) == 0x40)   // Deleted Address Mark.
                        {
                            if ((cmdBuffer[0] & 0x40) == 0x40)  // Skip
                                break;
                            else    // Return entire sector, and terminate.
                            {
                                uint_fast16_t slen = drive[cmdDrive()].length;
                                copy(&drive[cmdDrive()].buffer[0],
                                        &drive[cmdDrive()].buffer[slen],
                                        &dataBuffer[dataBytes]);
                                dataBytes += slen;
                            }
                        }
                        else    // Normal read. Dump and continue.
                        {
                            copy(&drive[cmdDrive()].buffer[0],
                                    &drive[cmdDrive()].buffer[outlen],
                                    &dataBuffer[dataBytes]);
                            dataBytes += outlen;
                        }
                    }
                    else if (drive[cmdDrive()].hole > 1)
                    {
                        sReg[1] |= 0x04;    // xxxxx1xx - ND
                        sReg[0] |= 0x40;    // 01000HUU - AT
                    }
                       
                    // End conditions.
                    if ((sReg[0] & 0xC0) != 0x00)   // Error happened.
                    {
                        resBuffer[0] = sReg[0];
                        resBuffer[1] = sReg[1];
                        resBuffer[2] = sReg[2];
                        resBuffer[3] = cmdBuffer[2];
                        resBuffer[4] = cmdBuffer[3];
                        resBuffer[5] = cmdBuffer[4];
                        resBuffer[6] = cmdBuffer[5];
                        interrupt = true;
                        state = FDCState::FDC_STATE_RESULT;
                    }
                    else if (((sReg[2] & 0x40) == 0x40) // DDAM & SK = 0.
                            && ((cmdBuffer[0] & 0x40) == 0x00))
                    {
                        resBuffer[0] = sReg[0];
                        resBuffer[1] = sReg[1];
                        resBuffer[2] = sReg[2];
                        resBuffer[3] = cmdBuffer[2];
                        resBuffer[4] = cmdBuffer[3];
                        resBuffer[5] = cmdBuffer[4] + 1;
                        resBuffer[6] = cmdBuffer[5];
                        interrupt = true;
                        cout << "Transfer " << dec << dataBytes << " bytes." << endl;
                        state = FDCState::FDC_STATE_TRANSMIT;
                    }
                    else if (drive[cmdDrive()].idSector == cmdBuffer[6]) // EOT
                    {
                        resBuffer[0] = sReg[0];
                        resBuffer[1] = sReg[1];
                        resBuffer[2] = sReg[2];
                        resBuffer[3] = cmdBuffer[2] + 1;
                        resBuffer[4] = cmdBuffer[3];
                        resBuffer[5] = 0x01;
                        resBuffer[6] = cmdBuffer[5];
                        interrupt = true;
                        cout << "Transfer " << dec << dataBytes << " bytes." << endl;
                        state = FDCState::FDC_STATE_TRANSMIT;
                    }
                    break;

                case 0x07:  // Recalibrate and seek physical track 0
                    // TODO: Concurrent recalibrate/seek operations are
                    // possible for each drive.
                    lastDrive = cmdDrive();
                    sReg[0] = cmdBuffer[1] & 0x07;
                    drive[cmdDrive()].recalibrate();
                    presCylNum[cmdDrive()] = 0;
                    if (drive[cmdDrive()].track0)
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
                    resBuffer[1] = presCylNum[lastDrive];
                    interrupt = false;
                    statusReg &= ~(SREG_DB0 | SREG_DB1 | SREG_DB2 | SREG_DB3);
                    cout << "ST0: ";
                    cout << hex << setw(2) << setfill('0') << static_cast<size_t>(resBuffer[0]) << endl;
                    cout << "PCN: ";
                    cout << hex << setw(2) << setfill('0') << static_cast<size_t>(resBuffer[1]) << endl;
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                case 0x09:  // Write deleted sector(s)
                    break;

                case 0x0A:  // Read ID
                    drive[cmdDrive()].readSector(cmdHead());
                    dataBuffer[0] = drive[cmdDrive()].idSector;
                    dataBytes = 1;
                    resBuffer[0] = cmdBuffer[1] & 0x07;
                    sReg[1] = resBuffer[1] = drive[cmdDrive()].statusReg1;
                    sReg[2] = resBuffer[2] = drive[cmdDrive()].statusReg2;
                    resBuffer[3] = drive[cmdDrive()].cylinder;
                    resBuffer[4] = cmdHead();
                    resBuffer[5] = drive[cmdDrive()].sector;
                    resBuffer[6] = drive[cmdDrive()].idSize;
                    drive[cmdDrive()].nextSector();
                    interrupt = true;
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                case 0x0C:  // Read deleted sector(s)
                    break;

                case 0x0D:  // Format track
                    break;

                case 0x0F:  // Seek track N
                    // TODO: Concurrent recalibrate/seek operations are
                    // possible for each drive.
                    statusReg |= SREG_DB0 | SREG_DB1 | SREG_DB2 | SREG_DB3;
                    if (drive[cmdDrive()].ready == false)
                    {
                        sReg[0] = cmdBuffer[1] & 0x07;
                        sReg[0] |= 0x48;    // 01001HUU - AT, NR.
                        interrupt = true;
                        reset();
                    }
                    else if (presCylNum[cmdDrive()] == cmdBuffer[2])
                    {
                        sReg[0] = cmdBuffer[1] & 0x07;
                        sReg[0] |= 0x20;    // 00100HUU - NT, SE.
                        interrupt = true;
                        reset();
                    }
                    else if (presCylNum[cmdDrive()] > cmdBuffer[2])
                    {
                        drive[cmdDrive()].prevTrack();
                        --presCylNum[cmdDrive()];
                    }
                    else
                    {
                        drive[cmdDrive()].nextTrack();
                        ++presCylNum[cmdDrive()];
                    }
                    cout << "Current track: " << static_cast<size_t>(presCylNum[cmdDrive()]) << endl;
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
            cout << "FDC reset or idle." << endl;
            state = FDCState::FDC_STATE_IDLE;
            byte = false;
            cmdIndex = resIndex = dataIndex = 0;
            cmdBytes = resBytes = dataBytes = 0;
            statusReg = SREG_RQM;
        }

        uint_fast8_t status()
        {

            static uint_fast8_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                // Uncomment for debugging
                // cout << "FDC status: " << hex << setw(2) << setfill('0');
                // cout << static_cast<size_t>(statusReg) << endl;
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
                if ((statusReg & SREG_EXM) == SREG_EXM)
                {
                    // cout << "Data buffer(" << dataIndex << "): ";
                    retval = dataBuffer[dataIndex++];
                }
                else
                {
                    // cout << "Result buffer: ";
                    retval = resBuffer[resIndex++];
                }
                byte = true;
                // cout << "Read byte: " << hex << setw(2) << setfill('0');
                // cout << static_cast<size_t>(retval) << endl;
            }

            return retval;
        }

        void write(uint_fast8_t value)
        {
            static uint_fast8_t wait = 0;

            if (++wait == 5 && byte == false)
            {
                wait = 0;
                if ((statusReg & SREG_EXM) == SREG_EXM)
                {
                    // cout << "Data buffer: ";
                    dataBuffer[dataIndex++] = value;
                }
                else
                {
                    // cout << "Command buffer: ";
                    cmdBuffer[cmdIndex++] = value;
                }
                byte = true;
                // cout << "Write byte: " << hex << setw(2) << setfill('0');
                // cout << static_cast<size_t>(value) << endl;
            }
        }

        size_t cmdDrive() { return (cmdBuffer[1] & 0x03); }
        size_t cmdHead() { return ((cmdBuffer[1] & 0x04) >> 2); }
};


// vim: et:sw=4:ts=4
