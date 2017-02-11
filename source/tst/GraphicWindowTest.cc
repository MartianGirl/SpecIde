//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphicWindow test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include "GraphicWindow.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    GraphicWindow gw0;
    gw0.open();
    gw0.update();
    gw0.close();

    GraphicWindow gw1(640, 400, "SpecIde 2X");
    gw1.open();
    gw1.update();
    gw1.close();
}

// EOF
// vim: et:sw=4:ts=4
