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
        m.as_ = (z80.c & SIGNAL_MREQ_) == SIGNAL_MREQ_;
        m.rd_ = (z80.c & SIGNAL_RD_) == SIGNAL_RD_;
        m.wr_ = (z80.c & SIGNAL_WR_) == SIGNAL_WR_;
        m.clock();
        z80.d = m.d;
    }
}

void loadBinary(string const& code, Memory& m, size_t addrbase)
{
    stringstream ss;
    uint16_t byte;

    for (size_t i = 0; i != code.size(); i += 2)
    {
        ss << code.substr(i, 2) << endl;
        ss >> hex >> byte;

        m.memory[addrbase + (i / 2)] = static_cast<uint8_t>(byte);
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
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
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
    BOOST_CHECK(z80.a == 0xFFFF);
    BOOST_CHECK(z80.d == 0xFF);
    BOOST_CHECK(z80.c == 0xFFFF);

    // Clock it once. We've run ST_OCF_T1_ADDRWR.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T2_DATARD);
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
    BOOST_CHECK(z80.a == 0x0000);
    BOOST_CHECK(z80.d == 0xFF);
    BOOST_CHECK(z80.c == (0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_)));

    // Clock it once. We've run ST_OCF_T2_DATARD.
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T3_RFSH1);
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
    BOOST_CHECK(z80.a == 0x0000);
    BOOST_CHECK(z80.d == 0xFF);
    BOOST_CHECK(z80.c == (0xFFFF & ~(SIGNAL_M1_ | SIGNAL_MREQ_ | SIGNAL_RD_)));

    // Clock it once. We've run ST_OCF_T3_RFSH1
    z80.clock();
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T4_RFSH2);
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
    BOOST_CHECK(z80.a == 0xFF7F);
    BOOST_CHECK(z80.d == 0xFF);
    BOOST_CHECK(z80.c == (0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RFSH_)));

    // Clock it once. We've run ST_OCF_T4_RFSH2
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
    BOOST_CHECK(z80.a == 0xFF7F);
    BOOST_CHECK(z80.d == 0xFF);
    BOOST_CHECK(z80.c == (0xFFFF & ~SIGNAL_RFSH_));
}

BOOST_AUTO_TEST_CASE(states_ld_r_n_test)
{
    Z80 z80;
    // LD r, n tests
    // LD A, n
    z80.reset();
    z80.clock();    // ST_RESET        -> ST_OCF_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0000);
    z80.clock();    // ST_OCF_T1_ADDRWR -> ST_OCF_T2_DATARD 
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    z80.clock();    // ST_OCF_T2_DATARD -> ST_OCF_T3_RFSH1
    z80.d = 0x3E;   // LD A, n
    z80.clock();    // ST_OCF_T3_RFSH1  -> ST_OCF_T4_RFSH2
    z80.clock();    // ST_OCF_T4_RFSH2  -> ST_MEMRD_T1_ADDRWR
    BOOST_CHECK(z80.state == Z80State::ST_MEMRD_T1_ADDRWR);
    z80.clock();    // ST_MEMRD_T1_ADDRWR -> ST_MEMRD_T2_WAITST
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0002);
    z80.clock();    // ST_MEMRD_T2_WAITST -> ST_MEMRD_T3_DATARD
    BOOST_CHECK(z80.state == Z80State::ST_MEMRD_T3_DATARD);
    z80.d = 0xA5;
    z80.clock();    // ST_MEMRD_T3_DATARD -> ST_OCF_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA5);

    // LD B, n
    z80.reset();
    z80.clock();    // ST_RESET        -> ST_OCF_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0000);
    z80.clock();    // ST_OCF_T1_ADDRWR -> ST_OCF_T2_DATARD 
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    z80.clock();    // ST_OCF_T2_DATARD -> ST_OCF_T3_RFSH1
    z80.d = 0x06;   // LD B, n
    z80.clock();    // ST_OCF_T3_RFSH1  -> ST_OCF_T4_RFSH2
    z80.clock();    // ST_OCF_T4_RFSH2  -> ST_MEMRD_T1_ADDRWR
    BOOST_CHECK(z80.state == Z80State::ST_MEMRD_T1_ADDRWR);
    z80.clock();    // ST_MEMRD_T1_ADDRWR -> ST_MEMRD_T2_WAITST
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0002);
    z80.clock();    // ST_MEMRD_T2_WAITST -> ST_MEMRD_T3_DATARD
    BOOST_CHECK(z80.state == Z80State::ST_MEMRD_T3_DATARD);
    z80.d = 0x34;
    z80.clock();    // ST_MEMRD_T3_DATARD -> ST_OCF_T1_ADDRWR
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x34);
}

