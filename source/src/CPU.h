#pragma once

#include <cstdint>

/** CPU
 *
 * Base class for all CPUs.
 */

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
