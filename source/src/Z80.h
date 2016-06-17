#pragma once

#include <cstdint>

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

using namespace std;

#include "CPU.h"
#include "Z80Defs.h"
#include "Z80Register.h"

class Z80 :
    public CPU
{
    public:
        Z80();
        virtual ~Z80();

        void reset();
        void clock();

        void start();

        // Registers
        size_t registerSet;

        Z80Register af[2];
        Z80Register bc[2];
        Z80Register de[2];
        Z80Register hl[2];
        Z80Register ri;

        Z80Register pc;
        Z80Register sp;
        Z80Register ix;
        Z80Register iy;

        // Signals
        uint_fast16_t a;
        uint_fast8_t d;
        uint_fast16_t c;

        // States
        enum class Z80State
        {
            ST_RESET,

            ST_M1_T0_ADDRWR,
            ST_M1_T1_DATARD,
            ST_M1_T2_RFSH1,
            ST_M1_T3_RFSH2,

            ST_M2_T0,
            ST_M2_T1,
            ST_M2_T2,

            ST_M3_T0,
            ST_M3_T1,
            ST_M3_T2
        };
        Z80State state;
};

// vim: et:sw=4:ts=4
