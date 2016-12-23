#pragma once

/** Z80Decoder
 *
 * Z80 Instruction decoder.
 *
 */

#include <cstdint>

#include "Z80Defs.h"
#include "Z80Register.h"
#include "Z80RegisterSet.h"

using namespace std;

class Z80Decoder
{
    public:
        Z80Decoder();

        void decode(uint_fast8_t byte);
        void readByte(uint_fast8_t byte);
        void execute();
        void reset();

        Z80Register operand;
        uint_fast8_t srcRegister;
        uint_fast16_t wrAddress;

        size_t memRdCycles;
        size_t memWrCycles;

        Z80AddressingMode z80AddrMode;

        uint_fast8_t x, y, z, p, q;
        uint_fast8_t prefix;

        Z80RegisterSet regs;
};

// vim: et:sw=4:ts=4
