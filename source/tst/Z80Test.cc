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

void startZ80(Z80& z80)
{
    z80.reset(); z80.clock();
}

void runCycles(Z80& z80, Memory& m, size_t cycles)
{
    for (size_t i = 0; i != cycles; ++i)
    {
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = z80.c & SIGNAL_MREQ_;
        m.rd_ = z80.c & SIGNAL_RD_;
        m.wr_ = z80.c & SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }
}

BOOST_AUTO_TEST_CASE(state_machine_test)
{
    // Create a Z80 and some memory.
    Z80 z80;

    // Reset the Z80, clock it. PC should be 0x0000.
    z80.reset();
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0000);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.a, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF);

    // Clock it once. We've run ST_M1_T1_ADDRWR.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T2_DATARD);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T2_DATARD.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T3_RFSH1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.a, 0x0000);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_));

    // Clock it once. We've run ST_M1_T3_RFSH1
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T4_RFSH2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.a, 0xFF7F);
    BOOST_CHECK_EQUAL(z80.d, 0xFF);
    BOOST_CHECK_EQUAL(z80.c, 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RFSH_));

    // Clock it once. We've run ST_M1_T4_RFSH2
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
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
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA5);

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
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x34);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_n_test)
{
    Z80 z80;
    Memory m(16, false);

    // My first program
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xB7;   // LD A, B7h
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xB0;   // LD B, B0h
    m.memory[0x0004] = 0x0E; m.memory[0x0005] = 0xB1;   // LD C, B1h
    m.memory[0x0006] = 0x16; m.memory[0x0007] = 0xB2;   // LD D, B2h
    m.memory[0x0008] = 0x1E; m.memory[0x0009] = 0xB3;   // LD E, B3h
    m.memory[0x000A] = 0x26; m.memory[0x000B] = 0xB4;   // LD H, B4h
    m.memory[0x000C] = 0x2E; m.memory[0x000D] = 0xB5;   // LD L, B5h

    runCycles(z80, m, 56);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xB7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xB0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.l, 0xB1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.h, 0xB2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.l, 0xB3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.h, 0xB4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.l, 0xB5);
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

    startZ80(z80);
    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.h, 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.l, 0x60);
    BOOST_CHECK_EQUAL(m.memory[0x4060], 0x34);

    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.h, 0x22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.l, 0x23);
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

            startZ80(z80);
            // LD A, n
            runCycles(z80, m, 7);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x0001]);

            // LD z, A
            runCycles(z80, m, 4);
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.reg8[z]), m.memory[0x0001]);

            // LD y, z
            runCycles(z80, m, 4);
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.reg8[y]), m.memory[0x0001]);

            // LD A, 00h
            runCycles(z80, m, 7);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);

            // LD A, y
            runCycles(z80, m, 4);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x0001]);
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

    startZ80(z80);
    // LD A, 00h
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x35);
    
    // LD R, A
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.prefix, PREFIX_ED);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    runCycles(z80, m, 4);

    BOOST_CHECK(z80.state == Z80State::ST_M0_T0_WAITST);
    runCycles(z80, m, 1);
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

    startZ80(z80);
    // LD A, 00h
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x43);
    
    // LD I, A
    runCycles(z80, m, 9);
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

    startZ80(z80);
    // LD A, ABh
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xAB);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    
    // LD R, A
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.l, 0xAB);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);

    // LD A, R
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xAD);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA9);
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

    startZ80(z80);
    // LD A, 00h
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);
    
    // LD I, A
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.h, 0x00);
    BOOST_CHECK(z80.state == Z80State::ST_M1_T1_ADDRWR);

    // LD A, I
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x41);
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

            startZ80(z80);
            // LD A, n
            runCycles(z80, m, 7);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x0001]);

            // LD z, A
            runCycles(z80, m, 8);
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regx8[z]), m.memory[0x0001]);

            // LD y, z
            runCycles(z80, m, 8);
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regx8[y]), m.memory[0x0001]);

            // LD A, 00h
            runCycles(z80, m, 7);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);

            // LD A, y
            runCycles(z80, m, 8);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x0001]);
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

            startZ80(z80);
            // LD A, n
            runCycles(z80, m, 7);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x0001]);

            // LD z, A
            runCycles(z80, m, 8);
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regy8[z]), m.memory[0x0001]);

            // LD y, z
            runCycles(z80, m, 8);
            BOOST_CHECK_EQUAL(*(z80.decoder.regs.regy8[y]), m.memory[0x0001]);

            // LD A, 00h
            runCycles(z80, m, 7);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);

            // LD A, y
            runCycles(z80, m, 8);
            BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x0001]);
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

    startZ80(z80);
    runCycles(z80, m, 77);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xD7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xD0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.l, 0xD1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.h, 0xD2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.l, 0xD3);
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

    startZ80(z80);
    runCycles(z80, m, 77);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xF7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xF0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.l, 0xF1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.h, 0xF2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.l, 0xF3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.h, 0xF4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.l, 0xF5);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_memhl_test)
{
    Z80 z80;
    Memory m(16, false);

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

    startZ80(z80);
    runCycles(z80, m, 105);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xB0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.l, 0xC1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.h, 0xD2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.l, 0xE3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.h, 0x44);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.l, 0x15);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_memix_test)
{
    Z80 z80;
    Memory m(16, false);

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

    startZ80(z80);
    runCycles(z80, m, 149);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4415);
}

