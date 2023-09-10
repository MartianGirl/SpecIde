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

#include <algorithm>
#include <iomanip>

#include "FD1793.h"

using namespace std;

void FD1793::clock() {

    // The FD1793 will accept eleven commands. Command words should only be
    // loaded in the Command Register when the Busy status bit is off. The
    // only exception is the Force Interrupt command.
    // Whenever a command is being executed, the Busy status bit is set.
    // When a command is completed, an interrupt is generated and the Busy
    // status bit is reset.
    // The Status Register indicates whether the completed command encountered
    // an error or was fault free.

    if (commandNew) {
        checkCommand();
    }
    // First check for FORCE INTERRUPT command.
    switch (state) {
        case FD1793State::FD1793_STATE_IDLE:
            break;
    }
}

void FD1793::reset() {

    state = FD1793_STATE_IDLE;
}

uint_fast8_t FD1793::read(uint_fast8_t address) {

    static uint_fast8_t retval = 0x00;

    switch (address) {
        case 0x00:  // Status register
            retval = statusReg;
            break;
        case 0x01:  // Track register
            retval = trackReg;
            break;
        case 0x02:  // Sector register
            retval = sectorReg;
            break;
        case 0x03:  // Data register
            retval = dataReg;
            break;
        case 0x07:  // System register (Not an FD1793 register, though)
            retval = systemReg;
            break;
    }

    return retval;
}

void FD1793::write(uint_fast8_t address, uint_fast8_t value) {

    switch (address) {
        case 0x00:  // Command register
            commandReg = value;
            commandNew = true;
            break;
        case 0x01:  // Track register
            trackReg = value;
            break;
        case 0x02:  // Sector register
            sectorReg = value;
            break;
        case 0x03:  // Data register
            dataReg = value;
            break;
        case 0x07:  // System register
            systemReg = value;
            break;
    }
}

void FD1793::checkCommand() {

    if ((commandReg & 0xF0) == 0xD0) {
        // Type IV command: Force Interrupt.
        command = FD1793Command::INTERRUPT;
        prepareType4();
    } else if (!(statusReg & STATUS_BUSY)) {
        switch (commandReg & 0xF0) {
            // Type I commands: Restore, Seek, Step, Step In, Step Out.
            case 0x00:
                command = FD1793Command::RESTORE;
                prepareType1();
                break;
            case 0x10:
                command = FD1793Command::SEEK;
                prepareType1();
                break;
            case 0x20:
            case 0x30:
                command = FD1793Command::STEP;
                prepareType1();
                break;
            case 0x40:
            case 0x50:
                command = FD1793Command::STEP_IN;
                prepareType1();
                break;
            case 0x60:
            case 0x70:
                command = FD1793Command::STEP_OUT;
                prepareType1();
                break;
            // Type II commands: Read Sector, Write Sector.
            case 0x80:
            case 0x90:
                command = FD1793Command::READ_SECTOR;
                prepareType2();
                break;
            case 0xA0:
            case 0xB0:
                command = FD1793Command::WRITE_SECTOR;
                prepareType2();
                break;
            // Type III commands: Read Address, Read Track, Write Track.
            case 0xC0:
                command = FD1793Command::READ_ADDRESS;
                prepareType3();
                break;
            case 0xE0:
                command = FD1793Command::READ_TRACK;
                prepareType3();
                break;
            case 0xF0:
                command = FD1793Command::WRITE_TRACK;
                prepareType3();
                break;
            // Type IV commands are covered above.
            default:
                command = FD1793Command::INVALID;
                break;
        }
    }
}

void prepareType1() {

    updateFlag = commandReg & 0x10;
    headLoadFlag = commandReg & 0x08;
    verifyFlag = commandReg & 0x04;
    stepRate = commandReg & 0x03;
}

void prepareType2() {

    multipleRecordFlag = commandReg & 0x10;
    sideSelectFlag = commandReg & 0x08;
    sideCompareFlag = commandReg & 0x02;

    prepareType3();
}

void prepareType3() {

    delayFlag = commandReg & 0x04;
}

void prepareType4() {

    notReadyToReady = commandReg & 0x01;
    readyToNotReady = commandReg & 0x02;
    indexPulseFlag = commandReg & 0x04;
    immediateInterrupt = commandReg & 0x08;
}
// vim: et:sw=4:ts=4
