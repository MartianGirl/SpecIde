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

#include "DiskDrive.h"

#include <algorithm>
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


constexpr size_t DELAY_1ms = 1000;     // Clocking at 1.000MHz
constexpr size_t SERVICE_MFM = 52;
constexpr size_t SERVICE_FM = 108;

class FDC
{
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

        void clock()
        {
            if (loaded && unload)
            {
                --unloadTimer;
                if (unloadTimer == 0)
                {
                    unloadTimer = headUnloadTime;
                    loaded = false;
                    unload = false;
                    cout << "Head unloaded..." << endl;
                }
            }

            switch (state)
            {
                case FDCState::FDC_STATE_IDLE:
                    statusReg = SREG_RQM;
                    if (byte)
                    {
                        cout << "Command: ";
                        if (checkCommand())
                        {
                            // cout << "Expecting " << hex << cmdBytes << " bytes." << endl;
                            state = FDCState::FDC_STATE_COMMAND;
                        }
                        else
                        {
                            sReg[0] = 0x80;
                            resBuffer[0] = sReg[0];
                            state = FDCState::FDC_STATE_RESULT;
                        }
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
                        byte = false;
                    break;

                case FDCState::FDC_STATE_TRANSMIT:
                    statusReg = SREG_RQM | SREG_DIO | SREG_CB | SREG_EXM;

                    if (transfer) serviceTimer--;

                    if (byte)
                    {
                        byte = false;
                        transfer = true;
                        serviceTimer = (mfmModeBit) ? SERVICE_MFM : SERVICE_FM;

                        if (dataIndex >= dataBytes)
                        {
                            transfer = false;
                            state = FDCState::FDC_STATE_RESULT;
                        }
                    }

                    // Signal Over Run if FDC is not serviced quickly enough.
                    if (serviceTimer == 0)
                    {
                        transfer = false;
                        sReg[1] |= 0x10;    // OR
                        sReg[0] |= 0x40;    // AT
                        interrupt = true;
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

                case 0x10:  // Version
                    cout << "Version." << endl;
                    cmdBytes = 1;
                    resBytes = 1;
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
                    cmdBytes = 1;   // XX
                    resBytes = 1;   //             S0 (80)
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
            serviceTimer = (mfmModeBit) ? SERVICE_MFM : SERVICE_FM;

            switch (cmdBuffer[0] & 0x1F)
            {
                case 0x02:  // Read Track (Diagnostic)
                    idmFound = false;
                    ddmFound = false;
                    eocFound = false;
                    secFound = false;
                    firstSector = cmdBuffer[4];
                    lastSector = cmdBuffer[6];
                    currSector = 0;
                    drive[cmdDrive()].hole = 0;
                    stage = FDCAccess::FDC_ACCESS_LOAD;
                    break;

                case 0x03:  // Specify SPD/DMA
                    break;

                case 0x04:  // Sense drive status
                    break;

                case 0x05:  // Write sector(s)
                    drive[cmdDrive()].hole = 0;
                    break;

                case 0x06:  // Read sector(s)
                    eocFound = false;
                    drive[cmdDrive()].hole = 0;
                    firstSector = cmdBuffer[4];
                    lastSector = cmdBuffer[6];
                    useDeletedDAM = false;
                    stage = FDCAccess::FDC_ACCESS_LOAD;
                    break;

                case 0x07:  // Recalibrate and seek physical track 0
                    break;

                case 0x08:  // Sense interrupt status
                    break;

                case 0x09:  // Write deleted sector(s)
                    break;

                case 0x0A:  // Read ID
                    drive[cmdDrive()].hole = 0;
                    eocFound = false;
                    ddmFound = false;
                    idmFound = false;
                    stage = FDCAccess::FDC_ACCESS_LOAD;
                    break;

                case 0x0C:  // Read deleted sector(s)
                    eocFound = false;
                    drive[cmdDrive()].hole = 0;
                    firstSector = cmdBuffer[4];
                    lastSector = cmdBuffer[6];
                    useDeletedDAM = true;
                    stage = FDCAccess::FDC_ACCESS_LOAD;
                    break;

                case 0x0D:  // Format track
                    break;

                case 0x0F:  // Seek track N
                    break;

                case 0x10:  // Version
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
                    headUnloadTime = (cmdBuffer[1] & 0x0F) * 32 * DELAY_1ms;
                    stepRateTime = ((cmdBuffer[1] & 0xF0) >> 4) * 32 * DELAY_1ms;
                    headLoadTime = (cmdBuffer[2] >> 1) * 4 * DELAY_1ms;
                    useDma = ((cmdBuffer[2] & 0x01) == 0x00);
                    loadTimer = headLoadTime;
                    unloadTimer = headUnloadTime;
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
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                case 0x09:  // Write deleted sector(s)
                    break;

                case 0x0A:  // Read ID
                    readIdCmd();
                    break;

                case 0x0C:  // Read deleted sector(s)
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
                    break;

                case 0x10:  // Version
                    resBuffer[0] = 0x80;
                    state = FDCState::FDC_STATE_RESULT;
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
            drive[cmdDrive()].readSector(cmdHead());

            // Check No Data (ND) condition.
            if (((drive[cmdDrive()].statusReg1) & 0x04) == 0x04)
            {
                // Maybe check the whole track?
                drive[cmdDrive()].nextSector();
                return false;
            }

            currSector = drive[cmdDrive()].idSector;

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
                return true;
        }

        bool readOp()
        {
            bool done = false;

            // We already have read the sector in the seekOp, and
            // currSector holds the current sector number.
            sReg[0] = cmdBuffer[1] & 0x07;
            sReg[1] = drive[cmdDrive()].statusReg1;
            sReg[2] = drive[cmdDrive()].statusReg2;

            // Now we look at the data mark.
            if ((sReg[2] & 0x40) == (useDeletedDAM ? 0x00 : 0x40))
                done = readDeletedDataOp();
            else
                done = readRegularDataOp();

            if (done)
            {
                drive[cmdDrive()].nextSector();

                if (sReg[0] == 0x00 && sReg[1] == 0x00 && currSector == lastSector)
                    eocFound = true;

                setResultBytesOp();
            }

            return done;
        }

        void setResultBytesOp()
        {
            resTrack = cmdBuffer[2];
            resHead = cmdBuffer[3];
            resSector = cmdBuffer[4];
            resSize = cmdBuffer[5];

            if (eocFound)
            {
                sReg[0] |= 0x40;
                sReg[1] |= 0x80;
            }

            if (!multiTrackBit)
            {
                if (resHead != 0x00 || currSector == lastSector)
                {
                    resSector = 1;
                    ++resTrack;
                }
                else
                {
                    ++resSector;
                }
            }
            else
            {
                if (currSector == lastSector)
                {
                    resHead ^= 0x01;
                    if (cmdBuffer[3] != 0x00)
                        ++resTrack;
                    resSector = 1;
                }
                else
                {
                    ++resSector;
                }
            }
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

            if (actlen == 0)
                actlen = outlen;

            vector<uint8_t> buf(&drive[cmdDrive()].buffer[0],
                    &drive[cmdDrive()].buffer[actlen]);

            for (size_t ii = actlen; ii < outlen; ++ii)
                buf.push_back(rand() & 0xFF);

            if (((sReg[1] & 0x20) == 0x20) || ((sReg[2] & 0x20) == 0x20))
            {
                randomizeSector(buf);
                sReg[0] |= 0x40;    // 01000HUU - AT
                error = true;
            }

            // If we reach here, it is a normal data read. Dump and continue.
            copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
            dataBytes += outlen;

            // We must signal the CM.
            sReg[2] &= 0xBF;
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

            vector<uint8_t> buf(&drive[cmdDrive()].buffer[0],
                    &drive[cmdDrive()].buffer[actlen]);

            for (size_t ii = actlen; ii < outlen; ++ii)
                buf.push_back(rand() & 0xFF);

            if (((sReg[1] & 0x20) == 0x20) || ((sReg[2] & 0x20) == 0x20))
            {
                randomizeSector(buf);
                sReg[0] |= 0x40;    // 01000HUU - AT
            }

            copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
            dataBytes += outlen;

            // We must signal the CM.
            sReg[2] |= 0x40;
            return true;
        }

        void readCmd()
        {
            switch (stage)
            {
                case FDCAccess::FDC_ACCESS_LOAD:
                    if (headLoadOp())
                        stage = FDCAccess::FDC_ACCESS_SEEK;
                    break;

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
                        dataBytes = 0;
                        sReg[1] |= 0x04;    // xxxxx1xx - ND
                        sReg[0] |= 0x40;    // 01000HUU - AT
                        stage = FDCAccess::FDC_ACCESS_UNLOAD;
                    }
                    break;

                case FDCAccess::FDC_ACCESS_DATA:
                    if (readOp())
                        stage = FDCAccess::FDC_ACCESS_UNLOAD;
                    else
                    {
                        // Sectors can be unordered.
                        drive[cmdDrive()].hole = 0;
                        ++firstSector;
                        stage = FDCAccess::FDC_ACCESS_SEEK;
                    }
                    break;

                case FDCAccess::FDC_ACCESS_UNLOAD:
                    resBuffer[0] = sReg[0];
                    resBuffer[1] = sReg[1];
                    resBuffer[2] = sReg[2];
                    resBuffer[3] = resTrack;
                    resBuffer[4] = resHead;;
                    resBuffer[5] = resSector;
                    resBuffer[6] = resSize;


                    if (dataBytes)
                        state = FDCState::FDC_STATE_TRANSMIT;
                    else
                        state = FDCState::FDC_STATE_RESULT;
                    interrupt = true;
                    unload = true;
                    break;

                case FDCAccess::FDC_ACCESS_NONE:
                    reset();
                    break;
            }
        }

        void readIdCmd()
        {
            switch (stage)
            {
                case FDCAccess::FDC_ACCESS_LOAD:
                    if (headLoadOp())
                        stage = FDCAccess::FDC_ACCESS_DATA;
                    break;

                case FDCAccess::FDC_ACCESS_DATA:
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

                        drive[cmdDrive()].idTrack = rand() & 0xFF;
                        drive[cmdDrive()].idHead = rand() & 0xFF;
                        drive[cmdDrive()].idSector = rand() & 0xFF;
                        drive[cmdDrive()].idSize = rand() & 0xFF;
                    }
                    else
                    {
                        // Check for Missing Address Mark or No Data.
                        if ((sReg[1] & 0x01) == 0x00)
                            idmFound = true;

                        if ((sReg[1] & 0x04) == 0x00)
                            ddmFound = true;

                        if (idmFound == false || ddmFound == false)
                            return;
                    }

                    stage = FDCAccess::FDC_ACCESS_UNLOAD;
                    break;

                case FDCAccess::FDC_ACCESS_UNLOAD:
                    resBuffer[0] = sReg[0];
                    resBuffer[1] = sReg[1];
                    resBuffer[2] = sReg[2];
                    resBuffer[3] = drive[cmdDrive()].idTrack;
                    resBuffer[4] = drive[cmdDrive()].idHead;
                    resBuffer[5] = drive[cmdDrive()].idSector;
                    resBuffer[6] = drive[cmdDrive()].idSize;
                    interrupt = true;
                    unload = true;
                    state = FDCState::FDC_STATE_RESULT;
                    break;

                default:
                    reset();
                    break;
            }
        }

        bool headLoadOp()
        {
            --loadTimer;
            if (loadTimer == 0)
            {
                loadTimer = headLoadTime;
                loaded = true;
                cout << "Head loaded..." << endl;
            }

            return loaded;
        }

        void readTrackCmd()
        {
            switch (stage)
            {
                case FDCAccess::FDC_ACCESS_LOAD:
                    if (headLoadOp())
                        stage = FDCAccess::FDC_ACCESS_SEEK;
                    break;

                // Seek stage. Here we try to find the disk hole.
                case FDCAccess::FDC_ACCESS_SEEK:
                    if (findHoleOp())
                        stage = FDCAccess::FDC_ACCESS_DATA;
                    // If we don't find the hole, the diskette is weird or
                    // the LED is broken...
                    break;

                case FDCAccess::FDC_ACCESS_DATA:
                    if (readTrackOp())
                        stage = FDCAccess::FDC_ACCESS_UNLOAD;
                    break;

                case FDCAccess::FDC_ACCESS_UNLOAD:
                    if (!idmFound)
                    {
                        sReg[0] |= 0x40;
                        sReg[1] |= 0x01;
                    }

                    if (!ddmFound)
                    {
                        sReg[0] |= 0x40;
                        sReg[2] |= 0x01;
                    }

                    if (!ddmFound && !idmFound)
                        sReg[1] |= 0x80;

                    if (!secFound)
                    {
                        sReg[0] |= 0x40;
                        sReg[1] |= 0x04;
                    }

                    resBuffer[0] = sReg[0];
                    resBuffer[1] = sReg[1];
                    resBuffer[2] = sReg[2];
                    resBuffer[3] = resTrack;
                    resBuffer[4] = resHead;
                    resBuffer[5] = resSector;
                    resBuffer[6] = resSize;

                    if (dataBytes)
                        state = FDCState::FDC_STATE_TRANSMIT;
                    else
                        state = FDCState::FDC_STATE_RESULT;
                    unload = true;
                    interrupt = true;
                    break;

                default:
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

            size_t actlen = drive[cmdDrive()].length;
            size_t outlen;

            // We determine how many bytes to read from this sector.
            if (cmdBuffer[5] == 0x00)   // Use SL as sector length.
                outlen = cmdBuffer[8];
            else                        // Use SZ as sector length.
                outlen = 0x80 << cmdBuffer[5];

            // Double check that the intended data length does
            // not exceed the ACTUAL data length.
            if (actlen != 0 && outlen > actlen)
                outlen = actlen;

            vector<uint8_t> buf(outlen, 0x00);
            buf.assign(&drive[cmdDrive()].buffer[0],
                    &drive[cmdDrive()].buffer[outlen]);

            if (((sReg[1] & 0x20) == 0x20) || ((sReg[2] & 0x20) == 0x20))
                randomizeSector(buf);   // Return random data.

            // If we reach here, it is a normal data read. Dump and continue.
            copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
            dataBytes += outlen;

            setResultBytesOp();

            if ((sReg[1] & 0x01) == 0x00)
                idmFound = true;

            if ((sReg[2] & 0x01) == 0x00)
                ddmFound = true;

            if (idmFound || ddmFound)
                secFound = true;

            drive[cmdDrive()].nextSector();
            return ((currSector == lastSector) || (drive[cmdDrive()].hole > 1));
        }

        void reset()
        {
            state = FDCState::FDC_STATE_IDLE;
            stage = FDCAccess::FDC_ACCESS_NONE;
            byte = false;
            cmdIndex = resIndex = dataIndex = 0;
            cmdBytes = resBytes = dataBytes = 0;
            statusReg = SREG_RQM;
        }

        uint_fast8_t status()
        {
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
                    retval = dataBuffer[dataIndex++];
                else
                    retval = resBuffer[resIndex++];
                byte = true;
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
                    dataBuffer[dataIndex++] = value;
                else
                    cmdBuffer[cmdIndex++] = value;
                byte = true;
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
                stage = FDCAccess::FDC_ACCESS_UNLOAD;
            }
        }

        void motor(bool status)
        {
            if (drive[0].motor != status)
                cout << "Disk drive motor is " << (status ? "ON" : "OFF") << "..." << endl;
            for (size_t ii = 0; ii < 2; ++ii)
                drive[ii].motor = status;
        }

        void randomizeSector(vector<uint8_t>& buf)
        {
            for (size_t ii = 0; ii < 0xB0; ++ii)
                buf[buf.size() - ii - 1] |= rand() & 0x01;

            for (size_t ii = 0; ii < 0x30; ++ii)
                buf[buf.size() - ii - 0xB1] = drive[cmdDrive()].filler;

            for (size_t ii = 0; ii < 0x20; ++ii)
                buf[buf.size() - ii - 0xE1] |= rand() & 0x01;
        }

};

// vim: et:sw=4:ts=4
