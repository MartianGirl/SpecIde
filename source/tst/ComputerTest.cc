#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Computer test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Computer.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Computer c0;
}

// EOF
// vim: et:sw=4:ts=4