BOOST_AUTO_TEST_CASE(ld_r_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code = 
        "3EB7"          // LD A, B7h
        "06B0"          // LD B, B0h
        "0EB1"          // LD C, B1h
        "16B2"          // LD D, B2h
        "1EB3"          // LD E, B3h
        "26B4"          // LD H, B4h
        "2EB5";         // LD L, B5h

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 56);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xB7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xB0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.l, 0xB1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.h, 0xB2);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.l, 0xB3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.h, 0xB4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.l, 0xB5);
}

BOOST_AUTO_TEST_CASE(ld_memhl_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "2640"          // LD H, 40h
        "2E60"          // LD L, 60h
        "3634"          // LD (HL), 34h
        "2622"          // LD H, 22h
        "2E23"          // LD L, 23h
        "3678";         // LD (HL), 78h

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_r_r_test)
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

BOOST_AUTO_TEST_CASE(ld_r_a_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E35"          // LD A, 35h
        "ED4F";         // LD R, A   

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    // LD A, nn
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x35);
    
    // LD R, A
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.prefix, PREFIX_ED);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
    runCycles(z80, m, 4);

    BOOST_CHECK(z80.state == Z80State::ST_CPU_T0_WAITST);
    runCycles(z80, m, 1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.l, 0x35);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(ld_i_a_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E43"          // LD A, 43h
        "ED47";         // LD I, A

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    // LD A, nn
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x43);
    
    // LD I, A
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.h, 0x43);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(ld_a_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3EAB"          // LD A, ABh
        "ED4F"          // LD R, A
        "ED5F";         // LD A, R

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    // LD A, ABh
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xAB);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
    
    // LD R, A
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.l, 0xAB);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);

    // LD A, R
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xAD);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA9);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(ld_a_i_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E00"          // LD A, 00h
        "ED47"          // LD I, A
        "ED57";         // LD A, I

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    // LD A, 00h
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
    
    // LD I, A
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ir.h, 0x00);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);

    // LD A, I
    runCycles(z80, m, 9);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x41);
    BOOST_CHECK(z80.state == Z80State::ST_OCF_T1_ADDRWR);
}

BOOST_AUTO_TEST_CASE(ld_rx_rx_test)
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

BOOST_AUTO_TEST_CASE(ld_ry_ry_test)
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

BOOST_AUTO_TEST_CASE(ld_rx_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD3ED7"        // LD A, D7h
        "DD06D0"        // LD B, D0h
        "DD0ED1"        // LD C, D1h
        "DD16D2"        // LD D, D2h
        "DD1ED3"        // LD E, D3h
        "DD26D4"        // LD IXh, D4h
        "DD2ED5";       // LD IXl, D5h

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_ry_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD3EF7"        // LD A, F7h
        "FD06F0"        // LD B, F0h
        "FD0EF1"        // LD C, F1h
        "FD16F2"        // LD D, F2h
        "FD1EF3"        // LD E, F3h
        "FD26F4"        // LD IYh, F4h
        "FD2EF5";       // LD IYl, F5h

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_r_memhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "2601"          // LD H, 01h
        "2E07"          // LD L, 07h
        "7E"            // LD A, (HL)
        "2E00"          // LD L, 00h
        "46"            // LD B, (HL)
        "2E01"          // LD L, 01h
        "4E"            // LD C, (HL)
        "2E02"          // LD L, 02h
        "56"            // LD D, (HL)
        "2E03"          // LD L, 03h
        "5E"            // LD E, (HL)
        "2E04"          // LD L, 04h
        "66"            // LD H, (HL)
        "2E05"          // LD L, 05h
        "6E";           // LD L, (HL)

    string data1 = "B0C1D2E3440000A7";
    string data2 = "15";

    loadBinary(code, m, 0x0000);
    loadBinary(data1, m, 0x0100);
    loadBinary(data2, m, 0x4405);
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

BOOST_AUTO_TEST_CASE(ld_r_memix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD2601"        // LD IXh, 01h
        "DD2E04"        // LD IXl, 04h
        "DD7E03"        // LD A, (IX + 3)
        "DD46FC"        // LD B, (IX - 4)
        "DD4EFD"        // LD C, (IX - 3)
        "DD56FE"        // LD D, (IX - 2)
        "DD5EFF"        // LD E, (IX - 1)
        "DD6600"        // LD H, (IX + 0)
        "DD6E01";       // LD L, (IX + 1)

    string data = "B0C1D2E3441500A7";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 155);

    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4415);
}

