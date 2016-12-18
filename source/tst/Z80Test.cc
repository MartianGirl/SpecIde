#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
#include <boost/test/unit_test.hpp>
//#include <boost/test/include/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <vector>

#include "Memory.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "Z80Decoder.h"

using namespace std;

BOOST_AUTO_TEST_CASE(state_machine_test)
{
    // Create a Z80 and some memory.
    Z80 z80;

    // Reset the Z80, clock it. PC should be 0x0000.
    z80.reset();
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF);

    // Clock it once. We've run ST_M1_T1_ADDRWR.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T2_DATARD);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T2_DATARD.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T3_RFSH1);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T3_RFSH1
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T4_RFSH2);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RFSH_));
    
    // Clock it once. We've run ST_M1_T4_RFSH2
    z80.clock();
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[0].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[1].w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.ix.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.iy.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.sp.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~SIGNAL_RFSH_);
}

BOOST_AUTO_TEST_CASE(instruction_ld_r_n_test)
{
    Z80 z80;
    // LD r, n tests
    // LD A, n
    z80.reset();
    z80.clock();    // ST_RESET        -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    z80.clock();    // ST_M1_T1_ADDRWR -> ST_M1_T2_DATARD 
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0001);
    z80.clock();    // ST_M1_T2_DATARD -> ST_M1_T3_RFSH1
    z80.d = 0x3E;   // LD A, n
    z80.clock();    // ST_M1_T3_RFSH1  -> ST_M1_T4_RFSH2
    BOOST_CHECK_EQUAL(z80.decoder.opcode, 0x3E);
    z80.clock();    // ST_M1_T4_RFSH2  -> ST_M2_T1_ADDRWR
    BOOST_CHECK(z80.state == Z80State::ST_M2_T1_ADDRWR);
    BOOST_CHECK_EQUAL(z80.decoder.opcode, 0x3E);
    BOOST_CHECK_EQUAL(z80.decoder.dstRegister, 0x07);
    z80.clock();    // ST_M2_T1_ADDRWR -> ST_M2_T2_WAITST
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0002);
    z80.clock();    // ST_M2_T2_WAITST -> ST_M2_T3_DATARD
    BOOST_CHECK(z80.state == Z80State::ST_M2_T3_DATARD);
    z80.d = 0xA5;
    z80.clock();    // ST_M2_T3_DATARD -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.af_pair[z80.decoder.registerSet].h, 0xA5);

    // LD B, n
    z80.reset();
    z80.clock();    // ST_RESET        -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0000);
    z80.clock();    // ST_M1_T1_ADDRWR -> ST_M1_T2_DATARD 
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0001);
    z80.clock();    // ST_M1_T2_DATARD -> ST_M1_T3_RFSH1
    z80.d = 0x06;   // LD B, n
    z80.clock();    // ST_M1_T3_RFSH1  -> ST_M1_T4_RFSH2
    BOOST_CHECK_EQUAL(z80.decoder.opcode, 0x06);
    z80.clock();    // ST_M1_T4_RFSH2  -> ST_M2_T1_ADDRWR
    BOOST_CHECK(z80.state == Z80State::ST_M2_T1_ADDRWR);
    BOOST_CHECK_EQUAL(z80.decoder.opcode, 0x06);
    BOOST_CHECK_EQUAL(z80.decoder.dstRegister, 0x00);
    z80.clock();    // ST_M2_T1_ADDRWR -> ST_M2_T2_WAITST
    BOOST_CHECK_EQUAL(z80.pc.w, 0x0002);
    z80.clock();    // ST_M2_T2_WAITST -> ST_M2_T3_DATARD
    BOOST_CHECK(z80.state == Z80State::ST_M2_T3_DATARD);
    z80.d = 0x34;
    z80.clock();    // ST_M2_T3_DATARD -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[z80.decoder.registerSet].h, 0x34);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_n_test)
{
    Z80 z80;
    vector<Memory> memory;
    memory.push_back(Memory(14, true));
    memory.push_back(Memory(14, false));
    memory.push_back(Memory(14, false));
    memory.push_back(Memory(14, false));

    // My first program
    memory[0].memory[0x0000] = 0x3E; memory[0].memory[0x0001] = 0xB7;
    memory[0].memory[0x0002] = 0x06; memory[0].memory[0x0003] = 0xB0;
    memory[0].memory[0x0004] = 0x0E; memory[0].memory[0x0005] = 0xB1;
    memory[0].memory[0x0006] = 0x16; memory[0].memory[0x0007] = 0xB2;
    memory[0].memory[0x0008] = 0x1E; memory[0].memory[0x0009] = 0xB3;
    memory[0].memory[0x000A] = 0x26; memory[0].memory[0x000B] = 0xB4;
    memory[0].memory[0x000C] = 0x2E; memory[0].memory[0x000D] = 0xB5;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 0x0008; ++i)
    {
        z80.clock(); memory[0].addr = z80.a; memory[0].clock();
        z80.clock(); z80.d = memory[0].data; memory[0].clock();
        z80.clock(); memory[0].clock();
        z80.clock(); memory[0].clock();
        z80.clock(); memory[0].addr = z80.a; memory[0].clock();
        z80.clock(); z80.d = memory[0].data; memory[0].clock();
        z80.clock(); memory[0].clock();
    }

    BOOST_CHECK_EQUAL(z80.decoder.af_pair[z80.decoder.registerSet].h, 0xB7);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[z80.decoder.registerSet].h, 0xB0);
    BOOST_CHECK_EQUAL(z80.decoder.bc_pair[z80.decoder.registerSet].l, 0xB1);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[z80.decoder.registerSet].h, 0xB2);
    BOOST_CHECK_EQUAL(z80.decoder.de_pair[z80.decoder.registerSet].l, 0xB3);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[z80.decoder.registerSet].h, 0xB4);
    BOOST_CHECK_EQUAL(z80.decoder.hl_pair[z80.decoder.registerSet].l, 0xB5);

    // LD r, r' tests
}

// EOF
// vim: et:sw=4:ts=4
