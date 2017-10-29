#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Screen test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Screen.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Screen sc0;

    Screen sc1(1);
}

BOOST_AUTO_TEST_CASE(update_test)
{
    Screen sc0(1);

    size_t c = 0;
    for (size_t f = 0; f < 2000; ++f)
    {
        for (size_t y = 0; y < 312; ++y)
        {
            sc0.spectrum.ula.retrace = ((y >= 248) && (y <= 251));
            for (size_t x = 0; x < 448; ++x)
            {
                if (x == 320)
                {
                    sc0.spectrum.ula.blanking = true;
                }
                else if (x == 415)
                {
                    sc0.spectrum.ula.blanking = sc0.spectrum.ula.retrace;
                }
                sc0.spectrum.ula.vSyncEdge = (y == 252) && (x == 320);
                sc0.spectrum.ula.hSyncEdge = (x == 368);
                c = (c + 1) % 1000;
                uint_fast8_t b = 
                    static_cast<uint_fast8_t>(((c >> 9) * 192) & 0xFF);
                if (((x >= 256) && (x <= 447))
                        || ((y >= 192) && (y <= 312)))
                {
                    sc0.spectrum.ula.rgba = b | (0xFF << 24);
                }
                else
                {
                    sc0.spectrum.ula.rgba = (x & 0xFF) | ((y & 0xFF) << 8) | (0x7F << 16)
                        | (0xFF << 24);
                }
                sc0.update();
            }
        }
    }
}
// EOF
// vim: et:sw=4:ts=4
