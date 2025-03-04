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

#include <algorithm>
#include <iomanip>

#include "FDC765.h"
#include "SpecIde.h"

using namespace std;

void FDC765::clock() {

    updateHeadUnload();

    switch (state) {
        case FDC765State::IDLE:
            mode = FDC765Mode::NONE;
            statusReg = SREG_RQM;
            if (inputByteReady) {
                checkCommand();
            }
            break;

        case FDC765State::COMMAND:
            mode = FDC765Mode::NONE;
            statusReg = SREG_RQM | SREG_CB;
            if (inputByteReady) {
                inputByteReady = false;
                if (cmdIndex == cmdBytes) {
                    setup();
                } else if (cmdIndex > cmdBytes) {
                    reset();
#ifdef DEBUGFDC765
                } else {
                    cout << "Command byte " << static_cast<uint32_t>(cmdIndex) << ": "
                        << static_cast<uint32_t>(cmdBuffer[cmdIndex]) << endl;
#endif
                }
            }
            break;

        case FDC765State::EXECUTION:
            mode = FDC765Mode::WRITE;
            statusReg = SREG_EXM | SREG_CB;
            execute();
            break;

        case FDC765State::RECEIVE:
            // Send data from CPU to FDC.
            mode = FDC765Mode::WRITE;
            statusReg = SREG_RQM | SREG_CB | SREG_EXM;

            if (inputByteReady) {
                inputByteReady = false;
                transfer = true;
                serviceTimer = clockFrequency * ((mfmModeBit) ? SERVICE_MFM : SERVICE_FM);
                if (dataIndex >= dataBytes) {
                    transfer = false;
                    state = FDC765State::EXECUTION;
                }
            }

            // Signal Over Run if FDC765 is not serviced quickly enough.
            if (transfer && --serviceTimer == 0) {
                transfer = false;
                sReg[1] |= 0x10;    // OR
                sReg[0] |= 0x40;    // AT
                interrupt = true;
                state = FDC765State::RESULT;
                outputByteReady = true;
            }
            break;

        case FDC765State::TRANSMIT:
            // Send data from FDC to CPU.
            mode = FDC765Mode::READ;
            statusReg = SREG_RQM | SREG_DIO | SREG_CB | SREG_EXM;

            if (!outputByteReady) {
                outputByteReady = true;
                transfer = true;
                serviceTimer = clockFrequency * ((mfmModeBit) ? SERVICE_MFM : SERVICE_FM);
                if (dataIndex >= dataBytes) {
                    transfer = false;
                    state = FDC765State::RESULT;
                }
            }

            // Signal Over Run if FDC765 is not serviced quickly enough.
            if (transfer && --serviceTimer == 0) {
                transfer = false;
                sReg[1] |= 0x10;    // OR
                sReg[0] |= 0x40;    // AT
                interrupt = true;
                state = FDC765State::RESULT;
#ifdef DEBUGFDC765
                cout << "Overrun!" << endl;
#endif
            }
            break;

        case FDC765State::RESULT:
            // Send result bytes from FDC to CPU.
            mode = FDC765Mode::NONE;
            statusReg = SREG_RQM | SREG_DIO | SREG_CB;
            if (!outputByteReady) {
                outputByteReady = true;
                if (resIndex >= resBytes) {
                    reset();
                }
            }
            break;
    }
}