BOOST_AUTO_TEST_CASE(ld_r_memiy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD2601"        // LD IYh, 01h
        "FD2E04"        // LD IYl, 04h
        "FD7E03"        // LD A, (IY + 3)
        "FD46FC"        // LD B, (IY - 4)
        "FD4EFD"        // LD C, (IY - 3)
        "FD56FE"        // LD D, (IY - 2)
        "FD5EFF"        // LD E, (IY - 1)
        "FD6600"        // LD H, (IY + 0)
        "FD6E01";       // LD L, (IY + 1)

    string data = "B0C1D2E3441500A7";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 155);

    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2E3);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4415);
}

BOOST_AUTO_TEST_CASE(ld_memhl_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3EA7"          // LD A, A7h
        "06B0"          // LD B, B0h
        "0EC1"          // LD C, C1h
        "16D2"          // LD D, D2h
        "1EE3"          // LD E, E3h
        "2601"          // LD H, 01h
        "2E07"          // LD L, 07h
        "77"            // LD (HL), A 
        "2E00"          // LD L, 00h
        "70"            // LD (HL), B
        "2E01"          // LD L, 01h
        "71"            // LD (HL), C
        "2E02"          // LD L, 02h
        "72"            // LD (HL), D
        "2E03"          // LD L, 03h
        "73"            // LD (HL), E
        "2E04"          // LD L, 04h
        "74"            // LD (HL), H
        "2E05"          // LD L, 05h
        "75";           // LD (HL), L

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_memix_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD2601"        // LD IXh, 01h
        "DD2E04"        // LD IXl, 04h
        "3EA7"          // LD A, A7h
        "06B0"          // LD B, B0h
        "0EC1"          // LD C, C1h
        "16D2"          // LD D, D2h
        "1EE3"          // LD E, E3h
        "2644"          // LD H, 44h
        "2E15"          // LD L, 15h
        "DD7703"        // LD (IX + 3), A
        "DD70FC"        // LD (IX - 4), B
        "DD71FD"        // LD (IX - 3), C
        "DD72FE"        // LD (IX - 2), D
        "DD73FF"        // LD (IX - 1), E
        "DD7400"        // LD (IX + 0), H
        "DD7501";       // LD (IX + 1), L

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_memiy_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD2601"        // LD IYh, 01h
        "FD2E04"        // LD IYl, 04h
        "3EA7"          // LD A, A7h
        "06B0"          // LD B, B0h
        "0EC1"          // LD C, C1h
        "16D2"          // LD D, D2h
        "1EE3"          // LD E, E3h
        "2644"          // LD H, 44h
        "2E15"          // LD L, 15h
        "FD7703"        // LD (IY + 3), A
        "FD70FC"        // LD (IY - 4), B
        "FD71FD"        // LD (IY - 3), C
        "FD72FE"        // LD (IY - 2), D
        "FD73FF"        // LD (IY - 1), E
        "FD7400"        // LD (IY + 0), H
        "FD7501";       // LD (IY + 1), L

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_memix_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD2601"        // LD IXh, 01h
        "DD2E04"        // LD IXl, 04h
        "DD36FCF0"      // LD (IX - 4), F0h
        "DD36FDF1"      // LD (IX - 3), F1h
        "DD36FEF2"      // LD (IX - 2), F2h
        "DD36FFF3"      // LD (IX - 1), F3h
        "DD3600F4"      // LD (IX + 0), F4h
        "DD3601F5"      // LD (IX + 1), F5h
        "DD3602F6"      // LD (IX + 2), F6h
        "DD3603F7";     // LD (IX + 3), F7h

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_memiy_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD2601"        // LD IYh, 01h
        "FD2E04"        // LD IYl, 04h
        "FD36FCF0"      // LD (IY - 4), F0h
        "FD36FDF1"      // LD (IY - 3), F1h
        "FD36FEF2"      // LD (IY - 2), F2h
        "FD36FFF3"      // LD (IY - 1), F3h
        "FD3600F4"      // LD (IY + 0), F4h
        "FD3601F5"      // LD (IY + 1), F5h
        "FD3602F6"      // LD (IY + 2), F6h
        "FD3603F7";     // LD (IY + 3), F7h

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(ld_a_membc_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0610"          // LD B, 0x10
        "0E20"          // LD C, 0x20
        "0A";           // LD A, (BC)

    m.memory[0x1020] = 0x56;

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 21);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x1020]);
}

