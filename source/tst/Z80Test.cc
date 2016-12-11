#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <vector>

#include "Memory.h"
#include "Z80.h"
#include "Z80Defs.h"

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    vector<Memory> memory;
    memory.push_back(Memory(14, true));
    memory.push_back(Memory(14, false));
    memory.push_back(Memory(14, false));
    memory.push_back(Memory(14, false));

    // Reset the Z80, clock it. PC should be 0x0000.
    z80.reset();
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    BOOST_CHECK_EQUAL(z80.af[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.af[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF);

    // Clock it once. We've run ST_M1_T1_ADDRWR.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T2_DATARD);
    BOOST_CHECK_EQUAL(z80.af[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.af[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, z80.pc.w);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T2_DATARD.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T3_RFSH1);
    BOOST_CHECK_EQUAL(z80.af[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.af[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, z80.pc.w);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RFSH_));

    // Clock it once. We've run ST_M1_T3_RFSH1
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T3_RFSH2);
    BOOST_CHECK_EQUAL(z80.af[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.af[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.bc[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.de[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.hl[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, z80.pc.w);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~SIGNAL_RFSH);
    
}

// EOF
// vim: et:sw=4:ts=4