BOOST_AUTO_TEST_CASE(execute_ld_r_memiy_test)
{
    Z80 z80;
    Memory m(16, false);

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

    startZ80(z80);
    runCycles(z80, m, 149);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4415);
}

BOOST_AUTO_TEST_CASE(execute_ld_memhl_r_test)
{
    Z80 z80;
    Memory m(16, false);

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

    startZ80(z80);
    runCycles(z80, m, 140);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x0105);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xA7);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xB0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xC1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xD2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0xE3);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x01);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x05);
}

BOOST_AUTO_TEST_CASE(execute_ld_memix_r_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x26; m.memory[0x0002] = 0x01;  // LD IXh, 01h
    m.memory[0x0003] = 0xDD; m.memory[0x0004] = 0x2E; m.memory[0x0005] = 0x04;  // LD IXl, 04h
    m.memory[0x0006] = 0x3E; m.memory[0x0007] = 0xA7;                           // LD A, A7h
    m.memory[0x0008] = 0x06; m.memory[0x0009] = 0xB0;                           // LD B, B0h
    m.memory[0x000A] = 0x0E; m.memory[0x000B] = 0xC1;                           // LD C, C1h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0xD2;                           // LD D, D2h
    m.memory[0x000E] = 0x1E; m.memory[0x000F] = 0xE3;                           // LD E, E3h
    m.memory[0x0010] = 0x26; m.memory[0x0011] = 0x44;                           // LD H, 44h
    m.memory[0x0012] = 0x2E; m.memory[0x0013] = 0x15;                           // LD L, 15h
    m.memory[0x0014] = 0xDD; m.memory[0x0015] = 0x77; m.memory[0x0016] = 0x03;  // LD (IX + 3), A
    m.memory[0x0017] = 0xDD; m.memory[0x0018] = 0x70; m.memory[0x0019] = 0xFC;  // LD (IX - 4), B
    m.memory[0x001A] = 0xDD; m.memory[0x001B] = 0x71; m.memory[0x001C] = 0xFD;  // LD (IX - 3), C
    m.memory[0x001D] = 0xDD; m.memory[0x001E] = 0x72; m.memory[0x001F] = 0xFE;  // LD (IX - 2), D
    m.memory[0x0020] = 0xDD; m.memory[0x0021] = 0x73; m.memory[0x0022] = 0xFF;  // LD (IX - 1), E
    m.memory[0x0023] = 0xDD; m.memory[0x0024] = 0x74; m.memory[0x0025] = 0x00;  // LD (IX + 0), H
    m.memory[0x0026] = 0xDD; m.memory[0x0027] = 0x75; m.memory[0x0028] = 0x01;  // LD (IX + 1), L

    startZ80(z80);
    runCycles(z80, m, 204);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4415);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xA7);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xB0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xC1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xD2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0xE3);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x44);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x15);
}

BOOST_AUTO_TEST_CASE(execute_ld_memiy_r_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x26; m.memory[0x0002] = 0x01;  // LD IYh, 01h
    m.memory[0x0003] = 0xFD; m.memory[0x0004] = 0x2E; m.memory[0x0005] = 0x04;  // LD IYl, 04h
    m.memory[0x0006] = 0x3E; m.memory[0x0007] = 0xA7;                           // LD A, A7h
    m.memory[0x0008] = 0x06; m.memory[0x0009] = 0xB0;                           // LD B, B0h
    m.memory[0x000A] = 0x0E; m.memory[0x000B] = 0xC1;                           // LD C, C1h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0xD2;                           // LD D, D2h
    m.memory[0x000E] = 0x1E; m.memory[0x000F] = 0xE3;                           // LD E, E3h
    m.memory[0x0010] = 0x26; m.memory[0x0011] = 0x44;                           // LD H, 44h
    m.memory[0x0012] = 0x2E; m.memory[0x0013] = 0x15;                           // LD L, 15h
    m.memory[0x0014] = 0xFD; m.memory[0x0015] = 0x77; m.memory[0x0016] = 0x03;  // LD (IY + 3), A
    m.memory[0x0017] = 0xFD; m.memory[0x0018] = 0x70; m.memory[0x0019] = 0xFC;  // LD (IY - 4), B
    m.memory[0x001A] = 0xFD; m.memory[0x001B] = 0x71; m.memory[0x001C] = 0xFD;  // LD (IY - 3), C
    m.memory[0x001D] = 0xFD; m.memory[0x001E] = 0x72; m.memory[0x001F] = 0xFE;  // LD (IY - 2), D
    m.memory[0x0020] = 0xFD; m.memory[0x0021] = 0x73; m.memory[0x0022] = 0xFF;  // LD (IY - 1), E
    m.memory[0x0023] = 0xFD; m.memory[0x0024] = 0x74; m.memory[0x0025] = 0x00;  // LD (IY + 0), H
    m.memory[0x0026] = 0xFD; m.memory[0x0027] = 0x75; m.memory[0x0028] = 0x01;  // LD (IY + 1), L

    startZ80(z80);
    runCycles(z80, m, 204);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4415);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xA7);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xB0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xC1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xD2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0xE3);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x44);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x15);
}

