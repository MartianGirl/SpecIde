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

BOOST_AUTO_TEST_CASE(execute_ld_memhl_n_test)
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

BOOST_AUTO_TEST_CASE(execute_ld_r_a_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD A, 35h
    // LD R, A
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x35;
    m.memory[0x0002] = 0xED; m.memory[0x0003] = 0x4F;

    z80.reset(); z80.clock();
    // LD A, 00h
    for (size_t i = 0; i != 7; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0x35);
    
    // LD R, A
    for (size_t i = 0; i != 4; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.prefix, PREFIX_ED);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    for (size_t i = 0; i != 4; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }

    BOOST_CHECK(z80.state == Z80State::ST_M0_T0_WAITST);
    for (size_t i = 0; i != 1; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.l, 0x35);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(execute_ld_i_a_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD A, 43h
    // LD I, A
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x43;
    m.memory[0x0002] = 0xED; m.memory[0x0003] = 0x47;

    z80.reset(); z80.clock();
    // LD A, 00h
    for (size_t i = 0; i != 7; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0x43);
    
    // LD I, A
    for (size_t i = 0; i != 9; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.h, 0x43);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(execute_ld_a_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD A, ABh
    // LD R, A
    // LD A, R
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xAB;
    m.memory[0x0002] = 0xED; m.memory[0x0003] = 0x4F;
    m.memory[0x0004] = 0xED; m.memory[0x0005] = 0x5F;

    z80.reset(); z80.clock();
    // LD A, ABh
    for (size_t i = 0; i != 7; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xAB);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    
    // LD R, A
    for (size_t i = 0; i != 9; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.l, 0xAB);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);

    // LD A, R
    for (size_t i = 0; i != 9; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xAD);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->l, 0xA9);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(execute_ld_a_i_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD A, 00h
    // LD I, A
    // LD A, I
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x00;
    m.memory[0x0002] = 0xED; m.memory[0x0003] = 0x47;
    m.memory[0x0004] = 0xED; m.memory[0x0005] = 0x57;

    z80.reset(); z80.clock();
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
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    
    // LD I, A
    for (size_t i = 0; i != 9; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.h, 0x00);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);

    // LD A, I
    for (size_t i = 0; i != 9; ++i)
    {
        z80.clock(); m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock(); z80.d = m.d;
    }
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->l, 0x41);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(execute_ld_rx_rx_test)
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

            m.memory[0x0002] = 0xDD;
            m.memory[0x0003] = static_cast<uint8_t>((z << 3) | 0x47);

            m.memory[0x0004] = 0xDD;
            m.memory[0x0005] = static_cast<uint8_t>((y << 3) | z | 0x40);

            m.memory[0x0006] = 0x3E;
            m.memory[0x0007] = 0x00;

            m.memory[0x0008] = 0xDD;
            m.memory[0x0009] = static_cast<uint8_t>(y | 0x78);

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
            for (size_t i = 0; i != 8; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regx8[z]), m.memory[0x0001]);

            // LD y, z
            for (size_t i = 0; i != 8; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regx8[y]), m.memory[0x0001]);

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
            for (size_t i = 0; i != 8; ++i)
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

BOOST_AUTO_TEST_CASE(execute_ld_ry_ry_test)
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

            m.memory[0x0002] = 0xFD;
            m.memory[0x0003] = static_cast<uint8_t>((z << 3) | 0x47);

            m.memory[0x0004] = 0xFD;
            m.memory[0x0005] = static_cast<uint8_t>((y << 3) | z | 0x40);

            m.memory[0x0006] = 0x3E;
            m.memory[0x0007] = 0x00;

            m.memory[0x0008] = 0xFD;
            m.memory[0x0009] = static_cast<uint8_t>(y | 0x78);

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
            for (size_t i = 0; i != 8; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regy8[z]), m.memory[0x0001]);

            // LD y, z
            for (size_t i = 0; i != 8; ++i)
            {
                z80.clock(); m.a = z80.a; m.d = z80.d;
                m.as_ = z80.c & SIGNAL_MREQ_;
                m.rd_ = z80.c & SIGNAL_RD_;
                m.wr_ = z80.c & SIGNAL_WR_;
                m.clock(); z80.d = m.d;
            }
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regy8[y]), m.memory[0x0001]);

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
            for (size_t i = 0; i != 8; ++i)
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

