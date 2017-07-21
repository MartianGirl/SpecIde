#pragma once

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

#include <cassert>
#include <cstdint>

#include "Z80Defs.h"
#include "Z80Decoder.h"
#include "Z80Register.h"

using namespace std;

class Z80
{
    public:
        Z80() {}

        void reset();
        void clock();

        void start();
        Z80State finishMachineCycle();
        bool execute();
        bool endOfInstruction();

        // Signals
        uint_fast16_t a;
        uint_fast8_t d;
        uint_fast16_t c;

        // Internals
        Z80Decoder decoder;
};

// vim: et:sw=4:ts=4