BOOST_AUTO_TEST_CASE(ld_a_memde_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "1610"          // LD D, 0x10
        "1E20"          // LD E, 0x20
        "1A";           // LD A, (BC)

    m.memory[0x1020] = 0x78;

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 21);

    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x1020]);
}

BOOST_AUTO_TEST_CASE(ld_a_memnn_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3A2010";       // LD A, (1020h)

    m.memory[0x1020] = 0x78;

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 13);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, m.memory[0x1020]);
}

BOOST_AUTO_TEST_CASE(ld_membc_a_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0610"          // LD B, 0x10
        "0E20"          // LD C, 0x20
        "3E23"          // LD A, 0x23
        "02";           // LD (BC), A

    m.memory[0x1020] = 0x56;

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 28);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x23);
    BOOST_CHECK_EQUAL(m.memory[0x1020], 0x23);
}

BOOST_AUTO_TEST_CASE(ld_memde_a_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "1610"          // LD D, 0x10
        "1E20"          // LD E, 0x20
        "3E48"          // LD A, 0x48
        "12";           // LD (DE), A

    m.memory[0x1020] = 0x56;

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 28);

    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x1020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x48);
    BOOST_CHECK_EQUAL(m.memory[0x1020], 0x48);
}  

BOOST_AUTO_TEST_CASE(ld_memnn_a_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E69"          // LD A, 69h
        "322010";       // LD (1020h), A

    m.memory[0x1020] = 0x78;

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 20);

    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x69);
    BOOST_CHECK_EQUAL(m.memory[0x1020], 0x69);
}

BOOST_AUTO_TEST_CASE(ld_rp_nn_test)
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

BOOST_AUTO_TEST_CASE(ld_rpx_nn_test)
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

BOOST_AUTO_TEST_CASE(ld_rpy_nn_test)
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

BOOST_AUTO_TEST_CASE(ld_hl_memword_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x2A; m.memory[0x0001] = 0x34; m.memory[0x0002] = 0x12;  // LD HL, (1234h)
    
    m.memory[0x1234] = 0x11; m.memory[0x1235] = 0x44;

    startZ80(z80);
    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x4411);
}

BOOST_AUTO_TEST_CASE(ld_reg_memword_test)
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

BOOST_AUTO_TEST_CASE(ld_ix_memword_test)
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

BOOST_AUTO_TEST_CASE(ld_iy_memword_test)
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

BOOST_AUTO_TEST_CASE(ld_memword_hl_test)
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

BOOST_AUTO_TEST_CASE(ld_memword_reg_test)
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

BOOST_AUTO_TEST_CASE(ld_memword_ix_test)
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

BOOST_AUTO_TEST_CASE(ld_memword_iy_test)
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

BOOST_AUTO_TEST_CASE(ld_sp_hl_test)
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

BOOST_AUTO_TEST_CASE(ld_sp_ix_test)
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

