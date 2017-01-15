#pragma once

/** Z80RegisterSet
 *
 * Registers and bits in the Z80
 *
 */

#include <cstdint>

#include "Z80Register.h"

using namespace std;

class Z80RegisterSet
{
    public:
        Z80RegisterSet();

        void reset();

        // Registers
        Z80Register pc;
        Z80Register ir;
        Z80Register sp;
        Z80Register ix;
        Z80Register iy;

        Z80Register af, af_;
        Z80Register bc, bc_;
        Z80Register de, de_;
        Z80Register hl, hl_;

        uint8_t* reg8[8];
        uint8_t* regx8[8];
        uint8_t* regy8[8];
        uint16_t* regp[4];
        uint16_t* regp2[4];

        Z80Register operand;
        Z80Register address;
        Z80Register outWord;
        Z80Register offset;

        uint_fast8_t iff;
        uint_fast8_t im;

        uint_fast8_t x, y, z, p, q;
        uint_fast8_t prefix;
        size_t executionStep;
        size_t memRdCycles, memWrCycles;
        uint_fast32_t memAddrMode;
};

// vim: et:sw=4:ts=4