void FDC765::checkCommand() {

    switch (cmdBuffer[0] & 0x1F) {

        case 0x02:  // Read Track (Diagnostic)
#ifdef DEBUGFDC765
            cout << "Read track" << endl;
#endif
            cmdBytes = 9;   // 02+MF+SK    HU TR HD SC SZ NM GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD NM SZ
            state = FDC765State::COMMAND;
            break;

        case 0x03:  // Specify SPD/DMA
#ifdef DEBUGFDC765
            cout << "Specify" << endl;
#endif
            cmdBytes = 3;   // 03          XX YY
            resBytes = 0;   //
            state = FDC765State::COMMAND;
            break;

        case 0x04:  // Sense drive status
#ifdef DEBUGFDC765
            cout << "Sense drive" << endl;
#endif
            cmdBytes = 2;   // 04          HU
            resBytes = 1;   //             S3
            state = FDC765State::COMMAND;
            break;

        case 0x05:  // Write sector(s)
#ifdef DEBUGFDC765
            cout << "Write sector" << endl;
#endif
            cmdBytes = 9;   // 05+MT+MF    HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x06:  // Read sector(s)
#ifdef DEBUGFDC765
            cout << "Read sector" << endl;
#endif
            cmdBytes = 9;   // 06+MT+MF+SK HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x07:  // Recalibrate and seek physical track 0
#ifdef DEBUGFDC765
            cout << "Recalibrate" << endl;
#endif
            cmdBytes = 2;   // 07          HU
            resBytes = 0;
            state = FDC765State::COMMAND;
            break;

        case 0x08:  // Sense interrupt status
#ifdef DEBUGFDC765
            cout << "Sense interrupt" << endl;
#endif
            cmdBytes = 1;   // 08
            resBytes = 2;   //             S0 TP
            state = FDC765State::COMMAND;
            break;

        case 0x09:  // Write deleted sector(s)
#ifdef DEBUGFDC765
            cout << "Write deleted" << endl;
#endif
            cmdBytes = 9;   // 09+MT+MF    HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x0A:  // Read ID
#ifdef DEBUGFDC765
            cout << "Read ID" << endl;
#endif
            cmdBytes = 2;   // 0A+MF       HU
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x0C:  // Read deleted sector(s)
#ifdef DEBUGFDC765
            cout << "Read deleted" << endl;
#endif
            cmdBytes = 9;   // 0C+MT+MF+SK HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x0D:  // Format track
#ifdef DEBUGFDC765
            cout << "Format track" << endl;
#endif
            cmdBytes = 6;   // 0D+MF       HU SZ NM GP FB
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x0F:  // Seek track N
#ifdef DEBUGFDC765
            cout << "Seek track" << endl;
#endif
            cmdBytes = 3;   // 0F          HU TP
            resBytes = 0;
            state = FDC765State::COMMAND;
            break;

        case 0x10:  // Version
#ifdef DEBUGFDC765
            cout << "Version" << endl;
#endif
            cmdBytes = 1;
            resBytes = 1;
            state = FDC765State::COMMAND;
            break;

        case 0x11:  // Scan equal
#ifdef DEBUGFDC765
            cout << "Scan equal" << endl;
#endif
            cmdBytes = 9;   // 11+MT+MF+SK HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x19:  // Scan low or equal
#ifdef DEBUGFDC765
            cout << "Scan low or equal" << endl;
#endif
            cmdBytes = 9;   // 19+MT+MF+SK HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        case 0x1D:  // Scan high or equal
#ifdef DEBUGFDC765
            cout << "Scan high or equal" << endl;
#endif
            cmdBytes = 9;   // 1D+MT+MF+SK HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            state = FDC765State::COMMAND;
            break;

        default:
            cmdBytes = 1;   // XX
            resBytes = 1;   //             S0 (80)
            sReg[0] = 0x80;
            resBuffer[0] = sReg[0];
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;
    }
}

void FDC765::setup() {

    // Get special bits from command byte
    multiTrackBit = ((cmdBuffer[0] & 0x80) == 0x80);
    mfmModeBit = ((cmdBuffer[0] & 0x40) == 0x40);
    skipDeletedBit = ((cmdBuffer[0] & 0x20) == 0x20);
    serviceTimer = clockFrequency * ((mfmModeBit) ? SERVICE_MFM : SERVICE_FM);
#ifdef DEBUGFDC765
    printBytes("->", cmdBytes, cmdBuffer);
#endif

    switch (cmdBuffer[0] & 0x1F) {

        case 0x02:  // Read Track (Diagnostic)
            idmFound = false;
            ddmFound = false;
            eocFound = false;
            secFound = false;
            drive[cmdDrive()].hole = 0;
            firstSector = cmdBuffer[4];
            lastSector = cmdBuffer[6];
            currSector = 0;
            stage = FDC765Access::LOAD;
            state = FDC765State::EXECUTION;
            break;

        case 0x0A:  // Read ID
            idmFound = false;
            ddmFound = false;
            eocFound = false;
            drive[cmdDrive()].hole = 0;
            stage = FDC765Access::DATA;
            state = FDC765State::EXECUTION;
            break;

        case 0x06:  // Read sector(s)
        case 0x0C:  // Read deleted sector(s)
            eocFound = false;
            drive[cmdDrive()].hole = 0;
            firstSector = cmdBuffer[4];
            lastSector = cmdBuffer[6];
            useDeletedDAM = ((cmdBuffer[0] & 0x1F) == 0x0C);
            stage = FDC765Access::LOAD;
            state = FDC765State::EXECUTION;
            break;

        case 0x05:  // Write sector(s)
        case 0x09:  // Write deleted sector(s)
            eocFound = false;
            drive[cmdDrive()].hole = 0;
            firstSector = cmdBuffer[4];
            lastSector = cmdBuffer[6];
            useDeletedDAM = ((cmdBuffer[0] & 0x1F) == 0x09);
            stage = FDC765Access::LOAD;
            state = FDC765State::RECEIVE;
            break;

        case 0x03:  // Specify SPD/DMA
        case 0x04:  // Sense drive status
        case 0x07:  // Recalibrate and seek physical track 0
        case 0x08:  // Sense interrupt status
        case 0x0F:  // Seek track N
        case 0x10:  // Version
            state = FDC765State::EXECUTION;
            break;

        case 0x0D:  // Format track
            dataBytes = 4 * cmdBuffer[3];
            currSector = 0;
            // Clear the track here...
            drive[cmdDrive()].formatTrack(cmdHead(),
                    presCylNum[cmdDrive()], cmdHead(),
                    cmdBuffer[2], cmdBuffer[3], cmdBuffer[4], cmdBuffer[5]);
            stage = FDC765Access::LOAD;
            state = FDC765State::RECEIVE;
            break;

        case 0x11:  // Scan equal
        case 0x19:  // Scan low or equal
        case 0x1D:  // Scan high or equal
            eocFound = false;
            ddmFound = false;
            drive[cmdDrive()].hole = 0;
            firstSector = cmdBuffer[4];
            lastSector = cmdBuffer[6];
            stepSector = cmdBuffer[8];
            stage = FDC765Access::LOAD;
            state = FDC765State::RECEIVE;
            break;

        default:
            reset();
            break;
    }
}

