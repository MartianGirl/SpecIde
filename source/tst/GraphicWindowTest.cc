#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphicWindow test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "GraphicWindow.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    GraphicWindow gw0(640, 400);

    GraphicWindow gw1(640, 400, "SpecIde 2X");
}

// EOF
// vim: et:sw=4:ts=4
