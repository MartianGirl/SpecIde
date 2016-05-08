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

        uint_fast16_t pc;
        uint_fast16_t sp;
        uint_fast16_t ix;
        uint_fast16_t iy;
};

// vim: et:sw=4:ts=4
