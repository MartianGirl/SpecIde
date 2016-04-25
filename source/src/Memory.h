#pragma once

#include <cstdint>
#include <vector>

/** Memory
 *
 * Memory bank.
 *
 */

using namespace std;

class Memory
{
    public:
        Memory(size_t addrWidth, bool readOnly);
        virtual ~Memory();

        /** Issue a clock tick.
         */
        void clock();

        vector<uint_fast8_t> memory;

        bool readOnly;
        bool write;

        uint_fast16_t addr;
        uint_fast8_t data;

        uint_fast16_t size;
};

// vim: et:sw=4:ts=4
