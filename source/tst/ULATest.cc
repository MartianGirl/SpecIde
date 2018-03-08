#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ULA test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "Memory.h"
#include "ULA.h"
#include "Screen.h"

#include <fstream>

BOOST_AUTO_TEST_CASE(constructors_test)
{
    ULA ula;
}

BOOST_AUTO_TEST_CASE(display_position_test)
{
    Screen sc0(1, false);

    sc0.spectrum.ula.borderAttr = 0x05;
    sc0.spectrum.ula.d = 0x8D;

    for (size_t i = 0; i < 139776000; ++i)
    {
        sc0.spectrum.ula.clock();
        sc0.spectrum.ula.d = sc0.spectrum.ram[5].read(sc0.spectrum.ula.a);
        sc0.update();
    }
}

BOOST_AUTO_TEST_CASE(image_generation_test)
{
    Screen sc0(2, false);

    // Prepare some image in the memory.
    for (size_t i = 0; i < 1024; ++i)
    {
        sc0.spectrum.map[1]->memory[i + 0x0000] = 0x0F; // Upper four scans of 1st third chars.
        sc0.spectrum.map[1]->memory[i + 0x0400] = 0xF0; // Lower four scans of 1st third chars.
        sc0.spectrum.map[1]->memory[i + 0x0800] = 0x0F; // Upper four scans of 2nd third chars.
        sc0.spectrum.map[1]->memory[i + 0x0C00] = 0xF0; // Lower four scans of 2nd third chars.
        sc0.spectrum.map[1]->memory[i + 0x1000] = 0x0F; // Upper four scans of 3rd third chars.
        sc0.spectrum.map[1]->memory[i + 0x1400] = 0xF0; // Lower four scans of 3rd third chars.
    }

    for (size_t i = 0; i < 256; ++i)
    {
        sc0.spectrum.map[1]->memory[i + 0x1800] = static_cast<uint8_t>(i); // Attrs, 1st third.
        sc0.spectrum.map[1]->memory[i + 0x1900] = static_cast<uint8_t>(i); // Attrs, 2nd third.
        sc0.spectrum.map[1]->memory[i + 0x1A00] = static_cast<uint8_t>(i); // Attrs, 3rd third.
    }

    sc0.spectrum.ula.borderAttr = 0x02;
    for (size_t i = 0; i < 139776000; ++i)
    {
        sc0.spectrum.ula.clock();
        sc0.spectrum.ula.d = sc0.spectrum.ram[5].read(sc0.spectrum.ula.a);
        sc0.update();
    }
}

BOOST_AUTO_TEST_CASE(image_load_test)
{
    Screen sc0(2, true);

    // Prepare some image in the memory.
    size_t pos = 0;
    char c;
    std::ifstream ifs("testfile.scr", std::ifstream::binary);
    while (ifs.get(c))
        sc0.spectrum.map[1]->memory[pos++] = c;

    sc0.spectrum.ula.borderAttr = 0x02;
    for (size_t i = 0; i < 139776000; ++i)
    {
        if (i % 1000 == 0)
            sc0.spectrum.ula.borderAttr = (sc0.spectrum.ula.borderAttr + 0x01) & 0x07;

        sc0.spectrum.ula.clock();
        sc0.spectrum.ula.d = sc0.spectrum.ram[5].read(sc0.spectrum.ula.a);
        sc0.update();
    }
}
// EOF
// vim: et:sw=4:ts=4
