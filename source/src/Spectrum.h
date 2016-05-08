#pragma once

/** Spectrum
 *
 * A Spectrum computer
 *
 */

#include "Computer.h"
#include "Memory.h"
#include "Z80.h"
#include "Z80Defs.h"

/** Spectrum.cc
 *
 * The Computer class for a Spectrum 48K
 */

class Spectrum :
    public Computer
{
    public:
        Spectrum();
        virtual ~Spectrum();

        Z80 z80;
        vector<Memory> memory;

        void clock();
        void reset();

};

// vim: et:sw=4:ts=4