BOOST_AUTO_TEST_CASE(ld_sp_iy_test)
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

BOOST_AUTO_TEST_CASE(push_rr_test)
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

BOOST_AUTO_TEST_CASE(pop_rr_test)
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

BOOST_AUTO_TEST_CASE(push_ix_test)
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

BOOST_AUTO_TEST_CASE(push_iy_test)
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

BOOST_AUTO_TEST_CASE(pop_ix_test)
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

BOOST_AUTO_TEST_CASE(pop_iy_test)
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

BOOST_AUTO_TEST_CASE(ex_de_hl_test)
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

BOOST_AUTO_TEST_CASE(ex_af_af_test)
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

BOOST_AUTO_TEST_CASE(exx_test)
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

BOOST_AUTO_TEST_CASE(ex_memsp_hl_test)
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

BOOST_AUTO_TEST_CASE(ex_memsp_ix_test)
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

BOOST_AUTO_TEST_CASE(ex_memsp_iy_test)
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

BOOST_AUTO_TEST_CASE(ldi_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x11; m.memory[0x0001] = 0x00; m.memory[0x0002] = 0x40;  // LD DE, 4000h
    m.memory[0x0003] = 0x21; m.memory[0x0004] = 0x00; m.memory[0x0005] = 0x80;  // LD HL, 8000h
    m.memory[0x0006] = 0x01; m.memory[0x0007] = 0x03; m.memory[0x0008] = 0x00;  // LD BC, 0003h
    m.memory[0x0009] = 0xED; m.memory[0x000A] = 0xA0;                           // LDI
    m.memory[0x000B] = 0xED; m.memory[0x000C] = 0xA0;                           // LDI
    m.memory[0x000D] = 0xED; m.memory[0x000E] = 0xA0;                           // LDI

    m.memory[0x8000] = 0x01; m.memory[0x8001] = 0x23; m.memory[0x8002] = 0x45;

    startZ80(z80);
    z80.decoder.regs.af.w = 0x0800;   // Clear flags
    runCycles(z80, m, 46);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8001);
    BOOST_CHECK_EQUAL(m.memory[0x4000], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x0C);

    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8002);
    BOOST_CHECK_EQUAL(m.memory[0x4001], 0x23);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x2C);
    
    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4003);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8003);
    BOOST_CHECK_EQUAL(m.memory[0x4002], 0x45);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
}

BOOST_AUTO_TEST_CASE(ldir_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x01; m.memory[0x0001] = 0x80; m.memory[0x0002] = 0x02;  // LD BC, 0280h
    m.memory[0x0003] = 0x11; m.memory[0x0004] = 0x00; m.memory[0x0005] = 0x40;  // LD DE, 4000h
    m.memory[0x0006] = 0x21; m.memory[0x0007] = 0x00; m.memory[0x0008] = 0xC0;  // LD HL, C000h
    m.memory[0x0009] = 0x3E; m.memory[0x000A] = 0x20;                           // LD A, 20h
    m.memory[0x000B] = 0xED; m.memory[0x000C] = 0xB0;                           // LDIR

    for (size_t i = 0; i != 0x280; ++i)
        m.memory[0xC000 + i] = i % 0x100;

    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 37);

    for (size_t i = 0; i != 0x27F; ++i)
    {
        runCycles(z80, m, 21);

        BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0280 - i - 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4000 + i + 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC000 + i + 1);
        BOOST_CHECK_EQUAL(m.memory[0x4000 + i], i % 0x100);
        BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 
                (((i + 0x20) % 0x100) & 0x08) 
                | ((((i + 0x20) % 0x100) & 0x02) << 4) 
                | 0x04);
    }

    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4280);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC280);
    BOOST_CHECK_EQUAL(m.memory[0x427F], 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x28);
}

