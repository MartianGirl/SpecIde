#pragma once

#include "Computer.h"
#include "Memory.h"
#include "Z80.h"

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
};

// vim: et:sw=4:ts=4