BOOST_AUTO_TEST_CASE(execute_ld_memix_n_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x26; m.memory[0x0002] = 0x01;  // LD IXh, 01h
    m.memory[0x0003] = 0xDD; m.memory[0x0004] = 0x2E; m.memory[0x0005] = 0x04;  // LD IXl, 04h
    m.memory[0x0006] = 0xDD; m.memory[0x0007] = 0x36;
    m.memory[0x0008] = 0xFC; m.memory[0x0009] = 0xF0;                           // LD (IX - 4), F0h
    m.memory[0x000A] = 0xDD; m.memory[0x000B] = 0x36;
    m.memory[0x000C] = 0xFD; m.memory[0x000D] = 0xF1;                           // LD (IX - 3), F1h
    m.memory[0x000E] = 0xDD; m.memory[0x000F] = 0x36;
    m.memory[0x0010] = 0xFE; m.memory[0x0011] = 0xF2;                           // LD (IX - 2), F2h
    m.memory[0x0012] = 0xDD; m.memory[0x0013] = 0x36;
    m.memory[0x0014] = 0xFF; m.memory[0x0015] = 0xF3;                           // LD (IX - 1), F3h
    m.memory[0x0016] = 0xDD; m.memory[0x0017] = 0x36;
    m.memory[0x0018] = 0x00; m.memory[0x0019] = 0xF4;                           // LD (IX + 0), F4h
    m.memory[0x001A] = 0xDD; m.memory[0x001B] = 0x36;
    m.memory[0x001C] = 0x01; m.memory[0x001D] = 0xF5;                           // LD (IX + 1), F5h
    m.memory[0x001E] = 0xDD; m.memory[0x001F] = 0x36;
    m.memory[0x0020] = 0x02; m.memory[0x0021] = 0xF6;                           // LD (IX + 2), F6h
    m.memory[0x0022] = 0xDD; m.memory[0x0023] = 0x36;
    m.memory[0x0024] = 0x03; m.memory[0x0025] = 0xF7;                           // LD (IX + 3), F7h

    startZ80(z80);
    runCycles(z80, m, 174);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xF0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xF1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xF2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0xF3);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xF4);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0xF5);
    BOOST_CHECK_EQUAL(m.memory[0x0106], 0xF6);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xF7);
}

BOOST_AUTO_TEST_CASE(execute_ld_memiy_n_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x26; m.memory[0x0002] = 0x01;  // LD IYh, 01h
    m.memory[0x0003] = 0xFD; m.memory[0x0004] = 0x2E; m.memory[0x0005] = 0x04;  // LD IYl, 04h
    m.memory[0x0006] = 0xFD; m.memory[0x0007] = 0x36;
    m.memory[0x0008] = 0xFC; m.memory[0x0009] = 0xF0;                           // LD (IY - 4), F0h
    m.memory[0x000A] = 0xFD; m.memory[0x000B] = 0x36;
    m.memory[0x000C] = 0xFD; m.memory[0x000D] = 0xF1;                           // LD (IY - 3), F1h
    m.memory[0x000E] = 0xFD; m.memory[0x000F] = 0x36;
    m.memory[0x0010] = 0xFE; m.memory[0x0011] = 0xF2;                           // LD (IY - 2), F2h
    m.memory[0x0012] = 0xFD; m.memory[0x0013] = 0x36;
    m.memory[0x0014] = 0xFF; m.memory[0x0015] = 0xF3;                           // LD (IY - 1), F3h
    m.memory[0x0016] = 0xFD; m.memory[0x0017] = 0x36;
    m.memory[0x0018] = 0x00; m.memory[0x0019] = 0xF4;                           // LD (IY + 0), F4h
    m.memory[0x001A] = 0xFD; m.memory[0x001B] = 0x36;
    m.memory[0x001C] = 0x01; m.memory[0x001D] = 0xF5;                           // LD (IY + 1), F5h
    m.memory[0x001E] = 0xFD; m.memory[0x001F] = 0x36;
    m.memory[0x0020] = 0x02; m.memory[0x0021] = 0xF6;                           // LD (IY + 2), F6h
    m.memory[0x0022] = 0xFD; m.memory[0x0023] = 0x36;
    m.memory[0x0024] = 0x03; m.memory[0x0025] = 0xF7;                           // LD (IY + 3), F7h

    startZ80(z80);
    runCycles(z80, m, 174);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xF0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xF1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xF2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0xF3);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xF4);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0xF5);
    BOOST_CHECK_EQUAL(m.memory[0x0106], 0xF6);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xF7);
}

