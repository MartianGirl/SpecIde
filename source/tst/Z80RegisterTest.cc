#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80Register test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Z80Register.h"

#include <cstdint>

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Z80Register reg0;

    // Set the word value from its bytes.
    reg0.h = 0x67;
    reg0.l = 0x42;
    BOOST_CHECK_EQUAL(reg0.reg, 0x6742);
    
    // Set the word value, then check its bytes.
    reg0.w = 0x9876;
    BOOST_CHECK_EQUAL(reg0.reg, 0x9876);
    BOOST_CHECK_EQUAL(reg0.h, 0x98);
    BOOST_CHECK_EQUAL(reg0.l, 0x76);

    // Setter and getter.
    reg0.setw(0x1234);
    BOOST_CHECK_EQUAL(reg0.getw(), 0x1234);
    BOOST_CHECK_EQUAL(reg0.h, 0x12);
    BOOST_CHECK_EQUAL(reg0.l, 0x34);

    // Incrementing the word.
    reg0.w = 0x3FFF;
    BOOST_CHECK_EQUAL(reg0.h, 0x3F);
    BOOST_CHECK_EQUAL(reg0.l, 0xFF);
    reg0.w++;
    BOOST_CHECK_EQUAL(reg0.h, 0x40);
    BOOST_CHECK_EQUAL(reg0.l, 0x00);

    // Incrementing the bytes (should not carry)
    reg0.w = 0x3FFF;
    BOOST_CHECK_EQUAL(reg0.h, 0x3F);
    BOOST_CHECK_EQUAL(reg0.l, 0xFF);
    reg0.l++;
    BOOST_CHECK_EQUAL(reg0.h, 0x3F);
    BOOST_CHECK_EQUAL(reg0.l, 0x00);
}

// EOF
// vim: et:sw=4:ts=4
