#pragma once

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

#include <cassert>
#include <cstdint>

#include "Z80Defs.h"
#include "Z80Register.h"

using namespace std;

class Z80
{
    public:
        Z80() : a(0xFFFF), d(0xFF), c(0xFFFF),
        state(Z80State::ST_RESET),
        reg8{&bc.h, &bc.l, &de.h, &de.l, &hl.h, &hl.l, nullptr, &af.h},
        regx8{&bc.h, &bc.l, &de.h, &de.l, &ix.h, &ix.l, nullptr, &af.h},
        regy8{&bc.h, &bc.l, &de.h, &de.l, &iy.h, &iy.l, nullptr, &af.h},
        regp{&bc.w, &de.w, &hl.w, &sp.w},
        regp2{&bc.w, &de.w, &hl.w, &af.w},
        regpx{&bc.w, &de.w, &ix.w, &sp.w},
        regpy{&bc.w, &de.w, &iy.w, &sp.w},
        x(0), y(0), z(0), p(0), q(0),
        prefix(0),
        executionStep(0),
        memRdCycles(0), memWrCycles(0),
        ioRdCycles(0), ioWrCycles(0),
        cpuProcCycles(0) {}

        void reset();
        void clock();

        void start();

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

#include "Z80Unprefixed.h"
#include "Z80CBPrefixed.h"
#include "Z80DDPrefixed.h"
#include "Z80EDPrefixed.h"
#include "Z80FDPrefixed.h"
#include "Z80XXCBPrefixed.h"

#include "Z80Nmi.h"
#include "Z80IntMode2.h"

        // Signals
        uint_fast16_t a;
        uint_fast8_t d;
        uint_fast16_t c;

        // State
        Z80State state;

        // Registers
        Z80Register pc;
        Z80Register ir;
        Z80Register sp;
        Z80Register ix;
        Z80Register iy;

        Z80Register af, af_;
        Z80Register bc, bc_;
        Z80Register de, de_;
        Z80Register hl, hl_;

        uint8_t* reg8[8];
        uint8_t* regx8[8];
        uint8_t* regy8[8];
        uint16_t* regp[4];
        uint16_t* regp2[4];
        uint16_t* regpx[4];
        uint16_t* regpy[4];

        // Internals
        Z80Register iReg;
        Z80Register oReg;
        Z80Register addr;
        Z80Register tmp;
        Z80Register acc;

        uint_fast8_t iff;
        uint_fast8_t im;

        uint_fast8_t x, y, z, p, q;
        uint_fast8_t opcode;
        uint_fast8_t prefix;
        size_t executionStep;
        size_t memRdCycles, memWrCycles;
        size_t ioRdCycles, ioWrCycles;
        size_t cpuProcCycles;
        uint_fast32_t memAddrMode;
};

// vim: et:sw=4:ts=4
