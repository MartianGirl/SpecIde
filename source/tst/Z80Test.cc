#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Z80.h"

#include <cstdint>

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Z80 z0();
}

// EOF
// vim: et:sw=4:ts=4
