/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2021.
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

#include <iomanip>
#include <iostream>

/**
 * PPI
 *
 * Implementation of the Mitsubishi M5L8255AP PPI.
 */
class PPI {

    public:
    /** Byte in port A. */
    uint_fast8_t portA = 0;
    /** Byte in port B. */
    uint_fast8_t portB = 0;
    /** Byte in port C. */
    uint_fast8_t portC = 0;
    /** Control port. */
    uint_fast8_t control = 0;

    /** Port A register. */
    uint_fast8_t regA = 0;
    /** Port A register. */
    uint_fast8_t regB = 0;
    /** Port A register. */
    uint_fast8_t regC = 0;

    /** Port A mode. */
    uint_fast8_t modeA = 0;
    /** Port B mode. */
    uint_fast8_t modeB = 0;
    /** Port C mode. */
    uint_fast8_t modeC = 0;

    /** Port A direction. */
    bool inputA = false;
    /** Port B direction. */
    bool inputB = false;
    /** Port C direction, low nybble. */
    bool inputLoC = false;
    /** Port C direction, high nybble. */
    bool inputHiC = false;

    /**
     * Read port A.
     *
     * @return A byte with the value on port A.
     */
    uint_fast8_t readPortA() {

        if (modeA == 2) {
        } else if (modeA == 1) {
            // Mode 1 as input: STBA is always active.
            if (inputA) {
                regC &= 0xD7; portC = regC;
                regA = portA;
            }
        } else {
            // Mode 0: Read value from port A.
            if (inputA) regA = portA;
        }
        return regA;
    };

    /**
     * Read port B.
     *
     * @return A byte with the value on port B.
     */
    uint_fast8_t readPortB() {

        if (modeB) {
            // Mode 1: Clear IBFB and INTEB
            regC &= 0xFC; portC = regC;
        } else {
            // Mode 0: Read value from port B.
            if (inputB) regB = portB;
        }
        return regB;
    }

    /**
     * Read port C.
     *
     * @return A byte with the value on port C.
     */
    uint_fast8_t readPortC() {

        if (inputLoC) { regC &= 0xF0; regC |= portC & 0x0F; }
        if (inputHiC) { regC &= 0x0F; regC |= portC & 0xF0; }

        return regC;
    }

    /**
     * Write port A from the data bus.
     *
     * @param byte The value for port A.
     */
    void writePortA(uint_fast8_t byte) {

        regA = byte;
        if (!inputA) { portA = regA; }
    }

    /**
     * Write port B from the data bus.
     *
     * @param byte The value for port B.
     */
    void writePortB(uint_fast8_t byte) { 

        regB = byte;
        if (!inputB) { portB = regB; }
    }

    /**
     * Write Port C from the data bus.
     *
     * @param byte The value for port C.
     */
    void writePortC(uint_fast8_t byte) {

        regC = byte;
        if (modeA == 0 && modeB == 0) {
            if (!inputLoC) { portC &= 0xF0; portC |= regC & 0x0F; }
            if (!inputHiC) { portC &= 0x0F; portC |= regC & 0xF0; }
        } else {
            if (modeA == 1 && inputA) {
                portC &= 0xD7;
                if (!inputHiC) { portC &= 0x3F; portC |= regC & 0xC0; }
            }

            if (modeB == 1 && inputB) {
                portC |= 0x03;
                if (!inputHiC) { portC &= 0x3F; portC |= regC & 0xC0; }
            }
        }
    }

    /**
     * Write control port.
     *
     * The PPI configuration is changed on write.
     *
     * @param byte The value for the control port.
     */
    void writeControlPort(uint_fast8_t byte) {

        control = byte;

        // b8: 1 = bit control. 0 = normal mode.
        if (control & 0x80) {
            modeA = (control & 0x60) >> 5;  // b6 b5: Port A mode.
            modeB = (control & 0x04) >> 2;  // b2. Port B mode.

            inputA = (control & 0x10);      // b4: Port A direction.
            inputB = (control & 0x02);      // b1. Port B direction.
            inputHiC = (control & 0x08);    // b3: Port C high nybble direction.
            inputLoC = (control & 0x01);    // b0. Port C low nybble direction.

            // All ports are reset when the control word is updated.
            regA = 0;
            regB = 0;
            regC = 0;
            if (!inputA) { portA = regA; }
            if (!inputB) { portB = regB; }
            if (!inputLoC) { portC &= 0xF0; portC |= regC & 0x0F; }
            if (!inputHiC) { portC &= 0x0F; portC |= regC & 0xF0; }
        } else {
            uint8_t mask = 1 << ((control & 0xe) >> 1);
            if (control & 0x1) {
                portC |= mask;
            } else {
                portC &= ~mask;
            }
            regC = portC;
        }
    }
};
// vim: et:sw=4:ts=4
