#pragma once

#include <cstdint>

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

using namespace std;

#include "CPU.h"
#include "Z80Register.h"

class Z80 :
    public CPU
{
    public:
        Z80();
        virtual ~Z80();

        void reset();
        void clock();

        // Registers
        Z80Register af[2];
        Z80Register bc[2];
        Z80Register de[2];
        Z80Register hl[2];
        Z80Register ri;

        size_t registerSet;

        Z80Register pc;
        Z80Register sp;
        Z80Register ix;
        Z80Register iy;

        // Signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool wr_;
        bool rd_;
        bool mreq_;
        bool ioreq_;
        bool m1_;
        bool rfsh_;

        bool halt_;
        bool wait_;
        bool int_;
        bool nmi_;

        bool busreq_;
        bool busak_;
        bool reset_;

        // States
        enum Z80State
        {
            ST_RESET,

            ST_M1_T0_ADDROUT,
            ST_M1_T1_DATAIN,
            ST_M1_T2_RFSH1,
            ST_M1_T3_RFSH2,
            ST_M1_TW_BUSWAIT,

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