BOOST_AUTO_TEST_CASE(execute_ld_a_membc_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x06; m.memory[0x0001] = 0x10; // LD B, 0x10
    m.memory[0x0002] = 0x0E; m.memory[0x0003] = 0x20; // LD C, 0x20
    m.memory[0x0004] = 0x0A;                          // LD A, (BC)

    m.memory[0x1020] = 0x56;

    startZ80(z80);
    runCycles(z80, m, 21);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x1020]);
}

BOOST_AUTO_TEST_CASE(execute_ld_a_memde_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x16; m.memory[0x0001] = 0x10; // LD D, 0x10
    m.memory[0x0002] = 0x1E; m.memory[0x0003] = 0x20; // LD E, 0x20
    m.memory[0x0004] = 0x1A;                          // LD A, (BC)

    m.memory[0x1020] = 0x78;

    startZ80(z80);
    runCycles(z80, m, 21);

    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x1020]);
}

BOOST_AUTO_TEST_CASE(execute_ld_a_memnn_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x3A; m.memory[0x0001] = 0x20; m.memory[0x0002] = 0x10;  // LD A, (1020h)
    m.memory[0x1020] = 0x78;

    startZ80(z80);
    runCycles(z80, m, 13);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x1020]);
}

BOOST_AUTO_TEST_CASE(execute_ld_membc_a_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x06; m.memory[0x0001] = 0x10; // LD B, 0x10
    m.memory[0x0002] = 0x0E; m.memory[0x0003] = 0x20; // LD C, 0x20
    m.memory[0x0004] = 0x3E; m.memory[0x0005] = 0x23; // LD A, 0x23
    m.memory[0x0006] = 0x02;                          // LD (BC), A
    m.memory[0x1020] = 0x56;

    startZ80(z80);
    runCycles(z80, m, 28);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x23);
    BOOST_CHECK_EQUAL(m.memory[0x1020], 0x23);
}

BOOST_AUTO_TEST_CASE(execute_ld_memde_a_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x16; m.memory[0x0001] = 0x10; // LD D, 0x10
    m.memory[0x0002] = 0x1E; m.memory[0x0003] = 0x20; // LD E, 0x20
    m.memory[0x0004] = 0x3E; m.memory[0x0005] = 0x48; // LD A, 0x48
    m.memory[0x0006] = 0x12;                          // LD (DE), A
    m.memory[0x1020] = 0x56;

    startZ80(z80);
    runCycles(z80, m, 28);

    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x48);
    BOOST_CHECK_EQUAL(m.memory[0x1020], 0x48);
}  

BOOST_AUTO_TEST_CASE(execute_ld_memnn_a_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x69;                           // LD A, 69h
    m.memory[0x0002] = 0x32; m.memory[0x0003] = 0x20; m.memory[0x0004] = 0x10;  // LD (1020h), A
    m.memory[0x1020] = 0x78;

    startZ80(z80);
    runCycles(z80, m, 20);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x69);
    BOOST_CHECK_EQUAL(m.memory[0x1020], 0x69);
}

BOOST_AUTO_TEST_CASE(execute_ld_rp_nn_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x01; m.memory[0x0001] = 0x11; m.memory[0x0002] = 0x00;  // LD BC, 0011h
    m.memory[0x0003] = 0x11; m.memory[0x0004] = 0x33; m.memory[0x0005] = 0x22;  // LD DE, 2233h
    m.memory[0x0006] = 0x21; m.memory[0x0007] = 0x55; m.memory[0x0008] = 0x44;  // LD HL, 4455h
    m.memory[0x0009] = 0x31; m.memory[0x000A] = 0x77; m.memory[0x000B] = 0x66;  // LD SP, 6677h

    startZ80(z80);
    runCycles(z80, m, 40);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0011);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x2233);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4455);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x6677);
}

BOOST_AUTO_TEST_CASE(execute_ld_rpx_nn_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x01;
    m.memory[0x0002] = 0x11; m.memory[0x0003] = 0x00;  // LD BC, 0011h
    m.memory[0x0004] = 0xDD; m.memory[0x0005] = 0x11;
    m.memory[0x0006] = 0x33; m.memory[0x0007] = 0x22;  // LD DE, 2233h
    m.memory[0x0008] = 0xDD; m.memory[0x0009] = 0x21;
    m.memory[0x000A] = 0x55; m.memory[0x000B] = 0x44;  // LD IX, 4455h
    m.memory[0x000C] = 0xDD; m.memory[0x000D] = 0x31;
    m.memory[0x000E] = 0x77; m.memory[0x000F] = 0x66;  // LD SP, 6677h

    startZ80(z80);
    runCycles(z80, m, 56);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0011);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x2233);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x4455);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x6677);
}

