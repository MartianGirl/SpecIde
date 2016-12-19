#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Memory test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <vector>

#include "Memory.h"

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    // Initialize a 8-bit 16kB RAM.
    Memory mem0(14, false);
    BOOST_CHECK_EQUAL(mem0.size, static_cast<uint_fast16_t>(0x4000));

    // Memory should not be modified until clock happens.
    mem0.a = 0x1234;
    mem0.d = 0xAA;
    mem0.wr_ = true;
    mem0.as_ = true;
    BOOST_CHECK_EQUAL(mem0.memory[0x1234], 0x00);

    // Clock edges make memory work.
    mem0.as_ = false; mem0.wr_ = true; mem0.clock();
    mem0.as_ = false; mem0.wr_ = false; mem0.clock();
    BOOST_CHECK_EQUAL(mem0.memory[0x1234], 0xAA);

    // Read test.
    mem0.d = 0x12;
    mem0.a = 0x1234;
    mem0.addr = 0x0000;
    mem0.as_ = false; mem0.rd_ = true; mem0.clock();
    mem0.as_ = false; mem0.rd_ = false; mem0.clock();
    BOOST_CHECK_EQUAL(mem0.memory[0x1234], 0xAA);
    BOOST_CHECK_EQUAL(mem0.d, 0xAA);
}

// EOF
// vim: et:sw=4:ts=4
