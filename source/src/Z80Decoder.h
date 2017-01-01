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
#include "Z80DDPrefixed.h"
#include "Z80EDPrefixed.h"
#include "Z80FDPrefixed.h"

using namespace std;

class Z80Decoder
{
    public:
        Z80Decoder();

        void decode(uint_fast8_t byte);
        void readByte(uint_fast8_t byte);
        uint_fast16_t getAddress();
        void writeByte();
        bool execute();
        void reset();

        Z80RegisterSet regs;

        Z80Unprefixed unprefixed;
        Z80DDPrefixed DDPrefixed;
        Z80EDPrefixed EDPrefixed;
        Z80FDPrefixed FDPrefixed;
};

// vim: et:sw=4:ts=4