BOOST_AUTO_TEST_CASE(execute_ld_rpy_nn_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x01;
    m.memory[0x0002] = 0x11; m.memory[0x0003] = 0x00;  // LD BC, 0011h
    m.memory[0x0004] = 0xFD; m.memory[0x0005] = 0x11;
    m.memory[0x0006] = 0x33; m.memory[0x0007] = 0x22;  // LD DE, 2233h
    m.memory[0x0008] = 0xFD; m.memory[0x0009] = 0x21;
    m.memory[0x000A] = 0x55; m.memory[0x000B] = 0x44;  // LD IY, 4455h
    m.memory[0x000C] = 0xFD; m.memory[0x000D] = 0x31;
    m.memory[0x000E] = 0x77; m.memory[0x000F] = 0x66;  // LD SP, 6677h

    startZ80(z80);
    runCycles(z80, m, 56);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0011);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x2233);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x4455);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x6677);
}

BOOST_AUTO_TEST_CASE(execute_ld_hl_memword_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x2A; m.memory[0x0001] = 0x34; m.memory[0x0002] = 0x12;  // LD HL, (1234h)
    
    m.memory[0x1234] = 0x11; m.memory[0x1235] = 0x44;

    startZ80(z80);
    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4411);
}

BOOST_AUTO_TEST_CASE(execute_ld_reg_memword_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xED; m.memory[0x0001] = 0x4B;
    m.memory[0x0002] = 0x00; m.memory[0x0003] = 0x02;   // LD BC, (0200h)
    m.memory[0x0004] = 0xED; m.memory[0x0005] = 0x5B;
    m.memory[0x0006] = 0x02; m.memory[0x0007] = 0x02;   // LD DE, (0202h)
    m.memory[0x0008] = 0xED; m.memory[0x0009] = 0x6B;
    m.memory[0x000A] = 0x04; m.memory[0x000B] = 0x02;   // LD HL, (0204h)
    m.memory[0x000C] = 0xED; m.memory[0x000D] = 0x7B;
    m.memory[0x000E] = 0x06; m.memory[0x000F] = 0x02;   // LD SP, (0206h)
    
    m.memory[0x0200] = 0x00; m.memory[0x0201] = 0x1B;
    m.memory[0x0202] = 0x00; m.memory[0x0203] = 0x40;
    m.memory[0x0204] = 0x4E; m.memory[0x0205] = 0x80;
    m.memory[0x0206] = 0x05; m.memory[0x0207] = 0xFE;

    startZ80(z80);
    runCycles(z80, m, 80);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x1B00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x804E);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFE05);
}

BOOST_AUTO_TEST_CASE(execute_ld_ix_memword_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x2A;
    m.memory[0x0002] = 0x34; m.memory[0x0003] = 0x12;  // LD IX, (1234h)
    
    m.memory[0x1234] = 0x78; m.memory[0x1235] = 0x56;

    startZ80(z80);
    runCycles(z80, m, 20);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x5678);
}

BOOST_AUTO_TEST_CASE(execute_ld_iy_memword_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x2A;
    m.memory[0x0002] = 0x34; m.memory[0x0003] = 0x12;  // LD IY, (1234h)
    
    m.memory[0x1234] = 0x78; m.memory[0x1235] = 0x56;

    startZ80(z80);
    runCycles(z80, m, 20);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x5678);
}

BOOST_AUTO_TEST_CASE(execute_ld_memword_hl_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x21; m.memory[0x0001] = 0x34; m.memory[0x0002] = 0x12;  // LD HL, 1234h
    m.memory[0x0003] = 0x22; m.memory[0x0004] = 0x34; m.memory[0x0005] = 0x12;  // LD (1234h), HL

    startZ80(z80);
    runCycles(z80, m, 26);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x1234);
    BOOST_CHECK_EQUAL(m.memory[0x1234], 0x34);
    BOOST_CHECK_EQUAL(m.memory[0x1235], 0x12);
}

