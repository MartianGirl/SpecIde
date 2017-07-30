#pragma once

/** Z80RegisterSet
 *
 * Registers and bits in the Z80
 *
 */

#include <cstddef>
#include <cstdint>

#include "Z80Register.h"

using namespace std;

class Z80RegisterSet
{
    public:
        Z80RegisterSet();

        void reset();
};

// vim: et:sw=4:ts=4
