#pragma once

/** Z80Decoder
 *
 * Z80 Instruction decoder.
 *
 */

#include <cassert>
#include <cstdint>

#include "Z80Defs.h"
#include "Z80Register.h"
#include "Z80RegisterSet.h"
#include "Z80Unprefixed.h"
#include "Z80CBPrefixed.h"
#include "Z80DDPrefixed.h"
#include "Z80EDPrefixed.h"
#include "Z80FDPrefixed.h"
#include "Z80XXCBPrefixed.h"

#include "Z80Nmi.h"
#include "Z80IntMode2.h"

using namespace std;

class Z80Decoder
{
    public:
        Z80Decoder();

        void decode(uint_fast8_t byte);
        void startInstruction();

        uint_fast16_t getAddress();

        void readMem(uint_fast8_t byte);
        uint_fast8_t writeMem();

        void readIo(uint_fast8_t byte);
        uint_fast8_t writeIo();

        void cpuProcCycle();

        bool execute();
        bool executeNmi();
        bool executeInt();

        void reset();

        Z80RegisterSet regs;

        Z80Unprefixed unprefixed;
        Z80CBPrefixed CBPrefixed;
        Z80DDPrefixed DDPrefixed;
        Z80EDPrefixed EDPrefixed;
        Z80FDPrefixed FDPrefixed;
        Z80XXCBPrefixed XXCBPrefixed;

        Z80Nmi z80Nmi;
        Z80IntMode2 z80IntMode2;
};

// vim: et:sw=4:ts=4
