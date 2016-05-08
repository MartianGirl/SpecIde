#pragma once

#include <cstdint>

/** Z80Register
 *
 * Each of the 16-bit groups of two 8-bit registers in the Z80.
 *
 */

using namespace std;

class Z80Register
{
    public:
        Z80Register();
        virtual ~Z80Register();

        void setw(uint_fast16_t value);
        uint_fast16_t getw(void);

        uint_fast8_t h;
        uint_fast8_t l;

};

// vim: et:sw=4:ts=4
