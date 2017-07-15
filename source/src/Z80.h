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
        Z80() : state(Z80State::ST_RESET) {}

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

        // States
        Z80State state;

        // Internals
        Z80Decoder decoder;

        // NMI requested
        bool nmiAccept;
        bool nmiProcess;

        // INT requested
        bool intProcess;
        uint_fast8_t iff;
};

// vim: et:sw=4:ts=4