BOOST_AUTO_TEST_CASE(execute_ld_rx_n_test)
{
    Z80 z80;
    Memory m(16, false);

    // My first program
    // LD A, D7h
    // LD B, D0h
    // LD C, D1h
    // LD D, D2h
    // LD E, D3h
    // LD IXh, D4h
    // LD IXl, D5h
    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x3E; m.memory[0x0002] = 0xD7;
    m.memory[0x0003] = 0xDD; m.memory[0x0004] = 0x06; m.memory[0x0005] = 0xD0;
    m.memory[0x0006] = 0xDD; m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0xD1;
    m.memory[0x0009] = 0xDD; m.memory[0x000A] = 0x16; m.memory[0x000B] = 0xD2;
    m.memory[0x000C] = 0xDD; m.memory[0x000D] = 0x1E; m.memory[0x000E] = 0xD3;
    m.memory[0x000F] = 0xDD; m.memory[0x0010] = 0x26; m.memory[0x0011] = 0xD4;
    m.memory[0x0012] = 0xDD; m.memory[0x0013] = 0x2E; m.memory[0x0014] = 0xD5;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 77; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xD7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->h, 0xD0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->l, 0xD1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->h, 0xD2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->l, 0xD3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.h, 0xD4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.l, 0xD5);
}

BOOST_AUTO_TEST_CASE(execute_ld_ry_n_test)
{
    Z80 z80;
    Memory m(16, false);

    // My first program
    // LD A, F7h
    // LD B, F0h
    // LD C, F1h
    // LD D, F2h
    // LD E, F3h
    // LD IYh, F4h
    // LD IYl, F5h
    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x3E; m.memory[0x0002] = 0xF7;
    m.memory[0x0003] = 0xFD; m.memory[0x0004] = 0x06; m.memory[0x0005] = 0xF0;
    m.memory[0x0006] = 0xFD; m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0xF1;
    m.memory[0x0009] = 0xFD; m.memory[0x000A] = 0x16; m.memory[0x000B] = 0xF2;
    m.memory[0x000C] = 0xFD; m.memory[0x000D] = 0x1E; m.memory[0x000E] = 0xF3;
    m.memory[0x000F] = 0xFD; m.memory[0x0010] = 0x26; m.memory[0x0011] = 0xF4;
    m.memory[0x0012] = 0xFD; m.memory[0x0013] = 0x2E; m.memory[0x0014] = 0xF5;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 77; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xF7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->h, 0xF0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->l, 0xF1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->h, 0xF2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->l, 0xF3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.h, 0xF4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.l, 0xF5);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_memhl_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD H, 01h
    // LD L, 07h
    // LD A, (HL)
    // LD L, 00h
    // LD B, (HL)
    // LD L, 01h
    // LD C, (HL)
    // LD L, 02h
    // LD D, (HL)
    // LD L, 03h
    // LD E, (HL)
    // LD L, 05h
    // LD L, (HL)
    // LD L, 04h
    // LD H, (HL)
    m.memory[0x0000] = 0x26; m.memory[0x0001] = 0x01;   // LD H, 01h
    m.memory[0x0002] = 0x2E; m.memory[0x0003] = 0x07;   // LD L, 07h
    m.memory[0x0004] = 0x7E;                            // LD A, (HL)
    m.memory[0x0005] = 0x2E; m.memory[0x0006] = 0x00;   // LD L, 00h
    m.memory[0x0007] = 0x46;                            // LD B, (HL)
    m.memory[0x0008] = 0x2E; m.memory[0x0009] = 0x01;   // LD L, 01h
    m.memory[0x000A] = 0x4E;                            // LD C, (HL)
    m.memory[0x000B] = 0x2E; m.memory[0x000C] = 0x02;   // LD L, 02h
    m.memory[0x000D] = 0x56;                            // LD D, (HL)
    m.memory[0x000E] = 0x2E; m.memory[0x000F] = 0x03;   // LD L, 03h
    m.memory[0x0010] = 0x5E;                            // LD E, (HL)
    m.memory[0x0011] = 0x2E; m.memory[0x0012] = 0x04;   // LD L, 04h
    m.memory[0x0013] = 0x66;                            // LD H, (HL)
    m.memory[0x0014] = 0x2E; m.memory[0x0015] = 0x05;   // LD L, 05h
    m.memory[0x0016] = 0x6E;                            // LD L, (HL)

    m.memory[0x0100] = 0xB0;
    m.memory[0x0101] = 0xC1;
    m.memory[0x0102] = 0xD2;
    m.memory[0x0103] = 0xE3;
    m.memory[0x0104] = 0x44;
    m.memory[0x0107] = 0xA7;
    m.memory[0x4405] = 0x15;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 105; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->h, 0xB0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->l, 0xC1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->h, 0xD2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->l, 0xE3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->h, 0x44);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->l, 0x15);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_memix_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD IXh, 01h
    // LD IXl, 04h
    // LD A, (IX + 3)
    // LD B, (IX - 4)
    // LD C, (IX - 3)
    // LD D, (IX - 2)
    // LD E, (IX - 1)
    // LD H, (IX + 0)
    // LD L, (IX + 1)
    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x26; m.memory[0x0002] = 0x01;   // LD IXh, 01h
    m.memory[0x0003] = 0xDD; m.memory[0x0004] = 0x2E; m.memory[0x0005] = 0x04;   // LD IXl, 04h
    m.memory[0x0006] = 0xDD; m.memory[0x0007] = 0x7E; m.memory[0x0008] = 0x03;   // LD A, (IX + 3)
    m.memory[0x0009] = 0xDD; m.memory[0x000A] = 0x46; m.memory[0x000B] = 0xFC;   // LD B, (IX - 4)
    m.memory[0x000C] = 0xDD; m.memory[0x000D] = 0x4E; m.memory[0x000E] = 0xFD;   // LD C, (IX - 3)
    m.memory[0x000F] = 0xDD; m.memory[0x0010] = 0x56; m.memory[0x0011] = 0xFE;   // LD D, (IX - 2)
    m.memory[0x0012] = 0xDD; m.memory[0x0013] = 0x5E; m.memory[0x0014] = 0xFF;   // LD E, (IX - 1)
    m.memory[0x0015] = 0xDD; m.memory[0x0016] = 0x66; m.memory[0x0017] = 0x00;   // LD H, (IX + 0)
    m.memory[0x0018] = 0xDD; m.memory[0x0019] = 0x6E; m.memory[0x001A] = 0x01;   // LD L, (IX + 1)

    m.memory[0x0100] = 0xB0;
    m.memory[0x0101] = 0xC1;
    m.memory[0x0102] = 0xD2;
    m.memory[0x0103] = 0xE3;
    m.memory[0x0104] = 0x44;
    m.memory[0x0105] = 0x15;
    m.memory[0x0107] = 0xA7;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 149; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->w, 0x4415);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_memiy_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD IYh, 01h
    // LD IYl, 04h
    // LD A, (IY + 3)
    // LD B, (IY - 4)
    // LD C, (IY - 3)
    // LD D, (IY - 2)
    // LD E, (IY - 1)
    // LD H, (IY + 0)
    // LD L, (IY + 1)
    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x26; m.memory[0x0002] = 0x01;   // LD IYh, 01h
    m.memory[0x0003] = 0xFD; m.memory[0x0004] = 0x2E; m.memory[0x0005] = 0x04;   // LD IYl, 04h
    m.memory[0x0006] = 0xFD; m.memory[0x0007] = 0x7E; m.memory[0x0008] = 0x03;   // LD A, (IY + 3)
    m.memory[0x0009] = 0xFD; m.memory[0x000A] = 0x46; m.memory[0x000B] = 0xFC;   // LD B, (IY - 4)
    m.memory[0x000C] = 0xFD; m.memory[0x000D] = 0x4E; m.memory[0x000E] = 0xFD;   // LD C, (IY - 3)
    m.memory[0x000F] = 0xFD; m.memory[0x0010] = 0x56; m.memory[0x0011] = 0xFE;   // LD D, (IY - 2)
    m.memory[0x0012] = 0xFD; m.memory[0x0013] = 0x5E; m.memory[0x0014] = 0xFF;   // LD E, (IY - 1)
    m.memory[0x0015] = 0xFD; m.memory[0x0016] = 0x66; m.memory[0x0017] = 0x00;   // LD H, (IY + 0)
    m.memory[0x0018] = 0xFD; m.memory[0x0019] = 0x6E; m.memory[0x001A] = 0x01;   // LD L, (IY + 1)

    m.memory[0x0100] = 0xB0;
    m.memory[0x0101] = 0xC1;
    m.memory[0x0102] = 0xD2;
    m.memory[0x0103] = 0xE3;
    m.memory[0x0104] = 0x44;
    m.memory[0x0105] = 0x15;
    m.memory[0x0107] = 0xA7;

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 149; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->w, 0x4415);
}