BOOST_AUTO_TEST_CASE(execute_ld_memword_reg_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x01; m.memory[0x0001] = 0xCC; m.memory[0x0002] = 0xBB;  // LD BC, BBCCh
    m.memory[0x0003] = 0x11; m.memory[0x0004] = 0xEE; m.memory[0x0005] = 0xDD;  // LD DE, DDEEh
    m.memory[0x0006] = 0x21; m.memory[0x0007] = 0x11; m.memory[0x0008] = 0x44;  // LD HL, 4411h
    m.memory[0x0009] = 0x31; m.memory[0x000A] = 0x99; m.memory[0x000B] = 0x55;  // LD SP, 5599h
    m.memory[0x000C] = 0xED; m.memory[0x000D] = 0x43;
    m.memory[0x000E] = 0x00; m.memory[0x000F] = 0x02;   // LD (0200h), BC
    m.memory[0x0010] = 0xED; m.memory[0x0011] = 0x53;
    m.memory[0x0012] = 0x02; m.memory[0x0013] = 0x02;   // LD (0202h), DE
    m.memory[0x0014] = 0xED; m.memory[0x0015] = 0x63;
    m.memory[0x0016] = 0x04; m.memory[0x0017] = 0x02;   // LD (0204h), HL
    m.memory[0x0018] = 0xED; m.memory[0x0019] = 0x73;
    m.memory[0x001A] = 0x06; m.memory[0x001B] = 0x02;   // LD (0206h), SP
    
    startZ80(z80);
    runCycles(z80, m, 120);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xBBCC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xDDEE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4411);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x5599);
    BOOST_CHECK_EQUAL(m.memory[0x0200], 0xCC);
    BOOST_CHECK_EQUAL(m.memory[0x0201], 0xBB);
    BOOST_CHECK_EQUAL(m.memory[0x0202], 0xEE);
    BOOST_CHECK_EQUAL(m.memory[0x0203], 0xDD);
    BOOST_CHECK_EQUAL(m.memory[0x0204], 0x11);
    BOOST_CHECK_EQUAL(m.memory[0x0205], 0x44);
    BOOST_CHECK_EQUAL(m.memory[0x0206], 0x99);
    BOOST_CHECK_EQUAL(m.memory[0x0207], 0x55);
}

BOOST_AUTO_TEST_CASE(execute_ld_memword_ix_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x21;
    m.memory[0x0002] = 0x34; m.memory[0x0003] = 0x12;   // LD IX, 1234h
    m.memory[0x0004] = 0xDD; m.memory[0x0005] = 0x22;
    m.memory[0x0006] = 0x34; m.memory[0x0007] = 0x12;   // LD (1234h), IX
    
    startZ80(z80);
    runCycles(z80, m, 34);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x1234);
    BOOST_CHECK_EQUAL(m.memory[0x1234], 0x34);
    BOOST_CHECK_EQUAL(m.memory[0x1235], 0x12);
}

BOOST_AUTO_TEST_CASE(execute_ld_memword_iy_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x21;
    m.memory[0x0002] = 0x34; m.memory[0x0003] = 0x12;   // LD IY, 1234h
    m.memory[0x0004] = 0xFD; m.memory[0x0005] = 0x22;
    m.memory[0x0006] = 0x34; m.memory[0x0007] = 0x12;   // LD (1234h), IY
    
    startZ80(z80);
    runCycles(z80, m, 34);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x1234);
    BOOST_CHECK_EQUAL(m.memory[0x1234], 0x34);
    BOOST_CHECK_EQUAL(m.memory[0x1235], 0x12);
}

BOOST_AUTO_TEST_CASE(execute_ld_sp_hl_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x21; m.memory[0x0001] = 0x56; m.memory[0x0002] = 0x34;  // LD HL, 3456h
    m.memory[0x0003] = 0xF9;                                                    // LD SP, HL

    startZ80(z80);
    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x3456);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x3456);
}

BOOST_AUTO_TEST_CASE(execute_ld_sp_ix_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x21;
    m.memory[0x0002] = 0x56; m.memory[0x0003] = 0x34;  // LD IX, 3456h
    m.memory[0x0004] = 0xDD; m.memory[0x0005] = 0xF9;  // LD SP, IX

    startZ80(z80);
    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x3456);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x3456);
}

BOOST_AUTO_TEST_CASE(execute_ld_sp_iy_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x21;
    m.memory[0x0002] = 0x56; m.memory[0x0003] = 0x34;  // LD IY, 3456h
    m.memory[0x0004] = 0xFD; m.memory[0x0005] = 0xF9;  // LD SP, IY

    startZ80(z80);
    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x3456);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0x3456);
}

BOOST_AUTO_TEST_CASE(execute_push_rr_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x77;                           // LD A, 77h
    m.memory[0x0002] = 0x01; m.memory[0x0003] = 0x11; m.memory[0x0004] = 0xBB;  // LD BC, BB11h
    m.memory[0x0005] = 0x11; m.memory[0x0006] = 0x33; m.memory[0x0007] = 0x22;  // LD DE, 2233h
    m.memory[0x0008] = 0x21; m.memory[0x0009] = 0x55; m.memory[0x000A] = 0x44;  // LD HL, 4455h
    m.memory[0x000B] = 0x31; m.memory[0x000C] = 0x80; m.memory[0x000D] = 0xC0;  // LD SP, C080h
    m.memory[0x000E] = 0xC5;                                                    // PUSH BC
    m.memory[0x000F] = 0xD5;                                                    // PUSH DE
    m.memory[0x0010] = 0xE5;                                                    // PUSH HL
    m.memory[0x0011] = 0xF5;                                                    // PUSH AF

    startZ80(z80);
    runCycles(z80, m, 91);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC078);
    BOOST_CHECK_EQUAL(m.memory[0xC078], z80.decoder.regs.af.l);
    BOOST_CHECK_EQUAL(m.memory[0xC079], z80.decoder.regs.af.h);
    BOOST_CHECK_EQUAL(m.memory[0xC07A], z80.decoder.regs.hl.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07B], z80.decoder.regs.hl.h);
    BOOST_CHECK_EQUAL(m.memory[0xC07C], z80.decoder.regs.de.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07D], z80.decoder.regs.de.h);
    BOOST_CHECK_EQUAL(m.memory[0xC07E], z80.decoder.regs.bc.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07F], z80.decoder.regs.bc.h);
}

