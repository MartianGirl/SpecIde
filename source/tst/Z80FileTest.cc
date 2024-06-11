#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80File test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

#include "Z80File.h"

using namespace std;

uint8_t z80V1Header[] {
                //  Offset  Length  Description
                //  ---------------------------
    0xAA,       //  0       1       A register
    0xFF,       //  1       1       F register
    0xCC, 0xBB, //  2       2       BC register pair (LSB, i.e. C, first)
    0x44, 0x11, //  4       2       HL register pair
    0x00, 0x80, //  6       2       Program counter
    0x00, 0xC0, //  8       2       Stack pointer
    0xF8,       //  10      1       Interrupt register       
    0x99,       //  11      1       Refresh register (Bit 7 is not significant)
    0x3E,       //  12      1       Bit 0   : Bit 7 of the R-register
                //                  Bit 1-3 : Border colour
                //                  Bit 4   : 1=Basic SamRom switched in
                //                  Bit 5   : 1=Block of data is compressed
                //                  Bit 6-7 : No meaning
    0xEE, 0xDD, //  13      2       DE register pair
    0xC0, 0xB0, //  15      2       BC' register pair
    0xE0, 0xD0, //  17      2       DE' register pair
    0x40, 0x10, //  19      2       HL' register pair
    0xA0,       //  21      1       A' register
    0xF0,       //  22      1       F' register
    0xA1, 0xA1, //  23      2       IY register pair (Again LSB first)
    0xA2, 0xA2, //  25      2       IX register pair
    0x27,       //  27      1       Interrupt flipflop, 0=DI, otherwise EI
    0x28,       //  28      1       IFF2 (not particularly important...)
    0x02,       //  29      1       Bit 0-1 : Interrupt mode (0, 1 or 2)
                //                  Bit 2   : 1=Issue 2 emulation
                //                  Bit 3   : 1=Double interrupt frequency
                //                  Bit 4-5 : 1=High video synchronisation
                //                            3=Low video synchronisation
                //                            0,2=Normal
                //                  Bit 6-7 : 0=Cursor/Protek/AGF joystick
                //                            1=Kempston joystick
                //                            2=Sinclair 2 Left joystick (or user
                //                              defined, for version 3 .z80 files)
                //                            3=Sinclair 2 Right joystick
};

BOOST_AUTO_TEST_CASE(v1_header_test)
{
    Z80File file;
    for (uint32_t ii = 0; ii < 0x100; ++ii) {
        file.fileData.push_back(0xED);
        file.fileData.push_back(0xED);
        file.fileData.push_back(0xFF);
        file.fileData.push_back(ii);
    }
    file.fileData.push_back(0x00);
    file.fileData.push_back(0xED);
    file.fileData.push_back(0xED);
    file.fileData.push_back(0x00);

    BOOST_CHECK_EQUAL(file.checkVersion(), true);
    BOOST_CHECK_EQUAL(file.version, 1);

    BOOST_CHECK_EQUAL(file.parseStatus(), true);
}

BOOST_AUTO_TEST_CASE(file_reporter)
{
    Z80File file;
    file.load(boost::unit_test::framework::master_test_suite().argv[1]);
    file.checkVersion();
    cout << "File version: " << static_cast<uint32_t>(file.version) << endl;
    file.parseStatus();
    cout << hex;
    cout << "AF: " << setfill('0') << setw(4) << file.status.af
        << "   BC: " << setfill('0') << setw(4) << file.status.bc
        << "   DE: " << setfill('0') << setw(4) << file.status.de
        << "   HL: " << setfill('0') << setw(4) << file.status.hl << endl;
    cout << "AF': " << setfill('0') << setw(4) << file.status.af_
        << "  BC': " << setfill('0') << setw(4) << file.status.bc_
        << "  DE': " << setfill('0') << setw(4) << file.status.de_
        << "  HL': " << setfill('0') << setw(4) << file.status.hl_ << endl;
}
// EOF
// vim: et:sw=4:ts=4

