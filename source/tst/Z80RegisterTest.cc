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

    reg0.h = 0x67;
    reg0.l = 0x42;
    BOOST_CHECK_EQUAL(reg0.getw(), 0x6742);
    
    reg0.setw(0x1234);
    BOOST_CHECK_EQUAL(reg0.getw(), 0x1234);
    BOOST_CHECK(reg0.h == 0x12);
    BOOST_CHECK(reg0.l == 0x34);
}

// EOF
// vim: et:sw=4:ts=4