BOOST_AUTO_TEST_CASE(execute_pop_rr_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x31; m.memory[0x0001] = 0x78; m.memory[0x0002] = 0xC0;  // LD SP, C078h
    m.memory[0x0003] = 0xC1;                                                    // POP BC
    m.memory[0x0004] = 0xD1;                                                    // POP DE
    m.memory[0x0005] = 0xE1;                                                    // POP HL
    m.memory[0x0006] = 0xF1;                                                    // POP AF

    m.memory[0xC078] = 0x11; m.memory[0xC079] = 0xBB;
    m.memory[0xC07A] = 0x33; m.memory[0xC07B] = 0x22;
    m.memory[0xC07C] = 0x55; m.memory[0xC07D] = 0x44;
    m.memory[0xC07E] = 0xF5; m.memory[0xC07F] = 0x77;

    startZ80(z80);
    runCycles(z80, m, 50);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC080);
    BOOST_CHECK_EQUAL(m.memory[0xC078], z80.decoder.regs.bc.l);
    BOOST_CHECK_EQUAL(m.memory[0xC079], z80.decoder.regs.bc.h);
    BOOST_CHECK_EQUAL(m.memory[0xC07A], z80.decoder.regs.de.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07B], z80.decoder.regs.de.h);
    BOOST_CHECK_EQUAL(m.memory[0xC07C], z80.decoder.regs.hl.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07D], z80.decoder.regs.hl.h);
    BOOST_CHECK_EQUAL(m.memory[0xC07E], z80.decoder.regs.af.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07F], z80.decoder.regs.af.h);
}

BOOST_AUTO_TEST_CASE(execute_push_ix_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xDD; m.memory[0x0001] = 0x21;
    m.memory[0x0002] = 0x55; m.memory[0x0003] = 0x44;  // LD IX, 4455h
    m.memory[0x0004] = 0x31;
    m.memory[0x0005] = 0x80; m.memory[0x0006] = 0xC0;  // LD SP, C080h
    m.memory[0x0007] = 0xDD; m.memory[0x0008] = 0xE5;  // PUSH IX

    startZ80(z80);
    runCycles(z80, m, 39);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC07E);
    BOOST_CHECK_EQUAL(m.memory[0xC07E], z80.decoder.regs.ix.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07F], z80.decoder.regs.ix.h);
}

BOOST_AUTO_TEST_CASE(execute_push_iy_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xFD; m.memory[0x0001] = 0x21;
    m.memory[0x0002] = 0x55; m.memory[0x0003] = 0x44;  // LD IY, 4455h
    m.memory[0x0004] = 0x31;
    m.memory[0x0005] = 0x80; m.memory[0x0006] = 0xC0;  // LD SP, C080h
    m.memory[0x0007] = 0xFD; m.memory[0x0008] = 0xE5;  // PUSH IY

    startZ80(z80);
    runCycles(z80, m, 39);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC07E);
    BOOST_CHECK_EQUAL(m.memory[0xC07E], z80.decoder.regs.iy.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07F], z80.decoder.regs.iy.h);
}

BOOST_AUTO_TEST_CASE(execute_pop_ix_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x31; m.memory[0x0001] = 0x7E; m.memory[0x0002] = 0xC0;  // LD SP, C07Eh
    m.memory[0x0003] = 0xDD; m.memory[0x0004] = 0xE1;                           // POP IX

    m.memory[0xC07E] = 0x98; m.memory[0xC07F] = 0x77;

    startZ80(z80);
    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC080);
    BOOST_CHECK_EQUAL(m.memory[0xC07E], z80.decoder.regs.ix.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07F], z80.decoder.regs.ix.h);
}

BOOST_AUTO_TEST_CASE(execute_pop_iy_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x31; m.memory[0x0001] = 0x7E; m.memory[0x0002] = 0xC0;  // LD SP, C07Eh
    m.memory[0x0003] = 0xFD; m.memory[0x0004] = 0xE1;                           // POP IY

    m.memory[0xC07E] = 0x98; m.memory[0xC07F] = 0x77;

    startZ80(z80);
    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC080);
    BOOST_CHECK_EQUAL(m.memory[0xC07E], z80.decoder.regs.iy.l);
    BOOST_CHECK_EQUAL(m.memory[0xC07F], z80.decoder.regs.iy.h);
}

