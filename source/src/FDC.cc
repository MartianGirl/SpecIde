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

#include "FDC.h"

using namespace std;

void FDC::clock() {

    if (loaded && unload) {
        --unloadTimer;

        if (!unloadTimer) {
            unloadTimer = headUnloadTime;
            loaded = false;
            unload = false;
        }
    }

    switch (state) {
        case FDCState::FDC_STATE_IDLE:
            statusReg = SREG_RQM;
            if (byte) {
                if (checkCommand()) {
                    state = FDCState::FDC_STATE_COMMAND;
                } else {
                    sReg[0] = 0x80;
                    resBuffer[0] = sReg[0];
                    state = FDCState::FDC_STATE_RESULT;
                }
            }
            break;

        case FDCState::FDC_STATE_COMMAND:
            statusReg = SREG_RQM | SREG_CB;
            if (byte) {
                byte = false;
                if (cmdIndex == cmdBytes) {
                    setup();    // TODO: FDCState as return value.
                    switch (mode) {
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
                } else if (cmdIndex > cmdBytes) {
                    reset();
                }
            }
            break;

        case FDCState::FDC_STATE_EXECUTION:
            statusReg = SREG_EXM | SREG_CB;
            execute();
            break;

        case FDCState::FDC_STATE_RECEIVE:
            statusReg = SREG_RQM | SREG_CB | SREG_EXM;

            if (byte) {
                byte = false;
            }
            break;

        case FDCState::FDC_STATE_TRANSMIT:
            statusReg = SREG_RQM | SREG_DIO | SREG_CB | SREG_EXM;

            if (transfer) {
                serviceTimer--;
            }

            if (byte) {
                byte = false;
                transfer = true;
                serviceTimer = (mfmModeBit) ? SERVICE_MFM : SERVICE_FM;

                if (dataIndex >= dataBytes) {
                    transfer = false;
                    state = FDCState::FDC_STATE_RESULT;
                }
            }

            // Signal Over Run if FDC is not serviced quickly enough.
            if (serviceTimer == 0) {
                transfer = false;
                sReg[1] |= 0x10;    // OR
                sReg[0] |= 0x40;    // AT
                interrupt = true;
                state = FDCState::FDC_STATE_RESULT;
            }
            break;

        case FDCState::FDC_STATE_RESULT:
            statusReg = SREG_RQM | SREG_DIO | SREG_CB;
            if (byte) {
                byte = false;
                if (resIndex >= resBytes) {
                    reset();
                }
            }
            break;
    }
}

bool FDC::checkCommand() {

    switch (cmdBuffer[0] & 0x1F)
    {
        case 0x02:  // Read Track (Diagnostic)
            cmdBytes = 9;   // 02+MF+SK    HU TR HD SC SZ NM GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD NM SZ
            mode = FDCMode::FDC_MODE_READ;
            return true;

        case 0x03:  // Specify SPD/DMA
            cmdBytes = 3;   // 03          XX YY
            resBytes = 0;   //
            mode = FDCMode::FDC_MODE_NONE;
            return true;

        case 0x04:  // Sense drive status
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
            cmdBytes = 9;   // 06+MT+MF+SK HU TR HD SC SZ LS GP SL
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            mode = FDCMode::FDC_MODE_READ;
            return true;

        case 0x07:  // Recalibrate and seek physical track 0
            cmdBytes = 2;   // 07          HU
            resBytes = 0;
            mode = FDCMode::FDC_MODE_NONE;
            return true;

        case 0x08:  // Sense interrupt status
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
            cmdBytes = 2;   // 0A+MF       HU
            resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
            mode = FDCMode::FDC_MODE_NONE;
            return true;

        case 0x0C:  // Read deleted sector(s)
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
            cmdBytes = 3;   // 0F          HU TP
            resBytes = 0;
            mode = FDCMode::FDC_MODE_NONE;
            return true;

        case 0x10:  // Version
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

void FDC::setup() {

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
            stage = FDCAccess::FDC_ACCESS_DATA;
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

void FDC::execute() {

    switch (cmdBuffer[0] & 0x1F) {
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

bool FDC::seekOp() {

    drive[cmdDrive()].readSector(cmdHead());
    drive[cmdDrive()].nextSector();

    // Check No Data (ND) condition.
    if (((drive[cmdDrive()].statusReg1) & 0x04) == 0x04) {
        return false;
    }

    currSector = drive[cmdDrive()].idSector;

    // Track and sector are checked.
    if (drive[cmdDrive()].idTrack != cmdBuffer[2]) {
        sReg[2] |= 0x10;    // WC
        if (drive[cmdDrive()].idTrack == 0xFF) {
            sReg[2] |= 0x02;    // BC
        }
        sReg[0] |= 0x40;    // AT
        return true;
    }

    return ((currSector == firstSector)
            && drive[cmdDrive()].idSize == cmdBuffer[5]);
}

bool FDC::readOp() {

    bool done = false;

    // We already have read the sector in the seekOp, and
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
        if (sReg[0] == 0x00 && sReg[1] == 0x00 && currSector == lastSector) {
            eocFound = true;
        }

        setResultBytesOp();
    }

    return done;
}

void FDC::setResultBytesOp() {

    resTrack = cmdBuffer[2];
    resHead = cmdBuffer[3];
    resSector = cmdBuffer[4];
    resSize = cmdBuffer[5];

    if (eocFound) {
        sReg[0] |= 0x40;
        sReg[1] |= 0x80;
    }
}

bool FDC::readRegularDataOp() {

    bool error = false;

    size_t actlen = drive[cmdDrive()].length;
    size_t outlen = 0x80 << cmdBuffer[5];   // Use SL as sector length.
    size_t offset = 0;

    // If SL = 0, Use SZ as sector length.
    if (cmdBuffer[5] == 0x00) {
        outlen = cmdBuffer[8];
    }

    // If actlen is a multiple of outlen, there are several
    // copies of this sector and we have to choose one.
    if ((actlen > outlen) && (actlen % outlen) == 0) {
        size_t sectorCopies = actlen / outlen;
        offset = outlen * sectorCopy;
        sectorCopy = (sectorCopy + 1) % sectorCopies;
    }

    size_t finish = min(offset + outlen, drive[cmdDrive()].buffer.size());
    vector<uint8_t> buf(&drive[cmdDrive()].buffer[offset],
            &drive[cmdDrive()].buffer[finish]);

    // Complete length
    buf.insert(buf.end(), (outlen - buf.size()), 0x00);

    // Detect Speedlock protection:
    // CRC error on track 00, sector 02, which is 512 bytes long.
    if (((sReg[2] & 0x20) == 0x20) && cmdBuffer[2] == 0x00
            && cmdBuffer[4] == 0x02 && cmdBuffer[5] == 0x02) {
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

bool FDC::readDeletedDataOp() {

    size_t actlen = drive[cmdDrive()].length;
    size_t outlen = 0x80 << drive[cmdDrive()].idSize;
    size_t offset = 0;

    if (skipDeletedBit) {
        return false;
    }

    // If actlen is a multiple of outlen, there are several
    // copies of this sector and we have to choose one.
    if ((actlen > outlen) && (actlen % outlen) == 0) {
        size_t sectorCopies = actlen / outlen;
        offset = outlen * sectorCopy;
        sectorCopy = (sectorCopy + 1) % sectorCopies;
    }

    size_t finish = min(offset + outlen, drive[cmdDrive()].buffer.size());
    vector<uint8_t> buf(&drive[cmdDrive()].buffer[offset],
            &drive[cmdDrive()].buffer[finish]);

    // Complete length
    buf.insert(buf.end(), (outlen - buf.size()), 0x00);

    // Detect Speedlock protection:
    // CRC error on track 00, sector 02, which is 512 bytes long.
    if (((sReg[2] & 0x20) == 0x20) && cmdBuffer[2] == 0x00
            && cmdBuffer[4] == 0x02 && cmdBuffer[5] == 0x02) {
        randomizeSector(buf);
        sReg[0] |= 0x40;    // 01000HUU - AT
    }

    copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
    dataBytes += outlen;

    // We must signal the CM.
    sReg[2] |= 0x40;

    return true;
}

void FDC::readCmd() {

    switch (stage) {
        case FDCAccess::FDC_ACCESS_LOAD:
            if (headLoadOp()) {
                sReg[0] &= 0x3F;
                stage = FDCAccess::FDC_ACCESS_SEEK;
            }
            break;

        case FDCAccess::FDC_ACCESS_SEEK:
            // Seek stage. Here we read all the sectors in the track,
            // until we find one whose ID matches command parameter R.
            // If during the seek we find the hole mark twice, this means
            // that the sector we're looking for does not exist, and we
            // end the operation with an error.
            if (seekOp()) {
                if ((sReg[0] & 0xC0) == 0x00) {
                    stage = FDCAccess::FDC_ACCESS_DATA;
                } else {
                    dataBytes = 0;
                    stage = FDCAccess::FDC_ACCESS_UNLOAD;
                }
            } else if (drive[cmdDrive()].hole > 1) {
                dataBytes = 0;
                sReg[1] |= 0x05;    // xxxxx1x1 - ND - MAD
                sReg[0] |= 0x40;    // 01000HUU - AT
                stage = FDCAccess::FDC_ACCESS_UNLOAD;
            }
            break;

        case FDCAccess::FDC_ACCESS_DATA:
            if (readOp()) {
                stage = FDCAccess::FDC_ACCESS_UNLOAD;
            } else {
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
            interrupt = true;
            unload = true;
            state = dataBytes ?
                FDCState::FDC_STATE_TRANSMIT : FDCState::FDC_STATE_RESULT;
            break;

        case FDCAccess::FDC_ACCESS_NONE:
            reset();
            break;
    }
}

void FDC::readIdCmd() {

    switch (stage) {
        case FDCAccess::FDC_ACCESS_DATA:
            if (readIdOp()) {
                stage = FDCAccess::FDC_ACCESS_UNLOAD;
            }
            break;

        case FDCAccess::FDC_ACCESS_UNLOAD:
            resBuffer[0] = sReg[0];
            resBuffer[1] = sReg[1];
            resBuffer[2] = sReg[2];
            resBuffer[3] = resTrack;
            resBuffer[4] = resHead;
            resBuffer[5] = resSector;
            resBuffer[6] = resSize;
            interrupt = true;
            unload = true;
            state = FDCState::FDC_STATE_RESULT;
            break;

        default:
            reset();
            break;
    }
}

bool FDC::readIdOp() {

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

bool FDC::headLoadOp() {

    --loadTimer;
    if (!loadTimer) {
        loadTimer = headLoadTime;
        loaded = true;
    }

    return loaded;
}

void FDC::readTrackCmd() {

    switch (stage) {
        case FDCAccess::FDC_ACCESS_LOAD:
            if (headLoadOp()) {
                stage = FDCAccess::FDC_ACCESS_SEEK;
            }
            break;

        case FDCAccess::FDC_ACCESS_SEEK:
            // Seek stage. Here we try to find the disk hole.
            if (findHoleOp()) {
                stage = FDCAccess::FDC_ACCESS_DATA;
            }
            // If we don't find the hole, the diskette is weird or
            // the LED is broken...
            break;

        case FDCAccess::FDC_ACCESS_DATA:
            if (readTrackOp()) {
                stage = FDCAccess::FDC_ACCESS_UNLOAD;
            }
            break;

        case FDCAccess::FDC_ACCESS_UNLOAD:
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
                FDCState::FDC_STATE_TRANSMIT : FDCState::FDC_STATE_RESULT;
            break;

        default:
            reset();
            break;
    }
}

bool FDC::findHoleOp() {

    drive[cmdDrive()].hole = 0;
    drive[cmdDrive()].nextSector();
    return (drive[cmdDrive()].hole > 0);
}

bool FDC::readTrackOp() {

    drive[cmdDrive()].readSector(cmdHead());
    sReg[0] = cmdBuffer[1] & 0x07;
    sReg[1] = drive[cmdDrive()].statusReg1;
    sReg[2] = drive[cmdDrive()].statusReg2;

    ++currSector;

    size_t actlen = drive[cmdDrive()].length;
    size_t outlen;

    // We determine how many bytes to read from this sector.
    // If SZ = 0, use SL
    outlen = (cmdBuffer[5]) ? (0x80 << cmdBuffer[5]) : cmdBuffer[8];

    // Double check that the intended data length does
    // not exceed the ACTUAL data length.
    if (actlen != 0 && outlen > actlen) {
        outlen = actlen;
    }

    vector<uint8_t> buf(outlen, 0x00);
    buf.assign(&drive[cmdDrive()].buffer[0],
            &drive[cmdDrive()].buffer[outlen]);

    // Detect Speedlock protection:
    // CRC error on track 00, sector 02, which is 512 bytes long.
    if (((sReg[2] & 0x20) == 0x20) && cmdBuffer[2] == 0x00
            && cmdBuffer[4] == 0x02 && cmdBuffer[5] == 0x02) {
        randomizeSector(buf);
    }

    // If we reach here, it is a normal data read. Dump and continue.
    copy(buf.begin(), buf.end(), &dataBuffer[dataBytes]);
    dataBytes += outlen;

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

void FDC::reset() {

    state = FDCState::FDC_STATE_IDLE;
    stage = FDCAccess::FDC_ACCESS_NONE;
    byte = false;
    cmdIndex = resIndex = dataIndex = 0;
    cmdBytes = resBytes = dataBytes = 0;
    statusReg = SREG_RQM;
}

uint_fast8_t FDC::read() {

    static uint_fast8_t retval = 0x00;

    if (!byte) {
        if ((statusReg & SREG_EXM) == SREG_EXM) {
            retval = dataBuffer[dataIndex++];
        } else {
            retval = resBuffer[resIndex++];
        }
        byte = true;
    }

    return retval;
}

void FDC::write(uint_fast8_t value) {

    if (!byte) {
        if ((statusReg & SREG_EXM) == SREG_EXM) {
            dataBuffer[dataIndex++] = value;
        } else {
            cmdBuffer[cmdIndex++] = value;
        }
        byte = true;
    }
}

void FDC::checkDrive() {

    if (cmdDrive() > 1 || cmdHead() > 0) {
        sReg[0] = cmdBuffer[0];
        sReg[0] |= 0xC4;    // 01..1HUU: AT, NR
        stage = FDCAccess::FDC_ACCESS_UNLOAD;
    }
}

void FDC::motor(bool status) {

    drive[0].motor = status;
    drive[1].motor = status;
}

void FDC::randomizeSector(vector<uint8_t>& buf) {

    // Theoretically, there is a pattern here. However, this seems to work.
    buf.back() |= rand() & 0xFF;
}
// vim: et:sw=4:ts=4