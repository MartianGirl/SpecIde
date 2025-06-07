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
    uint_fast8_t control = 0x9B;
    /** Byte in data bus. */
    uint_fast8_t bus = 0xFF;

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

    /** Port C mask for writes. */
    uint_fast8_t maskCWr = 0x00;
    /** Port C mask for reads. */
    uint_fast8_t maskCRd = 0x00;
    /** Port C mask for bit set/clear mode. */
    uint_fast8_t maskCPin = 0x00;

    /** Port A direction. */
    bool inputA = true;
    /** Port B direction. */
    bool inputB = true;
    /** Port C direction, low nybble. */
    bool inputLoC = true;
    /** Port C direction, high nybble. */
    bool inputHiC = true;

    /**
     * Read port A.
     *
     * @return A byte with the value on port A.
     */
    uint_fast8_t readPortA() {

        uint_fast8_t value = regA;
        if (modeA == 2) {
        } else if (modeA == 1) {
        } else {
            // Mode 0: Read value from port A.
            value = inputA ? portA : regA;
        }
        bus = value;
        return value;
    };

    /**
     * Read port B.
     *
     * @return A byte with the value on port B.
     */
    uint_fast8_t readPortB() {

        uint_fast8_t value = regB;
        if (modeB) {
        } else {
            // Mode 0: Read value from port B.
            value = inputB ? portB : regB;
        }
        bus = value;
        return value;
    }

    /**
     * Read port C.
     *
     * @return A byte with the value on port C.
     */
    uint_fast8_t readPortC() {

        uint_fast8_t value = 0;

        // On input, reading portC returns the value of the pins not used
        // as control pins for ports A & B.
        // On output, reading portC returns the latched value.
        if (inputLoC) {
            value |= portC & maskCRd & 0x0F;
        } else {
            value |= regC & 0x0F;
        }

        if (inputHiC) {
            value |= portC & maskCRd & 0xF0;
        } else {
            value |= regC & 0xF0;
        }

        bus = value;

        return value;
    }

    /**
     * Write port A from the data bus.
     *
     * Writes a byte to port A. The value is output if the port is outbound,
     * but it is always latched.
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
     * Writes a byte to port B. The value is output if the port is outbound,
     * but it is always latched.
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
     * Writes a byte to port C. The value is output if the port is outbound,
     * but it is always latched, except for the pins set as control pins or
     * in a group with non-zero mode.
     *
     * @param byte The value for port C.
     */
    void writePortC(uint_fast8_t byte) {

        if (!inputLoC) {
            portC &= 0xF0;
            portC |= ((byte & maskCWr) | (maskCPin & ~maskCWr)) & 0x0F;
        }

        if (!inputHiC) {
            portC &= 0x0F;
            portC |= ((byte & maskCWr) | (maskCPin & ~maskCWr)) & 0xF0;
        }

        // The input path is not group-limited.
        regC = (maskCPin & ~maskCWr) | (byte & maskCWr);
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

        // b8: 0 = bit control. 1 = normal mode.
        if (control & 0x80) {
            modeA = (control & 0x60) >> 5;  // b6 b5: Port A mode.
            modeB = (control & 0x04) >> 2;  // b2: Port B mode.

            inputA = (control & 0x10);      // b4: Port A direction.
            inputB = (control & 0x02);      // b1: Port B direction.
            inputHiC = (control & 0x08);    // b3: Port C high nybble direction.
            inputLoC = (control & 0x01);    // b0: Port C low nybble direction.

            // All ports are reset when the control word is updated.
            regA = 0; portA = inputA ? 0xFF : 0x00;
            regB = 0; portB = inputB ? 0xFF : 0x00;
            regC = 0; portC = 0;

            maskCWr = 0;
            maskCRd = 0;
            maskCPin = 0;
            // Mode A = 0 keeps bits PC7-PC3 for I/O.
            // Mode A = 1 (output) grabs bits PC 7,6,3 for control.
            // Mode A = 1 (input) grabs bits PC 5,4,3 for control.
            // Mode A = 2 grabs bits PC7-PC3 for control.
            // However, only output pins in a Mode 0 group will be affected.
            switch (modeA) {
                case 1:
                    maskCRd |= inputA ? 0xC0 : 0x30;
                    maskCWr |= 0x00;    // Pins PC7-PC4 are in group A, mode 1.
                    break;
                case 2: // fall-through
                case 3:
                    maskCRd |= 0x00;
                    maskCWr |= 0x00;
                    break;
                default:
                    maskCWr |= 0xF8;
                    maskCRd |= 0xF8;
                    break;
            }

            // Mode B = 0 keeps bits PC2-PC0 for I/O.
            // Mode B = 1 grabs bits PC2-PC0 for control.
            maskCRd |= modeB ? 0x00 : 0x07;
            maskCWr |= modeB ? 0x00 : 0x07;
        } else {
            uint_fast8_t mask = 1 << ((control & 0xe) >> 1);
            maskCPin &= ~maskCRd;
            // Bit control mode also updates the latch and port.
            // Input bits (including input control pins) are not affected.
            if (control & 0x1) {
                maskCPin |= mask;
                if (!inputLoC) { portC |= mask & 0x0F; regC |= mask & 0x0F; }
                if (!inputHiC) { portC |= mask & 0xF0; regC |= mask & 0xF0; }
            } else {
                maskCPin &= ~mask;
                if (!inputLoC) { portC &= ~(mask & 0x0F); regC &= ~(mask & 0x0F); }
                if (!inputHiC) { portC &= ~(mask & 0xF0); regC &= ~(mask & 0xF0); }
            }
        }
    }
};
// vim: et:sw=4:ts=4
