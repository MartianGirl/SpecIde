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
#include "Z80Unprefixed.h"
#include "Z80EDPrefixed.h"

using namespace std;

class Z80Decoder
{
    public:
        Z80Decoder();

        void decode(uint_fast8_t byte);
        void readByte(uint_fast8_t byte);
        void writeByte(uint_fast16_t addr);
        void waitState();
        void execute();
        void reset();

        Z80Register operand;
        Z80Register address;
        Z80Register offset;

        Z80RegisterSet regs;

        Z80Unprefixed unprefixed;
        Z80EDPrefixed EDPrefixed;
};

// vim: et:sw=4:ts=4
