#pragma once

/** CPU
 *
 * Base class for all CPUs.
 */

#include <cstdint>

using namespace std;

class CPU
{
    public:
        CPU();
        virtual ~CPU();

        virtual void clock();
        virtual void reset();
};

// vim: et:sw=4:ts=4
