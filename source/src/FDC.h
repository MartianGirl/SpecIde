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

enum class FDCAccess
{
    FDC_ACCESS_SEEK,
    FDC_ACCESS_DATA,
    FDC_ACCESS_END,
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
        size_t next[2];

        uint_fast8_t idTrack;
        uint_fast8_t idHead;
        uint_fast8_t idSector;
        uint_fast8_t idSize;
        uint_fast8_t statusReg1;
        uint_fast8_t statusReg2;
        uint_fast16_t length;
        vector<uint8_t> buffer;

        vector<DSKFile> images;
        vector<string> imagenames;
        size_t currentImage;

        /**
         * Advance disk to next sector.
         */
        void nextSector()
        {
            ++sector;
            if (disk)
            {
                size_t tr = (images[currentImage].numSides * cylinder);

                if (images[currentImage].tracks[tr].trackSize)
                {
                    for (size_t ii = 0; ii < images[currentImage].numSides; ++ii)
                        next[ii] = sector % images[currentImage].tracks[tr + ii].numSectors;
                }
                else
                {
                    next[0] = 0;
                    next[1] = 0;
                }
            }

            if (next[0] == 0)
                countHole();
        }

        /**
         * Seek next track.
         */
        void nextTrack()
        {
            size_t limit = disk ? images[currentImage].numTracks : 76;
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
            if (disk && head < images[currentImage].numSides)
            {
                size_t tr = (images[currentImage].numSides * cylinder) + head;
                size_t sc = sector % images[currentImage].tracks[tr].numSectors;
                if (sc == 0)
                    countHole();

                images[currentImage].tracks[tr].sectors[sc].data = buffer;
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
            if (disk && head < images[currentImage].numSides)
            {
                size_t tr = (images[currentImage].numSides * cylinder) + head;
                size_t sc = sector % images[currentImage].tracks[tr].numSectors;

                // If the track is formatted, read.
                if (images[currentImage].tracks[tr].trackSize)
                {
                    idTrack = images[currentImage].tracks[tr].sectors[sc].track;
                    idHead = images[currentImage].tracks[tr].sectors[sc].side;
                    idSector = images[currentImage].tracks[tr].sectors[sc].sectorId;
                    idSize = images[currentImage].tracks[tr].sectors[sc].sectorSize;
                    statusReg1 = images[currentImage].tracks[tr].sectors[sc].fdcStatusReg1;
                    statusReg2 = images[currentImage].tracks[tr].sectors[sc].fdcStatusReg2;
                    buffer = images[currentImage].tracks[tr].sectors[sc].data;
                    length = images[currentImage].tracks[tr].sectors[sc].sectorLength;
                }
                else
                {
                    idTrack = rand() & 0xFF;
                    idHead = rand() & 0xFF;
                    idSector = rand() & 0xFF;
                    idSize = rand() & 0xFF;
                    statusReg1 = 0x25;  // 00100101: DE, ND, MAM
                    statusReg2 = 0x20;  // 00110011: DD, WC, BC, MD
                    length = 0;
                }
            }
            // Should plan for no disk or wrong head.
        }

        void formatTrack(int head,
                uint_fast8_t trackNumber, uint_fast8_t sideNumber,
                uint_fast8_t sectorSize, uint_fast8_t numSectors,
                uint_fast8_t gapLength, uint_fast8_t fillerByte)
        {
            size_t tr = (images[currentImage].numSides * cylinder) + head;

            if (tr >= images[currentImage].tracks.size())
                images[currentImage].tracks.insert(images[currentImage].tracks.end(),
                        tr - images[currentImage].tracks.size() + 1, DSKFile::Track());
            
            images[currentImage].tracks[tr].trackNumber = trackNumber;
            images[currentImage].tracks[tr].sideNumber = sideNumber;
            images[currentImage].tracks[tr].sectorSize = sectorSize;
            images[currentImage].tracks[tr].numSectors = numSectors;
            images[currentImage].tracks[tr].gapLength = gapLength;
            images[currentImage].tracks[tr].fillerByte = fillerByte;
            images[currentImage].tracks[tr].sectors.assign(numSectors, DSKFile::Track::Sector());
        }

        void formatSector(int head,
                uint_fast8_t idTr, uint_fast8_t idHd,
                uint_fast8_t idSc, uint_fast8_t idSz,
                uint_fast8_t byte)
        {
            if (disk && head < images[currentImage].numSides)
            {
                size_t tr = (images[currentImage].numSides * cylinder) + head;
                size_t sc = sector % images[currentImage].tracks[tr].numSectors;
                if (sc == 0)
                    countHole();

                images[currentImage].tracks[tr].sectors[sc].track = idTr;
                images[currentImage].tracks[tr].sectors[sc].side = idHd;
                images[currentImage].tracks[tr].sectors[sc].sectorId = idSc;
                images[currentImage].tracks[tr].sectors[sc].sectorSize = idSz;
                images[currentImage].tracks[tr].sectors[sc].sectorLength = idSz;
                images[currentImage].tracks[tr].sectors[sc].data.assign(idSz, byte);
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

        void nextDisk()
        {
            if (images.size() > 0)
            {
                ++currentImage;
                if (currentImage == images.size())
                    currentImage = 0;
                cout << "Currently inserted disk: " << imagenames[currentImage] << endl;
            }
        }

        void prevDisk()
        {
            if (images.size() > 0)
            {
                if (currentImage == 0)
                    currentImage = images.size();
                --currentImage;
                cout << "Currently inserted disk: " << imagenames[currentImage] << endl;
            }
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
        bool useDeletedDAM;

        bool idmFound;
        bool ddmFound;

        FDCState state;
        FDCMode mode;
        FDCAccess stage;

        DiskDrive drive[4];
        uint_fast8_t presCylNum[4];
        uint_fast8_t sReg[4];

        size_t lastDrive;
        size_t firstSector = 0x00;
        size_t currSector = 0x00;
        size_t lastSector = 0x00;

        size_t resTrack = 0x00;
        size_t resHead = 0x00;
        size_t resSector = 0x00;
        size_t resSize = 0x00;

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
                    cout << "Read Track (Diagnostic): " << endl;
                    cout << "Drive: " << hex << setw(2) << setfill('0') << cmdDrive() << " ";
                    cout << "Head: " << hex << setw(2) << setfill('0') << cmdHead() << " ";
                    cout << "Track: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[2]) << " ";
                    cout << "Head: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[3]) << " ";
                    cout << "From Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[4]) << " ";
                    cout << "Sector Size: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[5]) << " ";
                    cout << "To Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[6]) << " ";
                    cout << "Gap Length: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[7]) << " ";
                    cout << "Data Length: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[8]) << endl;

                    idmFound = false;
                    ddmFound = false;
                    firstSector = cmdBuffer[4];
                    lastSector = cmdBuffer[6];
                    currSector = 0;
                    drive[cmdDrive()].hole = 0;
                    stage = FDCAccess::FDC_ACCESS_SEEK;
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
                    cout << "Read sector(s): " << endl;
                    cout << "Drive: " << hex << setw(2) << setfill('0') << cmdDrive() << " ";
                    cout << "Head: " << hex << setw(2) << setfill('0') << cmdHead() << " ";
                    cout << "Track: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[2]) << " ";
                    cout << "Head: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[3]) << " ";
                    cout << "From Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[4]) << " ";
                    cout << "Sector Size: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[5]) << " ";
                    cout << "To Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[6]) << " ";
                    cout << "Gap Length: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[7]) << " ";
                    cout << "Data Length: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[8]) << endl;

                    drive[cmdDrive()].hole = 0;
                    firstSector = cmdBuffer[4];
                    lastSector = cmdBuffer[6];
                    useDeletedDAM = false;
                    stage = FDCAccess::FDC_ACCESS_SEEK;
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

                    drive[cmdDrive()].hole = 0;
                    ddmFound = false;
                    idmFound = false;
                    break;

                case 0x0C:  // Read deleted sector(s)
                    cout << "Read deleted sector(s): " << endl;
                    cout << "Drive: " << hex << setw(2) << setfill('0') << cmdDrive() << " ";
                    cout << "Head: " << hex << setw(2) << setfill('0') << cmdHead() << " ";
                    cout << "Track: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[2]) << " ";
                    cout << "Head: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[3]) << " ";
                    cout << "From Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[4]) << " ";
                    cout << "Sector Size: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[5]) << " ";
                    cout << "To Sector: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[6]) << " ";
                    cout << "Gap Length: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[7]) << " ";
                    cout << "Data Length: " << hex << setw(2) << setfill('0');
                    cout << static_cast<size_t>(cmdBuffer[8]) << endl;

                    drive[cmdDrive()].hole = 0;
                    firstSector = cmdBuffer[4];
                    lastSector = cmdBuffer[6];
                    useDeletedDAM = true;
                    stage = FDCAccess::FDC_ACCESS_SEEK;
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
                    readTrackCmd();
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
                    // checkDrive();
                    readCmd();
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
                    readIdCmd();
                    break;

                case 0x0C:  // Read deleted sector(s)
                    // checkDrive();
                    readCmd();
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

        bool seekOp()
        {
            cout << "Seeking sector: " << hex << setw(2) << setfill('0');
            cout << firstSector;
            drive[cmdDrive()].readSector(cmdHead());

            // Check No Data (ND) condition.
            if (((drive[cmdDrive()].statusReg1) & 0x04) == 0x04)
            {
                // Maybe check the whole track?
                // cout << "... Track contains no data." << endl;
                drive[cmdDrive()].nextSector();
                // drive[cmdDrive()].hole = 2;
                return false;
            }

            currSector = drive[cmdDrive()].idSector;
            cout << " Found sector: " << currSector << endl;

            // The four bytes are checked.
            if (currSector != firstSector
                    || cmdBuffer[2] != drive[cmdDrive()].idTrack
                    || cmdBuffer[3] != drive[cmdDrive()].idHead
                    || cmdBuffer[5] != drive[cmdDrive()].idSize)
            {
                drive[cmdDrive()].nextSector();
                return false;
            }
            else
            {
                drive[cmdDrive()].hole = 0;
                return true;
            }
        }

        bool readOp()
        {
            drive[cmdDrive()].readSector(cmdHead());
            drive[cmdDrive()].nextSector();
            sReg[0] = cmdBuffer[1] & 0x07;
            sReg[1] = drive[cmdDrive()].statusReg1;
            sReg[2] = drive[cmdDrive()].statusReg2;
            currSector = drive[cmdDrive()].idSector;

            cout << "Reading sector... ";
            cout << hex << setw(2) << setfill('0') << currSector;
            cout << " (Skip bit " << (skipDeletedBit ? "ON)" : "OFF) ... ");

            // Now we look at the data mark.
            if ((sReg[2] & 0x40) == (useDeletedDAM ? 0x00 : 0x40))
                return readDeletedDataOp();
            else
                return readRegularDataOp();
        }

        bool readRegularDataOp()
        {
            bool error = false;

            size_t actlen = drive[cmdDrive()].length;
            size_t outlen;

            // We determine how many bytes to read from this sector.
            if (cmdBuffer[5] == 0x00)   // Use SL as sector length.
                outlen = cmdBuffer[8];
            else                        // Use SZ as sector length.
                outlen = 0x80 << cmdBuffer[5];
            cout << " (" << dec << outlen << " bytes)" << endl;

            vector<uint8_t> buf(outlen, 0x00);

            if (actlen == 0)
                actlen = outlen;
            buf.assign(&drive[cmdDrive()].buffer[0],
                    &drive[cmdDrive()].buffer[actlen]);

            if ((drive[cmdDrive()].idSize < 6)
                    && (((sReg[1] & 0x20) == 0x20) || ((sReg[2] & 0x20) == 0x20)))
            {
                cout << " - CRC error..." << endl;
                // Return random data.
                buf.back() = rand() & 0xFF;
                sReg[0] |= 0x40;    // 01000HUU - AT
                error = true;
            }

            // If we reach here, it is a normal data read. Dump and continue.
            copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
            dataBytes += outlen;

            // Update result info.
            resTrack = drive[cmdDrive()].idTrack;
            resHead = drive[cmdDrive()].idHead;
            resSector = drive[cmdDrive()].idSector;
            resSize = drive[cmdDrive()].idSize;

            return (error || currSector == lastSector);
        }

        bool readDeletedDataOp()
        {
            // We determine how many bytes to read from this sector.
            size_t outlen = 0x80 << drive[cmdDrive()].idSize;
            size_t actlen = drive[cmdDrive()].length;

            if (skipDeletedBit)
                return false;

            // Not skipping: Dump entire sector and finish.
            if (actlen == 0)
                actlen = outlen;

            vector<uint8_t> buf(outlen, 0x00);
            buf.assign(&drive[cmdDrive()].buffer[0],
                    &drive[cmdDrive()].buffer[actlen]);

            if ((drive[cmdDrive()].idSize < 6)
                    && (((sReg[1] & 0x20) == 0x20) || ((sReg[2] & 0x20) == 0x20)))
            {
                cout << " CRC error..." << endl;
                buf.back() = rand() & 0xFF;
                sReg[0] |= 0x40;    // 01000HUU - AT
            }

            copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
            dataBytes += outlen;

            resTrack = drive[cmdDrive()].idTrack;
            resHead = drive[cmdDrive()].idHead;
            resSector = drive[cmdDrive()].idSector;
            resSize = drive[cmdDrive()].idSize;

            return true;
        }

        void readCmd()
        {
            switch (stage)
            {
                // Seek stage. Here we read all the sectors in the track,
                // until we find one whose ID matches command parameter R.
                // If during the seek we find the hole mark twice, this means
                // that the sector we're looking for does not exist, and we
                // end the operation with an error.
                case FDCAccess::FDC_ACCESS_SEEK:
                    if (seekOp())
                        stage = FDCAccess::FDC_ACCESS_DATA;
                    else if (drive[cmdDrive()].hole > 1)
                    {
                        cout << "Sector not found..." << endl;
                        dataBytes = 0;
                        sReg[1] |= 0x04;    // xxxxx1xx - ND
                        sReg[0] |= 0x40;    // 01000HUU - AT
                        stage = FDCAccess::FDC_ACCESS_END;
                    }
                    break;

                case FDCAccess::FDC_ACCESS_DATA:
                    if (readOp())
                        stage = FDCAccess::FDC_ACCESS_END;
                    else
                    {
                        // Sectors can be unordered.
                        ++firstSector;
                        stage = FDCAccess::FDC_ACCESS_SEEK;
                    }
                    break;

                case FDCAccess::FDC_ACCESS_END:
                    resBuffer[0] = sReg[0];
                    resBuffer[1] = sReg[1];
                    resBuffer[2] = sReg[2];
                    resBuffer[3] = resTrack;
                    resBuffer[4] = resHead;;
                    resBuffer[5] = resSector;
                    resBuffer[6] = resSize;

                    if (dataBytes)
                    {
                        cout << "Returning " << dataBytes << " bytes..." << endl;
                        state = FDCState::FDC_STATE_TRANSMIT;
                    }
                    else
                        state = FDCState::FDC_STATE_RESULT;
                    interrupt = true;
                    break;

                case FDCAccess::FDC_ACCESS_NONE:
                    reset();
                    break;
            }
        }

        void readIdCmd()
        {
            drive[cmdDrive()].readSector(cmdHead());
            drive[cmdDrive()].nextSector();
            sReg[0] = cmdBuffer[1] & 0x07;
            sReg[1] = drive[cmdDrive()].statusReg1;
            sReg[2] = drive[cmdDrive()].statusReg2;

            if (drive[cmdDrive()].hole > 1)
            {
                if (idmFound == false) sReg[1] |= 0x01;     // xxxxxxx1 - MAD
                if (ddmFound == false) sReg[1] |= 0x04;     // xxxxx1xx - ND
                sReg[0] |= 0x40;    // 01000HUU - AT
            }
            else
            {
                // Check for Missing Address Mark or No Data.
                if ((sReg[1] & 0x01) == 0x00)
                    idmFound = true;

                if ((sReg[1] & 0x04) == 0x00)
                    ddmFound = true;

                if (idmFound == false)
                    return;
            }

            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = drive[cmdDrive()].idTrack;
            resBuffer[4] = drive[cmdDrive()].idHead;
            resBuffer[5] = drive[cmdDrive()].idSector;
            resBuffer[6] = drive[cmdDrive()].idSize;
            interrupt = true;
            state = FDCState::FDC_STATE_RESULT;
        }

        void readTrackCmd()
        {
            switch (stage)
            {
                // Seek stage. Here we try to find the disk hole.
                case FDCAccess::FDC_ACCESS_SEEK:
                    if (findHoleOp())
                        stage = FDCAccess::FDC_ACCESS_DATA;
                    // If we don't find the hole, the diskette is weird or
                    // the LED is broken...
                    break;

                case FDCAccess::FDC_ACCESS_DATA:
                    if (readTrackOp())
                        stage = FDCAccess::FDC_ACCESS_END;
                    break;

                case FDCAccess::FDC_ACCESS_END:
                    if (idmFound == false)
                        sReg[1] |= 0x04;
                    if (ddmFound == false)
                    {
                        sReg[0] |= 0x40;
                        sReg[1] |= 0x01;
                    }

                    resBuffer[0] = sReg[0];
                    resBuffer[1] = sReg[1];
                    resBuffer[2] = sReg[2];
                    resBuffer[3] = resTrack;
                    resBuffer[4] = resHead;
                    resBuffer[5] = resSector;
                    resBuffer[6] = resSize;

                    if (dataBytes)
                    {
                        cout << "Returning " << dataBytes << " bytes..." << endl;
                        state = FDCState::FDC_STATE_TRANSMIT;
                    }
                    else
                        state = FDCState::FDC_STATE_RESULT;
                    interrupt = true;
                    break;

                case FDCAccess::FDC_ACCESS_NONE:
                    reset();
                    break;
            }
        }

        bool findHoleOp()
        {
            drive[cmdDrive()].hole = 0;
            drive[cmdDrive()].nextSector();
            return (drive[cmdDrive()].hole > 0);
        }

        bool readTrackOp()
        {
            drive[cmdDrive()].readSector(cmdHead());
            sReg[0] = cmdBuffer[1] & 0x07;
            sReg[1] = drive[cmdDrive()].statusReg1;
            sReg[2] = drive[cmdDrive()].statusReg2;
            ++currSector;

            // cout << "Sectors read: ";
            // cout << hex << setw(2) << setfill('0') << currSector << endl;

            size_t actlen = drive[cmdDrive()].length;
            size_t outlen;

            // We determine how many bytes to read from this sector.
            if (cmdBuffer[5] == 0x00)   // Use SL as sector length.
                outlen = cmdBuffer[8];
            else                        // Use SZ as sector length.
                outlen = 0x80 << cmdBuffer[5];
            // cout << " (" << dec << outlen << " bytes)" << endl;

            // Double check that the intended data length does
            // not exceed the ACTUAL data length.
            if (actlen != 0 && outlen > actlen)
            {
                outlen = actlen;
                cout << "Warning: Sector only has " << outlen << " bytes." << endl;
            }

            vector<uint8_t> buf(outlen, 0x00);
            buf.assign(&drive[cmdDrive()].buffer[0],
                    &drive[cmdDrive()].buffer[outlen]);

            if ((drive[cmdDrive()].idSize < 6)
                    && (((sReg[1] & 0x20) == 0x20) || ((sReg[2] & 0x20) == 0x20)))
            {
                cout << " - CRC error..." << endl;
                // Return random data.
                buf.back() = rand() & 0xFF;
            }

            // If we reach here, it is a normal data read. Dump and continue.
            copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
            dataBytes += outlen;

            // Update result info.
            resTrack = drive[cmdDrive()].idTrack;
            resHead = drive[cmdDrive()].idHead;
            resSector = drive[cmdDrive()].idSector;
            resSize = drive[cmdDrive()].idSize;

            if (resSector == firstSector)
                idmFound = true;

            if ((sReg[1] & 0x01) == 0x00)
                ddmFound = true;

            drive[cmdDrive()].nextSector();
            return ((currSector == lastSector) || (drive[cmdDrive()].hole > 1));
        }

        void reset()
        {
            cout << "FDC reset or idle." << endl;
            state = FDCState::FDC_STATE_IDLE;
            stage = FDCAccess::FDC_ACCESS_NONE;
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

        size_t cmdDrive() { return (cmdBuffer[1] & 0x01); } // 2 drives max.
        size_t cmdHead() { return ((cmdBuffer[1] & 0x04) >> 2); }
        void checkDrive()
        {
            if (cmdDrive() > 1 || cmdHead() > 0)
            {
                sReg[0] = cmdBuffer[0];
                sReg[0] |= 0xC4;    // 01..1HUU: AT, NR
                stage = FDCAccess::FDC_ACCESS_END;
            }
        }
};


// vim: et:sw=4:ts=4
