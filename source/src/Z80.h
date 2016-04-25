#pragma once

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

#include "CPU.h"

class Z80 :
    public CPU
{
    public:
        Z80();
        virtual ~Z80();
};

// vim: et:sw=4:ts=4
