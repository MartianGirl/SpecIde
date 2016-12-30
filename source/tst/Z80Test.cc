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

using namespace std;

BOOST_AUTO_TEST_CASE(state_machine_test)
{
    // Create a Z80 and some memory.
    Z80 z80;

    // Reset the Z80, clock it. PC should be 0x0000.
    z80.reset();
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF);

    // Clock it once. We've run ST_M1_T1_ADDRWR.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T2_DATARD);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T2_DATARD.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T3_RFSH1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T3_RFSH1
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T4_RFSH2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0xFF7F);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RFSH_));

    // Clock it once. We've run ST_M1_T4_RFSH2
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[0].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_pair[1].w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.registerSet, 0x00);
    BOOST_CHECK_EQUAL(z80.a, 0xFF7F);
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
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0000);
    z80.clock();    // ST_M1_T1_ADDRWR -> ST_M1_T2_DATARD 
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    z80.clock();    // ST_M1_T2_DATARD -> ST_M1_T3_RFSH1
    z80.d = 0x3E;   // LD A, n
    z80.clock();    // ST_M1_T3_RFSH1  -> ST_M1_T4_RFSH2
    z80.clock();    // ST_M1_T4_RFSH2  -> ST_M2_T1_ADDRWR
    BOOST_CHECK(z80.state == Z80State::ST_M2_T1_ADDRWR);
    z80.clock();    // ST_M2_T1_ADDRWR -> ST_M2_T2_WAITST
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0002);
    z80.clock();    // ST_M2_T2_WAITST -> ST_M2_T3_DATARD
    BOOST_CHECK(z80.state == Z80State::ST_M2_T3_DATARD);
    z80.d = 0xA5;
    z80.clock();    // ST_M2_T3_DATARD -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xA5);

    // LD B, n
    z80.reset();
    z80.clock();    // ST_RESET        -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0000);
    z80.clock();    // ST_M1_T1_ADDRWR -> ST_M1_T2_DATARD 
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    z80.clock();    // ST_M1_T2_DATARD -> ST_M1_T3_RFSH1
    z80.d = 0x06;   // LD B, n
    z80.clock();    // ST_M1_T3_RFSH1  -> ST_M1_T4_RFSH2
    z80.clock();    // ST_M1_T4_RFSH2  -> ST_M2_T1_ADDRWR
    BOOST_CHECK(z80.state == Z80State::ST_M2_T1_ADDRWR);
    z80.clock();    // ST_M2_T1_ADDRWR -> ST_M2_T2_WAITST
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0002);
    z80.clock();    // ST_M2_T2_WAITST -> ST_M2_T3_DATARD
    BOOST_CHECK(z80.state == Z80State::ST_M2_T3_DATARD);
    z80.d = 0x34;
    z80.clock();    // ST_M2_T3_DATARD -> ST_M1_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->h, 0x34);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_n_test)
{
    Z80 z80;
    Memory m(16, true);

    // My first program
    // LD A, B7h
    // LD B, B0h
    // LD C, B1h
    // LD D, B2h
    // LD E, B3h
    // LD H, B4h
    // LD L, B5h
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xB7;
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xB0;
    m.memory[0x0004] = 0x0E; m.memory[0x0005] = 0xB1;
    m.memory[0x0006] = 0x16; m.memory[0x0007] = 0xB2;
    m.memory[0x0008] = 0x1E; m.memory[0x0009] = 0xB3;
    m.memory[0x000A] = 0x26; m.memory[0x000B] = 0xB4;
    m.memory[0x000C] = 0x2E; m.memory[0x000D] = 0xB5;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 56; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xB7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->h, 0xB0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->l, 0xB1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->h, 0xB2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->l, 0xB3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->h, 0xB4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->l, 0xB5);
}

BOOST_AUTO_TEST_CASE(execute_ld_HL_n_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD H, 0x40
    // LD L, 0x60
    // LD (HL), 0x34
    m.memory[0x0000] = 0x26; m.memory[0x0001] = 0x40;
    m.memory[0x0002] = 0x2E; m.memory[0x0003] = 0x60;
    m.memory[0x0004] = 0x36; m.memory[0x0005] = 0x34;
    m.memory[0x0006] = 0x26; m.memory[0x0007] = 0x22;
    m.memory[0x0008] = 0x2E; m.memory[0x0009] = 0x23;
    m.memory[0x000A] = 0x36; m.memory[0x000B] = 0x78;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 24; ++i)
    {
        z80.clock();
        cout << hex << " A: " << m.a;
        cout << hex << " D: " << static_cast<int>(z80.d);
        cout << hex << " C: " << z80.c << " -> ";
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        cout << hex << " A: " << m.a;
        cout << hex << " D: " << static_cast<int>(z80.d);
        cout << hex << " C: " << z80.c << endl;
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->h, 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->l, 0x60);
    BOOST_CHECK_EQUAL(m.memory[0x4060], 0x34);

    for (size_t i = 0; i != 24; ++i)
    {
        z80.clock();
        cout << hex << " A: " << m.a;
        cout << hex << " D: " << static_cast<int>(z80.d);
        cout << hex << " C: " << z80.c << " -> ";
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        cout << hex << " A: " << m.a;
        cout << hex << " D: " << static_cast<int>(z80.d);
        cout << hex << " C: " << z80.c << endl;
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->h, 0x22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->l, 0x23);
    BOOST_CHECK_EQUAL(m.memory[0x2223], 0x78);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_r_test)
{
    Z80 z80;
    Memory m(16, false);

    for (size_t y = 0; y != 6; ++y)
    {
        for (size_t z = 0; z != 6; ++z)
        {
            // LD A, n
            // LD z, A
            // LD y, z
            // LD A, 00h
            // LD A, y
            m.memory[0x0000] = 0x3E;
            m.memory[0x0001] = static_cast<uint8_t>((y << 3) | z);
            m.memory[0x0002] = static_cast<uint8_t>((z << 3) | 0x47);
            m.memory[0x0003] = static_cast<uint8_t>((y << 3) | z | 0x40);
            m.memory[0x0004] = 0x3E;
            m.memory[0x0005] = 0x00;
            m.memory[0x0006] = static_cast<uint8_t>(y | 0x78);

            z80.reset(); z80.clock();
            // LD A, n
            for (size_t i = 0; i != 7; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, m.memory[0x0001]);

            // LD z, A
            for (size_t i = 0; i != 4; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.reg8[z]), m.memory[0x0001]);

            // LD y, z
            for (size_t i = 0; i != 4; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.reg8[y]), m.memory[0x0001]);

            // LD A, 00h
            for (size_t i = 0; i != 7; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0x00);

            // LD A, y
            for (size_t i = 0; i != 4; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, m.memory[0x0001]);
        }
    }
}
// EOF
// vim: et:sw=4:ts=4
