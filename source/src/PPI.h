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

/**
 * PPI
 *
 * Implementation of the Mitsubishi M5L8255AP PPI.
 */
class PPI {

    public:
    /** Byte in port A. */
    uint_fast8_t portA;
    /** Byte in port B. */
    uint_fast8_t portB;
    /** Byte in port C. */
    uint_fast8_t portC;
    /** Control port. */
    uint_fast8_t control;

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
    uint_fast8_t readPortA() const { return inputA ? portA : 0xFF; };

    /**
     * Read port B.
     *
     * @return A byte with the value on port B.
     */
    uint_fast8_t readPortB() const { return inputB ? portB : 0xFF; }

    /**
     * Read port C.
     *
     * @return A byte with the value on port C.
     */
    uint_fast8_t readPortC() const { return 
        (inputHiC ? portC & 0xF0 : 0xF0) | (inputLoC ? portC & 0x0F : 0x0F); }

    /**
     * Write port A from the data bus.
     *
     * @param byte The value for port A.
     */
    void writePortA(uint_fast8_t byte) {

        if (!inputA) {
            portA = byte;
        }
    }

    /**
     * Write port B from the data bus.
     *
     * @param byte The value for port B.
     */
    void writePortB(uint_fast8_t byte) { 

        if (!inputB) {
            portB = byte;
        }
    }

    /**
     * Write Port C from the data bus.
     *
     * @param byte The value for port C.
     */
    void writePortC(uint_fast8_t byte) {
       
        if (!inputLoC) {
            portC = (portC & 0xF0) | (byte & 0x0F);
        }

        if (!inputHiC) {
            portC = (portC & 0x0F) | (byte & 0xF0);
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

        if ((control & 0x80) == 0x80) {

            modeA = (control & 0x60) >> 5;
            modeB = (control & 0x04) >> 2;
            modeC = 0;

            inputA = ((control & 0x10) >> 4) == 1;
            inputB = ((control & 0x02) >> 1) == 1;
            inputLoC = ((control & 0x08) >> 3) == 1;
            inputHiC = (control & 0x01) == 1;
        } else {
        }
    }
};
// vim: et:sw=4:ts=4