BOOST_AUTO_TEST_CASE(ldd_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x11; m.memory[0x0001] = 0x02; m.memory[0x0002] = 0x40;  // LD DE, 4002h
    m.memory[0x0003] = 0x21; m.memory[0x0004] = 0x02; m.memory[0x0005] = 0x80;  // LD HL, 8002h
    m.memory[0x0006] = 0x01; m.memory[0x0007] = 0x03; m.memory[0x0008] = 0x00;  // LD BC, 0003h
    m.memory[0x0009] = 0xED; m.memory[0x000A] = 0xA8;                           // LDD
    m.memory[0x000B] = 0xED; m.memory[0x000C] = 0xA8;                           // LDD
    m.memory[0x000D] = 0xED; m.memory[0x000E] = 0xA8;                           // LDD

    m.memory[0x8000] = 0x01; m.memory[0x8001] = 0x23; m.memory[0x8002] = 0x45;

    startZ80(z80);
    z80.decoder.regs.af.w = 0x0800;   // Clear flags
    runCycles(z80, m, 46);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8001);
    BOOST_CHECK_EQUAL(m.memory[0x4002], 0x45);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x0C);

    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x4000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8000);
    BOOST_CHECK_EQUAL(m.memory[0x4001], 0x23);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x2C);
    
    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x3FFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x7FFF);
    BOOST_CHECK_EQUAL(m.memory[0x4000], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
}

BOOST_AUTO_TEST_CASE(lddr_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x01; m.memory[0x0001] = 0x80; m.memory[0x0002] = 0x02;  // LD BC, 0280h
    m.memory[0x0003] = 0x11; m.memory[0x0004] = 0x7F; m.memory[0x0005] = 0x42;  // LD DE, 427Fh
    m.memory[0x0006] = 0x21; m.memory[0x0007] = 0x7F; m.memory[0x0008] = 0xC2;  // LD HL, C27Fh
    m.memory[0x0009] = 0x3E; m.memory[0x000A] = 0x20;                           // LD A, 20h
    m.memory[0x000B] = 0xED; m.memory[0x000C] = 0xB8;                           // LDDR

    for (size_t i = 0; i != 0x280; ++i)
        m.memory[0xC000 + i] = i % 0x100;

    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 37);

    for (size_t i = 0; i != 0x27F; ++i)
    {
        runCycles(z80, m, 21);

        BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0280 - i - 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x427F - i - 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC27F - i - 1);
        BOOST_CHECK_EQUAL(m.memory[0x427F - i], (0x27F - i) % 0x100);
        BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 
                ((((0x27F - i) + 0x20) % 0x100) & 0x08) 
                | (((((0x27F - i) + 0x20) % 0x100) & 0x02) << 4)
                | 0x04);
    }

    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x3FFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xBFFF);
    BOOST_CHECK_EQUAL(m.memory[0x4000], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(cpi_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210080"        // LD HL, 8000h
        "010300"        // LD BC, 0003h
        "EDA1"          // CPI
        "EDA1"          // CPI
        "EDA1";         // CPI

    string data = "012345";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x8000);

    startZ80(z80);
    z80.decoder.regs.af.w = 0x2300;   // Clear flags

    runCycles(z80, m, 36);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x26);

    runCycles(z80, m, 16);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x46);
    
    runCycles(z80, m, 16);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8003);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x9A);
}

BOOST_AUTO_TEST_CASE(cpir_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "018000"        // LD BC, 0080h
        "2100C0"        // LD HL, C000h
        "3E20"          // LD A, 20h
        "EDB1";         // CPIR

    loadBinary(code, m, 0x0000);

    for (size_t i = 0; i != 0x80; ++i)
        m.memory[0xC000 + i] = static_cast<uint8_t>(i);

    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 27);

    for (size_t i = 0; i != 0x20; ++i)
    {
        runCycles(z80, m, 21);

        uint8_t s = static_cast<uint8_t>(i);
        uint8_t h = (-(s & 0x0F) & 0x10);
        uint8_t n = 0x20 - s - (h >> 4);
        uint8_t f = h | (n & 0x08) | ((n << 4) & 0x20) | 0x06;

        BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0080 - i - 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC000 + i + 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, f);
    }

    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x005F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC021);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x46);
}