BOOST_AUTO_TEST_CASE(execute_ld_memhl_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // LD A, A7h
    // LD B, B0h
    // LD C, C1h
    // LD D, D2h
    // LD E, E3h
    // LD H, 01h
    // LD L, 07h
    // LD (HL), A
    // LD L, 00h
    // LD (HL), B
    // LD L, 01h
    // LD (HL), C
    // LD L, 02h
    // LD (HL), D
    // LD L, 03h
    // LD (HL), E
    // LD L, 04h
    // LD (HL), H
    // LD L, 05h
    // LD (HL), L
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xA7;   // LD A, A7h
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xB0;   // LD B, B0h
    m.memory[0x0004] = 0x0E; m.memory[0x0005] = 0xC1;   // LD C, C1h
    m.memory[0x0006] = 0x16; m.memory[0x0007] = 0xD2;   // LD D, D2h
    m.memory[0x0008] = 0x1E; m.memory[0x0009] = 0xE3;   // LD E, E3h
    m.memory[0x000A] = 0x26; m.memory[0x000B] = 0x01;   // LD H, 01h
    m.memory[0x000C] = 0x2E; m.memory[0x000D] = 0x07;   // LD L, 07h
    m.memory[0x000E] = 0x77;                            // LD (HL), A 
    m.memory[0x000F] = 0x2E; m.memory[0x0010] = 0x00;   // LD L, 00h
    m.memory[0x0011] = 0x70;                            // LD (HL), B
    m.memory[0x0012] = 0x2E; m.memory[0x0013] = 0x01;   // LD L, 01h
    m.memory[0x0014] = 0x71;                            // LD (HL), C
    m.memory[0x0015] = 0x2E; m.memory[0x0016] = 0x02;   // LD L, 02h
    m.memory[0x0017] = 0x72;                            // LD (HL), D
    m.memory[0x0018] = 0x2E; m.memory[0x0019] = 0x03;   // LD L, 03h
    m.memory[0x001A] = 0x73;                            // LD (HL), E
    m.memory[0x001B] = 0x2E; m.memory[0x001C] = 0x04;   // LD L, 04h
    m.memory[0x001D] = 0x74;                            // LD (HL), H
    m.memory[0x001E] = 0x2E; m.memory[0x001F] = 0x05;   // LD L, 05h
    m.memory[0x0020] = 0x75;                            // LD (HL), L

    z80.reset(); z80.clock();
    for (size_t i = 0; i != 140; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }

    BOOST_CHECK_EQUAL(z80.decoder.regs.af->h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc->w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de->w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl->w, 0x0105);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xA7);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xB0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xC1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xD2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0xE3);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x01);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x05);
}
// EOF
// vim: et:sw=4:ts=4