void FDC765::execute() {

    switch (cmdBuffer[0] & 0x1F) {
        case 0x02:  // Read Track (Diagnostic)
            readTrackCmd();
            break;

        case 0x03:  // Specify SPD/DMA
            specifyCmd();
            break;

        case 0x04:  // Sense drive status
            sReg[3] = cmdBuffer[1] & 0x07;
            sReg[3] |= drive[cmdDrive()].senseStatus();
            resBuffer[0] = sReg[3];
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;

        case 0x05:  // Write sector(s)
        case 0x09:  // Write deleted sector(s)
            writeCmd();
            break;

        case 0x06:  // Read sector(s)
        case 0x0C:  // Read deleted sector(s)
            readCmd();
            break;

        case 0x07:  // Recalibrate and seek physical track 0
            // TODO: Concurrent recalibrate/seek operations are
            // possible for each drive.
            lastDrive = cmdDrive();
            sReg[0] = cmdBuffer[1] & 0x07;
            drive[cmdDrive()].recalibrate();
            presCylNum[cmdDrive()] = 0;
            if (drive[cmdDrive()].track0) {
                sReg[0] |= 0x20;    // 00100HUU - NT, SE.
            } else {
                sReg[0] |= 0x70;    // 01110HUU - AT, SE, EC.
            }
            interrupt = true;
            reset();
            break;

        case 0x08:  // Sense interrupt status
            if (!interrupt) {
                sReg[0] = 0x80;
            }
            resBuffer[0] = sReg[0];
            resBuffer[1] = presCylNum[lastDrive];
            interrupt = false;
            statusReg &= ~(SREG_DB0 | SREG_DB1 | SREG_DB2 | SREG_DB3);
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;

        case 0x0A:  // Read ID
            readIdCmd();
            break;

        case 0x0D:  // Format track
            formatCmd();
            break;

        case 0x0F:  // Seek track N
            // TODO: Concurrent recalibrate/seek operations are
            // possible for each drive.
            statusReg |= SREG_DB0 | SREG_DB1 | SREG_DB2 | SREG_DB3;
            if (drive[cmdDrive()].ready == false) {
                sReg[0] = cmdBuffer[1] & 0x07;
                sReg[0] |= 0x48;    // 01001HUU - AT, NR.
                interrupt = true;
                reset();
            } else if (presCylNum[cmdDrive()] == cmdBuffer[2]) {
                sReg[0] = cmdBuffer[1] & 0x07;
                sReg[0] |= 0x20;    // 00100HUU - NT, SE.
                interrupt = true;
                reset();
            } else if (presCylNum[cmdDrive()] > cmdBuffer[2]) {
                drive[cmdDrive()].prevTrack();
                --presCylNum[cmdDrive()];
            } else {
                drive[cmdDrive()].nextTrack();
                ++presCylNum[cmdDrive()];
            }
            break;

        case 0x10:  // Version
            resBuffer[0] = 0x80;
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;

        case 0x11:  // Scan equal
        case 0x19:  // Scan low or equal
        case 0x1D:  // Scan high or equal
            cout << "Not yet implemented." << endl;
            scanCmd();
            break;

        default:
            break;
    }
}

