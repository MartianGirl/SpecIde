#pragma once

/** Z80Decoder
 *
 * Z80 Instruction decoder.
 *
 */

#include <cstdint>

#include "Z80Defs.h"
#include "Z80Register.h"

using namespace std;

class Z80Decoder
{
    public:
        Z80Decoder();

        void decode(uint_fast8_t byte);
        void readByte(uint_fast8_t byte);
        void execute();
        void selectRegisterSet(size_t set);
        void reset();

        Z80Register operand;
        uint_fast8_t opcode;
        uint_fast8_t dstRegister;
        uint_fast8_t srcRegister;
        uint_fast16_t wrAddress;

        size_t memRdCycles;
        size_t memWrCycles;

        Z80AddressingMode z80AddrMode;

        // Registers
        Z80Register sp;
        Z80Register ix;
        Z80Register iy;

        size_t registerSet;
        Z80Register af_pair[2];
        Z80Register bc_pair[2];
        Z80Register de_pair[2];
        Z80Register hl_pair[2];
        Z80Register *af;
        Z80Register *bc;
        Z80Register *de;
        Z80Register *hl;
};

// vim: et:sw=4:ts=4
