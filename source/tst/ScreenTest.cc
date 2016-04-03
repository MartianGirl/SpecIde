#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Screen test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Screen.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Screen sc0;
    sc0.open();
    sc0.update();
    sc0.close();

    Screen sc1(2);
    sc1.open();
    sc1.update();
    sc1.close();
}

// EOF
// vim: et:sw=4:ts=4