void FDC765::formatCmd() {

    switch (stage) {
        case FDC765Access::LOAD:
            if (headLoadOp()) {
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::SEEK:
            // Seek stage. Here we try to find the disk hole.
            if (findHoleOp()) {
                stage = FDC765Access::DATA;
            }
            // If we don't find the hole, the diskette is weird or
            // the LED is broken...
            break;

        case FDC765Access::DATA:
            // Get four id bytes for each sector
            if (currSector < cmdBuffer[3]) {
                uint_fast8_t fmtTrack = dataBuffer[4 * currSector];
                uint_fast8_t fmtHead = dataBuffer[4 * currSector + 1];
                uint_fast8_t fmtSector = dataBuffer[4 * currSector + 2];
                uint_fast8_t fmtSize = dataBuffer[4 * currSector + 3];
                drive[cmdDrive()].buffer.assign(0x80 << fmtSize, cmdBuffer[5]);
                drive[cmdDrive()].formatSector(cmdHead(),
                        fmtTrack, fmtHead, fmtSector, fmtSize);
                drive[cmdDrive()].nextSector();
                ++currSector;
            } else {
                sReg[0] = (cmdBuffer[1] & 0x07);
                sReg[1] = 0x00;
                sReg[2] = 0x00;
                resTrack = dataBuffer[dataBytes - 4];
                resHead = dataBuffer[dataBytes - 3];
                resSector = dataBuffer[dataBytes - 2] + 1;
                resSize = dataBuffer[dataBytes - 1];
                stage = FDC765Access::UNLOAD;
            }
            break;

        case FDC765Access::UNLOAD:
            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = resTrack;
            resBuffer[4] = resHead;
            resBuffer[5] = resSector;
            resBuffer[6] = resSize;
            unload = true;
            interrupt = true;
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;

        default:
            reset();
            break;
    }
}

bool FDC765::seekForReadOp() {

    drive[cmdDrive()].readSector(cmdHead());
    drive[cmdDrive()].nextSector();

    // Check No Data (ND) condition.
    if (((drive[cmdDrive()].statusReg1) & 0x04) == 0x04) {
        return false;
    }

    currSector = drive[cmdDrive()].idSector;

    // Track and sector are checked.
    if (drive[cmdDrive()].idTrack != cmdBuffer[2]) {
        sReg[2] |= 0x10;        // WC
        if (drive[cmdDrive()].idTrack == 0xFF) {
            sReg[2] |= 0x02;    // BC
        }
        sReg[0] |= 0x40;        // AT
        return true;
    }

    return (currSector == firstSector);
}

bool FDC765::readOp() {

    bool done = false;
#ifdef DEBUGFDC765
    cout << firstSector << "..";
#endif

    // We already have read the sector in the seekForReadOp, and
    // currSector holds the current sector number.
    sReg[0] = cmdBuffer[1] & 0x07;
    sReg[1] = drive[cmdDrive()].statusReg1;
    sReg[2] = drive[cmdDrive()].statusReg2;

    // Now we look at the data mark.
    if ((sReg[2] & 0x40) == (useDeletedDAM ? 0x00 : 0x40)) {
        done = readDeletedDataOp();
    } else {
        done = readRegularDataOp();
    }

    if (done) {
        drive[cmdDrive()].nextSector();
        if (sReg[0] == 0x00 && sReg[1] == 0x00 && currSector >= lastSector) {
            eocFound = true;
        }

        setResultBytesOp();
    }

    return done;
}

void FDC765::setResultBytesOp() {

    resTrack = cmdBuffer[2];
    resHead = cmdBuffer[3];
    resSector = cmdBuffer[4];
    resSize = cmdBuffer[5];

    if (eocFound) {
        sReg[0] |= 0x40;
        sReg[1] |= 0x80;
    }
}

bool FDC765::readRegularDataOp() {

#ifdef DEBUGFDC765
    cout << "Read..";
#endif
    bool error = false;

    size_t actlen = drive[cmdDrive()].length;
    size_t outlen = cmdBuffer[5] ? (0x80 << cmdBuffer[5]) : cmdBuffer[8];
    size_t offset = 0;

    // If actlen is a multiple of outlen, there are several
    // copies of this sector and we have to choose one.
    if ((actlen > outlen) && (actlen % outlen) == 0) {
        size_t sectorCopies = actlen / outlen;
        offset = outlen * sectorCopy;
        sectorCopy = (sectorCopy + 1) % sectorCopies;
    }

    // If the actual sector size doesn't match the requested sector size,
    // then signal ND (No Data)
    if (drive[cmdDrive()].idSize != cmdBuffer[5]) {
        sReg[0] |= 0x40;    // 01000HUU - AT
        sReg[1] |= 0x04;    // xxxxx1xx - ND
    }

    size_t finish = min(offset + outlen, drive[cmdDrive()].buffer.size());
    vector<uint8_t> buf(&drive[cmdDrive()].buffer[offset],
            &drive[cmdDrive()].buffer[finish]);

    // Complete length
    buf.resize(outlen, drive[cmdDrive()].filler);

    // Detect Speedlock protection:
    // CRC error on track 00, sector 02, which is 512 bytes long.
    if (((sReg[2] & 0x20) == 0x20) && cmdBuffer[2] == 0x00
            && cmdBuffer[4] == 0x02 && cmdBuffer[5] == 0x02) {
        randomizeSector(buf);
        sReg[0] |= 0x40;    // 01000HUU - AT
        error = true;
    }

    // If we reach here, it is a normal data read. Dump and continue.
    appendToDataBuffer(buf);

    // We must signal the CM.
    sReg[2] &= 0xBF;
    return (error || currSector >= lastSector);
}

bool FDC765::readDeletedDataOp() {

    size_t actlen = drive[cmdDrive()].length;
    size_t outlen = cmdBuffer[5] ? (0x80 << cmdBuffer[5]) : cmdBuffer[8];
    size_t offset = 0;

    if (skipDeletedBit) {
#ifdef DEBUGFDC765
        cout << "Skip deleted..";
#endif
        return false;
    }
#ifdef DEBUGFDC765
    cout << "Read deleted..";
#endif

    // If actlen is a multiple of outlen, there are several
    // copies of this sector and we have to choose one.
    if ((actlen > outlen) && (actlen % outlen) == 0) {
        size_t sectorCopies = actlen / outlen;
        offset = outlen * sectorCopy;
        sectorCopy = (sectorCopy + 1) % sectorCopies;
    }

    // If the actual sector size doesn't match the requested sector size,
    // then signal ND (No Data)
    if (drive[cmdDrive()].idSize != cmdBuffer[5]) {
        sReg[0] |= 0x40;    // 01000HUU - AT
        sReg[1] |= 0x04;    // xxxxx1xx - ND
    }

    size_t finish = min(offset + outlen, drive[cmdDrive()].buffer.size());
    vector<uint8_t> buf(&drive[cmdDrive()].buffer[offset],
            &drive[cmdDrive()].buffer[finish]);

    // Complete length
    buf.resize(outlen, drive[cmdDrive()].filler);

    // Detect Speedlock protection:
    // CRC error on track 00, sector 02, which is 512 bytes long.
    if (((sReg[2] & 0x20) == 0x20) && cmdBuffer[2] == 0x00
            && cmdBuffer[4] == 0x02 && cmdBuffer[5] == 0x02) {
        randomizeSector(buf);
        sReg[0] |= 0x40;    // 01000HUU - AT
    }

    appendToDataBuffer(buf);

    // We must signal the CM.
    sReg[2] |= 0x40;

    return true;
}

void FDC765::readCmd() {

    switch (stage) {
        case FDC765Access::LOAD:
            if (headLoadOp()) {
                sReg[0] &= 0x3F;
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::SEEK:
            // Seek stage. Here we read all the sectors in the track,
            // until we find one whose ID matches command parameter R.
            // If during the seek we find the hole mark twice, this means
            // that the sector we're looking for does not exist, and we
            // end the operation with an error.
            if (seekForReadOp()) {
                if ((sReg[0] & 0xC0) == 0x00) {
                    stage = FDC765Access::DATA;
                } else {
                    dataBytes = 0;
                    stage = FDC765Access::UNLOAD;
                }
            } else if (drive[cmdDrive()].hole > 1) {
                dataBytes = 0;
                sReg[1] |= 0x05;    // xxxxx1x1 - ND - MAD
                sReg[0] |= 0x40;    // 01000HUU - AT
                stage = FDC765Access::UNLOAD;
            }
            break;

        case FDC765Access::DATA:
            if (readOp()) {
                stage = FDC765Access::UNLOAD;
            } else {
                // Sectors can be unordered.
                drive[cmdDrive()].hole = 0;
                ++firstSector;
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::UNLOAD:
            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = resTrack;
            resBuffer[4] = resHead;;
            resBuffer[5] = resSector;
            resBuffer[6] = resSize;
            interrupt = true;
            unload = true;
            state = dataBytes ?
                FDC765State::TRANSMIT : FDC765State::RESULT;
            outputByteReady = true;
#ifdef DEBUGFDC765
            printBytes("<-", dataBytes, dataBuffer);
            printBytes("<-", resBytes, resBuffer);
#endif
            break;

        case FDC765Access::NONE:
            reset();
            break;
    }
}

void FDC765::readIdCmd() {

    switch (stage) {
        case FDC765Access::DATA:
            if (readIdOp()) {
                stage = FDC765Access::UNLOAD;
            }
            break;

        case FDC765Access::UNLOAD:
            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = resTrack;
            resBuffer[4] = resHead;
            resBuffer[5] = resSector;
            resBuffer[6] = resSize;
            interrupt = true;
            unload = true;
            state = FDC765State::RESULT;
            outputByteReady = true;
#ifdef DEBUGFDC765
            printBytes("<-", resBytes, resBuffer);
#endif
            break;

        default:
            reset();
            break;
    }
}

bool FDC765::readIdOp() {

    sReg[0] = cmdBuffer[1] & 0x07;
    sReg[1] = 0x00;
    sReg[2] = 0x00;

    drive[cmdDrive()].readSector(cmdHead());
    drive[cmdDrive()].nextSector();

    if (loaded) {
        sReg[1] = drive[cmdDrive()].statusReg1;
        sReg[2] = drive[cmdDrive()].statusReg2;
    }

    resTrack = drive[cmdDrive()].idTrack;
    resHead = drive[cmdDrive()].idHead;
    resSector = drive[cmdDrive()].idSector;
    resSize = drive[cmdDrive()].idSize;

    if (drive[cmdDrive()].hole > 1) {
        sReg[1] |= 0x01;    // xxxxxxx1 - MAD
        sReg[1] |= 0x04;    // xxxxx1xx - ND
        sReg[0] |= 0x40;    // 01000HUU - AT
        return true;
    } else {
        // Check for Missing Address Mark or No Data.
        bool crcOk = ((sReg[1] & 0x20) == 0x00);
        idmFound = ((sReg[1] & 0x01) == 0x00);
        ddmFound = ((sReg[1] & 0x04) == 0x00);
        return idmFound && ddmFound && crcOk;
    }
}

bool FDC765::headLoadOp() {

    --loadTimer;
    if (!loadTimer) {
        loadTimer = headLoadTime;
        loaded = true;
    }

    return loaded;
}

void FDC765::updateHeadUnload() {

    if (loaded && unload) {
        --unloadTimer;

        if (!unloadTimer) {
            unloadTimer = headUnloadTime;
            loaded = false;
            unload = false;
        }
    }
}

void FDC765::readTrackCmd() {

    switch (stage) {
        case FDC765Access::LOAD:
            if (headLoadOp()) {
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::SEEK:
            // Seek stage. Here we try to find the disk hole.
            if (findHoleOp()) {
                stage = FDC765Access::DATA;
            }
            // If we don't find the hole, the diskette is weird or
            // the LED is broken...
            break;

        case FDC765Access::DATA:
            if (readTrackOp()) {
                stage = FDC765Access::UNLOAD;
            }
            break;

        case FDC765Access::UNLOAD:
            if (!idmFound) {
                sReg[0] |= 0x40;
                sReg[1] |= 0x01;
            }

            if (!ddmFound) {
                sReg[0] |= 0x40;
                sReg[2] |= 0x01;
            }

            if (!ddmFound && !idmFound) {
                sReg[1] |= 0x80;
            }

            if (!secFound) {
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
            unload = true;
            interrupt = true;
            state = dataBytes ?
                FDC765State::TRANSMIT : FDC765State::RESULT;
            outputByteReady = true;
            break;

        default:
            reset();
            break;
    }
}

bool FDC765::findHoleOp() {

    drive[cmdDrive()].hole = 0;
    drive[cmdDrive()].nextSector();
    return (drive[cmdDrive()].hole > 0);
}

bool FDC765::readTrackOp() {

    drive[cmdDrive()].readSector(cmdHead());
    sReg[0] = cmdBuffer[1] & 0x07;
    sReg[1] = drive[cmdDrive()].statusReg1;
    sReg[2] = drive[cmdDrive()].statusReg2;

    ++currSector;

    size_t actlen = drive[cmdDrive()].length;
    size_t outlen;

    // We determine how many bytes to read from this sector.
    // If SZ = 0, use SL
    outlen = cmdBuffer[5] ? (0x80 << cmdBuffer[5]) : cmdBuffer[8];

    // Double check that the intended data length does
    // not exceed the ACTUAL data length.
    if (actlen != 0 && outlen > actlen) {
        outlen = actlen;
    }

    vector<uint8_t> buf(
            &drive[cmdDrive()].buffer[0], &drive[cmdDrive()].buffer[outlen]);

    // Detect Speedlock protection:
    // CRC error on track 00, sector 02, which is 512 bytes long.
    if (((sReg[2] & 0x20) == 0x20) && cmdBuffer[2] == 0x00
            && cmdBuffer[4] == 0x02 && cmdBuffer[5] == 0x02) {
        randomizeSector(buf);
    }

    // If we reach here, it is a normal data read. Dump and continue.
    appendToDataBuffer(buf);

    setResultBytesOp();

    if ((sReg[1] & 0x01) == 0x00) {
        idmFound = true;
    }

    if ((sReg[2] & 0x01) == 0x00) {
        ddmFound = true;
    }

    if (idmFound || ddmFound) {
        secFound = true;
    }

    drive[cmdDrive()].nextSector();
    return ((currSector == lastSector) || (drive[cmdDrive()].hole > 1));
}

bool FDC765::seekForWriteOp() {

    drive[cmdDrive()].readSector(cmdHead());
    sReg[0] = cmdBuffer[1] & 0x07;
    sReg[1] = drive[cmdDrive()].statusReg1;
    sReg[2] = drive[cmdDrive()].statusReg2;

    // Check bad CRC
    if ((sReg[1] & 0x20) == 0x20) {
        sReg[0] |= 0x40;        // AT
        return true;
    }

    // Check No Data (ND) condition and IDR
    if (drive[cmdDrive()].idTrack != cmdBuffer[2]
            || drive[cmdDrive()].idHead != cmdBuffer[3]
            || drive[cmdDrive()].idSector != firstSector
            || drive[cmdDrive()].idSize != cmdBuffer[5]) {
        drive[cmdDrive()].nextSector();
        return false;
    }

    // If we arrive here, we've found our sector
    currSector = drive[cmdDrive()].idSector;
    return true;
}

void FDC765::writeCmd() {

    switch (stage) {
        case FDC765Access::LOAD:
            if (headLoadOp()) {
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::SEEK:
            // Seek stage. Here we read all the sectors in the track,
            // until we find one whose coordinates (C, H, R, N) match
            // the ones given in the command.
            // If during the seek we find the hole mark twice, this means
            // that the sector we're looking for does not exist, and we
            // end the operation with an error.
            if (seekForWriteOp()) {
                if ((sReg[0] & 0xC0) == 0x00) {
                    dataBytes = cmdBuffer[5] ? (0x80 << cmdBuffer[5]) : cmdBuffer[8];
                    stage = FDC765Access::DATA;
                    state = FDC765State::RECEIVE;
                } else {
                    dataBytes = 0;
                    stage = FDC765Access::UNLOAD;
                }
            } else if (drive[cmdDrive()].hole > 1) {
                dataBytes = 0;
                sReg[1] |= 0x05;    // xxxxx1x1 - ND - MAD
                sReg[0] |= 0x40;    // 01000HUU - AT
                stage = FDC765Access::UNLOAD;
            }
            break;

        case FDC765Access::DATA:
            {
                vector<uint8_t> buffer = drive[cmdDrive()].buffer;
                buffer.resize(dataBytes, 0);

                // If we're doing a multisector transfer, we copy this sector
                // and remove the copied part from the dataBuffer.
                if (dataIndex >= dataBytes) {
                    copy(&dataBuffer[0], &dataBuffer[dataBytes], &buffer[0]);
                    copy(&dataBuffer[dataBytes], &dataBuffer[dataIndex], &dataBuffer[0]);
                    dataIndex -= dataBytes;
                } else {
                    copy(&dataBuffer[0], &dataBuffer[dataIndex], &buffer[0]);
                    dataIndex = 0;
                }

                drive[cmdDrive()].buffer = buffer;
                drive[cmdDrive()].statusReg1 = 0x00;
                drive[cmdDrive()].statusReg2 |= useDeletedDAM ? 0x40 : 0x00;
                drive[cmdDrive()].writeSector(cmdHead());
                drive[cmdDrive()].nextSector();
            }

            if (((sReg[0] & 0xC0) != 0x00) || currSector == lastSector) {
                eocFound = true;
                setResultBytesOp();
                stage = FDC765Access::UNLOAD;
            } else {
                // Sectors can be unordered.
                drive[cmdDrive()].hole = 0;
                ++firstSector;
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::UNLOAD:
            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = resTrack;
            resBuffer[4] = resHead;;
            resBuffer[5] = resSector;
            resBuffer[6] = resSize;
            interrupt = true;
            unload = true;
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;

        case FDC765Access::NONE:
            reset();
            break;
    }
}

void FDC765::specifyCmd() {

    // The Head Unload Time is programmable from 16 to 240ms in increments of
    // 16ms (01 = 16ms, 02 = 32ms ... 0F = 240ms)
    // The Step Rate Time is programmable from 1 to 16ms in increments of 1ms.
    // (F = 1ms, E = 2ms, D = 3ms, etc.)
    // The Head Load Time is programmable from 2 to 254ms in increments of
    // 2ms. (01 = 2ms, 02 = 4ms, 03 = 6ms ... 7F = 254ms)
    // The time intervals mentioned above are a direct function of the clock.
    // Times indicated above are for an 8MHz clock; if the clock was reduced
    // to 4MHz (mini-floppy application) then all time intervals are increased
    // by a factor of 2.
    // Since the base frequency I'm using here is 1MHz (FDC clock in the ZX
    // Spectrum is 1MHz), I've got to multiply all intervals times 8:
    // 16ms * 8 = 128ms.
    // 2ms * 8 = 16ms.
    headUnloadTime = (cmdBuffer[1] & 0x0F) * 16 * DELAY_1ms;
    stepRateTime = (0x10 - ((cmdBuffer[1] & 0xF0) >> 4)) * 128 * DELAY_1ms;
    headLoadTime = (cmdBuffer[2] >> 1) * 2 * DELAY_1ms;
    useDma = ((cmdBuffer[2] & 0x01) == 0x00);
    loadTimer = headLoadTime;
    unloadTimer = headUnloadTime;
    reset();
}

void FDC765::scanCmd() {

    switch (stage) {
        case FDC765Access::LOAD:
            if (headLoadOp()) {
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::SEEK:
            // Seek stage. Here we read all the sectors in the track,
            // until we find one whose coordinates (C, H, R, N) match
            // the ones given in the command.
            // If during the seek we find the hole mark twice, this means
            // that the sector we're looking for does not exist, and we
            // end the operation with an error.
            if (seekForReadOp()) {
                if ((sReg[0] & 0xC0) == 0x00) {
                    dataBytes = 0x80 << cmdBuffer[5];
                    stage = FDC765Access::DATA;
                    state = FDC765State::RECEIVE;
                } else {
                    dataBytes = 0;
                    stage = FDC765Access::UNLOAD;
                }
            } else if (drive[cmdDrive()].hole > 1) {
                dataBytes = 0;
                sReg[1] |= 0x05;    // xxxxx1x1 - ND - MAD
                sReg[0] |= 0x40;    // 01000HUU - AT
                stage = FDC765Access::UNLOAD;
            }
            break;

        case FDC765Access::DATA:
            {
                bool done = false;

                // We already have read the sector in the seekForReadOp, and
                // currSector holds the current sector number.
                sReg[0] = cmdBuffer[1] & 0x07;
                sReg[1] = drive[cmdDrive()].statusReg1;
                sReg[2] = drive[cmdDrive()].statusReg2;

                // If the FDC encounters a Deleted Data Address Mark on one of
                // the sectors:
                // SK = 0: It regards the sector as the last sector on the
                //         cylinder, sets CM (Control Mark) flag on sReg[2]
                // and terminates the command.
                // SK = 1: It skips the sector with the Deleted Address Mark
                //         and reads the next sector. In the second case, the
                //         sReg[2] CM (Control Mark) bit is set in order to
                //         show that a Deleted Sector has been encountered.
                if ((sReg[2] & 0x40) == 0x40) {
                    if (skipDeletedBit) {
                    } else {
                        done = true;
                    }
                } else {
                    done = readRegularDataOp();
                }

                if (done) {
                    drive[cmdDrive()].nextSector();
                    if (sReg[0] == 0x00 && sReg[1] == 0x00 && currSector == lastSector) {
                        eocFound = true;
                    }

                    setResultBytesOp();
                }

                vector<uint8_t> buffer = drive[cmdDrive()].buffer;
                buffer.resize(dataBytes, 0);
            }

            if (((sReg[0] & 0xC0) != 0x00) || currSector == lastSector) {
                eocFound = true;
                setResultBytesOp();
                stage = FDC765Access::UNLOAD;
            } else {
                // Sectors can be unordered.
                drive[cmdDrive()].hole = 0;
                ++firstSector;
                stage = FDC765Access::SEEK;
            }
            break;

        case FDC765Access::UNLOAD:
            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = resTrack;
            resBuffer[4] = resHead;;
            resBuffer[5] = resSector;
            resBuffer[6] = resSize;
            interrupt = true;
            unload = true;
            state = FDC765State::RESULT;
            outputByteReady = true;
            break;

        case FDC765Access::NONE:
            reset();
            break;
    }
}

void FDC765::reset() {

    state = FDC765State::IDLE;
    stage = FDC765Access::NONE;
    mode = FDC765Mode::NONE;
    outputByteReady = false;
    inputByteReady = false;
    cmdIndex = resIndex = dataIndex = 0;
    cmdBytes = resBytes = dataBytes = 0;
    statusReg = SREG_RQM;
}

uint_fast8_t FDC765::read() {

    static uint_fast8_t retval = 0x00;

#ifdef DEBUGFDC765
    cout << "Attempt FDC data reg: ";
#endif
    if (outputByteReady) {
        if (mode == FDC765Mode::READ) {
            retval = dataBuffer[dataIndex++];
            dataIndex %= DATABUFFER_SIZE;
#ifdef DEBUGFDC765
            cout << "Read FDC data reg (data): ";
            cout << hex << setw(2) << setfill('0') << static_cast<uint32_t>(retval) << endl;
#endif
        } else {
            retval = resBuffer[resIndex++];
            resIndex %= RESBUFFER_SIZE;
#ifdef DEBUGFDC765
            cout << "Read FDC data reg (result): ";
            cout << hex << setw(2) << setfill('0') << static_cast<uint32_t>(retval) << endl;
#endif
        }
        outputByteReady = false;
    }
    return retval;
}

void FDC765::write(uint_fast8_t value) {

    if (!inputByteReady) {
        if (mode == FDC765Mode::WRITE) {
            dataBuffer[dataIndex++] = value;
            dataIndex %= DATABUFFER_SIZE;
        } else {
            cmdBuffer[cmdIndex++] = value;
            cmdIndex %= CMDBUFFER_SIZE;
        }
        inputByteReady = true;
    }
}

uint_fast8_t FDC765::status() {

#ifdef DEBUGFDC765
    cout << "FDC status reg: ";
    cout << hex << setw(2) << setfill('0') << static_cast<uint32_t>(statusReg) << endl;
#endif
    return statusReg;
}

void FDC765::checkDrive() {

    if (cmdDrive() > 1 || cmdHead() > 0) {
        sReg[0] = cmdBuffer[0];
        sReg[0] |= 0xC4;    // 01..1HUU: AT, NR
        stage = FDC765Access::UNLOAD;
    }
}

void FDC765::motor(bool status) {

    drive[0].motor = status;
    drive[1].motor = status;
}

void FDC765::randomizeSector(vector<uint8_t>& buf) {

    // Theoretically, there is a pattern here. However, this seems to work.
    buf.back() |= rand() & 0xFF;
}

void FDC765::appendToDataBuffer(vector<uint8_t>& buf) {

    size_t available = DATABUFFER_SIZE - dataBytes;
    if (available < buf.size()) {
        buf.resize(available, 0x00);
    }
    copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
    dataBytes += buf.size();
}
// vim: et:sw=4:ts=4
