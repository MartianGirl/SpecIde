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
    Screen sc0(2);
    ULA ula;

    ula.ioPortOut = 0x05;
    ula.d = 0x8D;

    sc0.setRgbaInput(&ula.rgba);
    sc0.setVSyncInput(&ula.vSync);
    sc0.setHSyncInput(&ula.hSync);
    sc0.setBlankInput(&ula.blank);

    for (size_t i = 0; i < 139776000; ++i)
    {
        ula.clock();
        sc0.update();
    }
}

BOOST_AUTO_TEST_CASE(image_generation_test)
{
    Screen sc0(2);
    ULA ula;
    Memory m(16, false);

    sc0.setRgbaInput(&ula.rgba);
    sc0.setVSyncInput(&ula.vSync);
    sc0.setHSyncInput(&ula.hSync);
    sc0.setBlankInput(&ula.blank);

    // Prepare some image in the memory.
    for (size_t i = 0; i < 1024; ++i)
    {
        m.memory[i + 0x4000] = 0x0F; // Upper four scans of 1st third chars.
        m.memory[i + 0x4400] = 0xF0; // Lower four scans of 1st third chars.
        m.memory[i + 0x4800] = 0x0F; // Upper four scans of 2nd third chars.
        m.memory[i + 0x4C00] = 0xF0; // Lower four scans of 2nd third chars.
        m.memory[i + 0x5000] = 0x0F; // Upper four scans of 3rd third chars.
        m.memory[i + 0x5400] = 0xF0; // Lower four scans of 3rd third chars.
    }

    for (size_t i = 0; i < 256; ++i)
    {
        m.memory[i + 0x5800] = static_cast<uint8_t>(i); // Attrs, 1st third.
        m.memory[i + 0x5900] = static_cast<uint8_t>(i); // Attrs, 2nd third.
        m.memory[i + 0x5A00] = static_cast<uint8_t>(i); // Attrs, 3rd third.
    }

    ula.ioPortOut = 0x02;
    for (size_t i = 0; i < 139776000; ++i)
    {
        ula.clock();
        if (ula.hiz == false)
        {
            m.a = ula.a | 0x4000;
            m.rd_ = ula.hiz;
            m.as_ = ula.hiz;
            m.clock();
            ula.d = m.d;
        }
        sc0.update();
    }
}

BOOST_AUTO_TEST_CASE(image_load_test)
{
    Screen sc0(2);
    ULA ula;
    Memory m(16, false);

    sc0.setRgbaInput(&ula.rgba);
    sc0.setVSyncInput(&ula.vSync);
    sc0.setHSyncInput(&ula.hSync);
    sc0.setBlankInput(&ula.blank);

    // Prepare some image in the memory.
    size_t pos = 0x4000;
    char c;
    std::ifstream ifs("testfile.scr", std::ifstream::binary);
    while (ifs.get(c))
        m.memory[pos++] = c;

    ula.ioPortOut = 0x02;
    for (size_t i = 0; i < 139776000; ++i)
    {
        if (i % 1000 == 0)
            ula.ioPortOut = (ula.ioPortOut + 0x01) & 0x07;

        ula.clock();
        if (ula.hiz == false)
        {
            m.a = ula.a | 0x4000;
            m.rd_ = ula.hiz;
            m.as_ = ula.hiz;
            m.clock();
            ula.d = m.d;
        }
        sc0.update();
    }
}
// EOF
// vim: et:sw=4:ts=4
