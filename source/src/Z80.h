#pragma once

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

#include <cassert>
#include <cstdint>

#include "CPU.h"
#include "Z80Defs.h"
#include "Z80Decoder.h"
#include "Z80Register.h"

using namespace std;

class Z80 :
    public CPU
{
    public:
        Z80();
        virtual ~Z80();

        void reset();
        void clock();

        void start();
        Z80State finishMemoryCycle();

        // Signals
        uint_fast16_t a;
        uint_fast8_t d;
        uint_fast16_t c;

        // States
        Z80State state;

        // Internals
        Z80Decoder decoder;

        // NMI requested
        bool nmiDelayed;
        bool nmiProcess;
        bool nmiRequest;
};

// vim: et:sw=4:ts=4