BOOST_AUTO_TEST_CASE(execute_ex_de_hl_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x11; m.memory[0x0001] = 0x78; m.memory[0x0002] = 0x56;  // LD DE, 5678h
    m.memory[0x0003] = 0x21; m.memory[0x0004] = 0x34; m.memory[0x0005] = 0x12;  // LD HL, 1234h
    m.memory[0x0006] = 0xEB;                                                    // EX DE, HL

    startZ80(z80);
    runCycles(z80, m, 24);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x5678);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x1234);
}

BOOST_AUTO_TEST_CASE(execute_ex_af_af_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x08;    // EX AF, AF'

    startZ80(z80);
    z80.decoder.regs.af.w = 0x1234;
    z80.decoder.regs.af_.w = 0x5678;
    runCycles(z80, m, 4);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x5678);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af_.w, 0x1234);
}

BOOST_AUTO_TEST_CASE(execute_exx_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0xD9;    // EXX

    startZ80(z80);
    z80.decoder.regs.bc_.w = 0x1234;
    z80.decoder.regs.de_.w = 0x5678;
    z80.decoder.regs.hl_.w = 0x9ABC;
    z80.decoder.regs.bc.w = 0x1122;
    z80.decoder.regs.de.w = 0x3344;
    z80.decoder.regs.hl.w = 0x5566;
    runCycles(z80, m, 4);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x1234);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x5678);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x9ABC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc_.w, 0x1122);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de_.w, 0x3344);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl_.w, 0x5566);
}

BOOST_AUTO_TEST_CASE(execute_ex_memsp_hl_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x31; m.memory[0x0001] = 0x00; m.memory[0x0002] = 0xC0;  // LD SP, C000h
    m.memory[0x0003] = 0x01; m.memory[0x0004] = 0x34; m.memory[0x0005] = 0x12;  // LD BC, 1234h
    m.memory[0x0006] = 0xC5;                                                    // PUSH BC
    m.memory[0x0007] = 0x21; m.memory[0x0008] = 0x78; m.memory[0x0009] = 0x56;  // LD HL, 5678h
    m.memory[0x000A] = 0xE3;                                                    // EX (SP), HL
    m.memory[0x000B] = 0xC1;                                                    // POP BC

    startZ80(z80);
    runCycles(z80, m, 70);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x5678);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x1234);
    BOOST_CHECK_EQUAL(m.memory[0xBFFE], 0x78);
    BOOST_CHECK_EQUAL(m.memory[0xBFFF], 0x56);
}

BOOST_AUTO_TEST_CASE(execute_ex_memsp_ix_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x31; m.memory[0x0001] = 0x00; m.memory[0x0002] = 0xC0;  // LD SP, C000h
    m.memory[0x0003] = 0x01; m.memory[0x0004] = 0x34; m.memory[0x0005] = 0x12;  // LD BC, 1234h
    m.memory[0x0006] = 0xC5;                                                    // PUSH BC
    m.memory[0x0007] = 0xDD; m.memory[0x0008] = 0x21;
    m.memory[0x0009] = 0x78; m.memory[0x000A] = 0x56;                           // LD IX, 5678h
    m.memory[0x000B] = 0xDD; m.memory[0x000C] = 0xE3;                           // EX (SP), IX
    m.memory[0x000D] = 0xC1;                                                    // POP BC

    startZ80(z80);
    runCycles(z80, m, 78);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x5678);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x1234);
    BOOST_CHECK_EQUAL(m.memory[0xBFFE], 0x78);
    BOOST_CHECK_EQUAL(m.memory[0xBFFF], 0x56);
}

BOOST_AUTO_TEST_CASE(execute_ex_memsp_iy_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x31; m.memory[0x0001] = 0x00; m.memory[0x0002] = 0xC0;  // LD SP, C000h
    m.memory[0x0003] = 0x01; m.memory[0x0004] = 0x34; m.memory[0x0005] = 0x12;  // LD BC, 1234h
    m.memory[0x0006] = 0xC5;                                                    // PUSH BC
    m.memory[0x0007] = 0xFD; m.memory[0x0008] = 0x21;
    m.memory[0x0009] = 0x78; m.memory[0x000A] = 0x56;                           // LD IY, 5678h
    m.memory[0x000B] = 0xFD; m.memory[0x000C] = 0xE3;                           // EX (SP), IY
    m.memory[0x000D] = 0xC1;                                                    // POP BC

    startZ80(z80);
    runCycles(z80, m, 78);

    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xC000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x5678);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x1234);
    BOOST_CHECK_EQUAL(m.memory[0xBFFE], 0x78);
    BOOST_CHECK_EQUAL(m.memory[0xBFFF], 0x56);
}
// EOF
// vim: et:sw=4:ts=4
