#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Screen test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Screen.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Screen sc0;
    sc0.open();
    sc0.update(0, 0, 0);
    sc0.close();

    Screen sc1(2);
    sc1.open();
    sc1.update(0, 0, 0);
    sc1.close();
}

BOOST_AUTO_TEST_CASE(update_test)
{
    Screen sc0(1);
    sc0.open();

    size_t c = 0;
    for (size_t f = 0; f < 500; ++f)
    {
        for (size_t y = 0; y < 312; ++y)
        {
            for (size_t x = 0; x < 448; ++x)
            {
                sc0.blank = (x >= 320) && (x <= 415);
                sc0.hSync = (x >= 336) && (x <= 367);
                sc0.vSync = (y >= 248) && (y <= 251) && sc0.hSync;
                c = (c + 1) % 1000;
                uint_fast8_t b = (c >> 9) * 192;
                if (((x >= 256) && (x <= 447))
                        || ((y >= 192) && (y <= 312)))
                    sc0.update(b, 0, 0);
                else
                    sc0.update((x & 0xFF), (y & 0xFF), 0x7F);
            }
        }
    }

    sc0.close();
}



// EOF
// vim: et:sw=4:ts=4
