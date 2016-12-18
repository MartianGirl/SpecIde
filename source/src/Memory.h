#pragma once

/** Memory
 *
 * Memory bank.
 *
 * This is intended to be a TMS4116-like memory. The pinout is:
 * 1. VBB (-5V)                     16. VSS (GND)
 * 2. D                             15. CAS# (Column Address Strobe)
 * 3. W#                            14. Q
 * 4. RAS# (Row Address Strobe)     13. A6
 * 5. A0                            12. A3
 * 6. A2                            11. A4
 * 7. A1                            10. A5
 * 8. VDD (12V)                      9. VCC (5V)
 * 
 * The point here is, the memories are asynchronous. The addresses are
 * latched by the CAS#/RAS# signals.
 *
 */

#include <cstdint>
#include <vector>

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
