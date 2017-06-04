#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Screen test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Screen.h"

BOOST_AUTO_TEST_CASE(constructors_test)
{
    Screen sc0;
    uint32_t rgba = 0xC0C0C0FF;
    uint16_t a = 0x0000;
    uint8_t k[8];
    bool vSync, hSync, blank;
    sc0.setRgbaInput(&rgba);
    sc0.setVSyncInput(&vSync);
    sc0.setHSyncInput(&hSync);
    sc0.setBlankInput(&blank);
    sc0.setKeyboardPort(&a, k);
    sc0.open();
    sc0.update();
    sc0.close();

    Screen sc1(1);
    sc1.setRgbaInput(&rgba);
    sc1.setVSyncInput(&vSync);
    sc1.setHSyncInput(&hSync);
    sc1.setBlankInput(&blank);
    sc1.setKeyboardPort(&a, k);
    sc1.open();
    sc1.update();
    sc1.close();
}

BOOST_AUTO_TEST_CASE(update_test)
{
    Screen sc0(1);
    sc0.open();

    size_t c = 0;
    uint32_t pixel = 0;
    bool vSync, hSync, blank;
    sc0.setRgbaInput(&pixel);
    sc0.setVSyncInput(&vSync);
    sc0.setHSyncInput(&hSync);
    sc0.setBlankInput(&blank);
    for (size_t f = 0; f < 2000; ++f)
    {
        for (size_t y = 0; y < 312; ++y)
        {
            for (size_t x = 0; x < 448; ++x)
            {
                blank = ((x >= 320) && (x <= 415))
                    || ((y >= 248) && (y <= 255));
                hSync = (x >= 336) && (x <= 367);
                vSync = (y >= 248) && (y <= 251);
                c = (c + 1) % 1000;
                uint_fast8_t b = 
                    static_cast<uint_fast8_t>(((c >> 9) * 192) & 0xFF);
                if (((x >= 256) && (x <= 447))
                        || ((y >= 192) && (y <= 312)))
                {
                    pixel = b | (0xFF << 24);
                    sc0.update();
                }
                else
                {
                    pixel = (x & 0xFF) | ((y & 0xFF) << 8) | (0x7F << 16)
                        | (0xFF << 24);
                    sc0.update();
                }
            }
        }
    }

    sc0.close();
}
// EOF
// vim: et:sw=4:ts=4