BOOST_AUTO_TEST_CASE(cpd_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210280"        // LD HL, 8002h
        "010300"        // LD BC, 0003h
        "EDA9"          // CPD
        "EDA9"          // CPD
        "EDA9";         // CPD

    string data = "012345";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x8000);
    startZ80(z80);
    z80.decoder.regs.af.w = 0x2300;   // Clear flags

    runCycles(z80, m, 36);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x9E);

    runCycles(z80, m, 16);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x8000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x46);
    
    runCycles(z80, m, 16);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x7FFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x22);
}

BOOST_AUTO_TEST_CASE(cpdr_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "018000"        // LD BC, 0080h
        "217FC0"        // LD HL, C07Fh
        "3E20"          // LD A, 20h
        "EDB9";         // CPDR

    loadBinary(code, m, 0x0000);

    for (size_t i = 0; i != 0x80; ++i)
        m.memory[0xC000 + i] = static_cast<uint8_t>(i);

    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 27);

    for (size_t i = 0; i != 0x5F; ++i)
    {
        runCycles(z80, m, 21);

        uint8_t s = static_cast<uint8_t>(0x7F - i);
        uint8_t h = (-(s & 0x0F) & 0x10);
        uint8_t n = 0x20 - s - (h >> 4);
        uint8_t f = 0x80 | h | (n & 0x08) | ((n << 4) & 0x20) | 0x06;

        BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0080 - i - 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC07F - i - 1);
        BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, f);
    }

    runCycles(z80, m, 16);

    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0020);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xC01F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x46);
}

BOOST_AUTO_TEST_CASE(ld_r_memix_states_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD2601"        // LD IXh, 01h
        "DD2E04"        // LD IXl, 04h
        "DD7E03"        // LD A, (IX + 3)
        "DD46FC"        // LD B, (IX - 4)
        "DD4EFD"        // LD C, (IX - 3)
        "DD56FE"        // LD D, (IX - 2)
        "67";           // LD H, A

    string data = "B0C1D2E3441500A7";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);

    runCycles(z80, m, 98);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);

    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xA7FF);
}

BOOST_AUTO_TEST_CASE(ld_r_memiy_states_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD2601"        // LD IYh, 01h
        "FD2E04"        // LD IYl, 04h
        "FD7E03"        // LD A, (IY + 3)
        "FD46FC"        // LD B, (IY - 4)
        "FD4EFD"        // LD C, (IY - 3)
        "FD56FE"        // LD D, (IY - 2)
        "67";           // LD H, A

    string data = "B0C1D2E3441500A7";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);

    runCycles(z80, m, 98);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);

    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xA7FF);
}

BOOST_AUTO_TEST_CASE(ld_memix_r_states_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD2601"        // LD IXh, 01h
        "DD2E04"        // LD IXl, 04h
        "3EA7"          // LD A, A7h
        "06B0"          // LD B, B0h
        "0EC1"          // LD C, C1h
        "16D2"          // LD D, D2h
        "DD7703"        // LD (IX + 3), A
        "DD70FC"        // LD (IX - 4), B
        "DD71FD"        // LD (IX - 3), C
        "DD72FE"        // LD (IX - 2), D
        "67";           // LD H, A

    loadBinary(code, m, 0x0000);
    startZ80(z80);

    runCycles(z80, m, 126);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xA7);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xB0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xC1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xD2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0x00);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x00);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x00);

    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xA7FF);
}

BOOST_AUTO_TEST_CASE(ld_memiy_r_states_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD2601"        // LD IYh, 01h
        "FD2E04"        // LD IYl, 04h
        "3EA7"          // LD A, A7h
        "06B0"          // LD B, B0h
        "0EC1"          // LD C, C1h
        "16D2"          // LD D, D2h
        "FD7703"        // LD (IY + 3), A
        "FD70FC"        // LD (IY - 4), B
        "FD71FD"        // LD (IY - 3), C
        "FD72FE"        // LD (IY - 2), D
        "67";           // LD H, A

    loadBinary(code, m, 0x0000);
    startZ80(z80);

    runCycles(z80, m, 126);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0x0104);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xA7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xB0C1);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xD2FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0xA7);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xB0);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0xC1);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0xD2);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0x00);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x00);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x00);

    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xA7FF);
}
// EOF
// vim: et:sw=4:ts=4
