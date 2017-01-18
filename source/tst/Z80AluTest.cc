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

BOOST_AUTO_TEST_CASE(add_r_test)
{
    Z80 z80;
    Memory m(16, false);

    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x0C;   // LD A, 0Ch
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0x0C;   // LD B, 0Ch
    m.memory[0x0004] = 0x80;                            // ADD B    (18h, 00011000)
    m.memory[0x0005] = 0x0E; m.memory[0x0006] = 0x0B;   // LD C, 0Bh
    m.memory[0x0007] = 0x81;                            // ADD C    (23h, 00110000)
    m.memory[0x0008] = 0x16; m.memory[0x0009] = 0x68;   // LD D, 68h
    m.memory[0x000A] = 0x82;                            // ADD D    (8Bh, 10001100)
    m.memory[0x000B] = 0x1E; m.memory[0x000C] = 0x18;   // LD E, 18h
    m.memory[0x000D] = 0x83;                            // ADD E    (A3h, 10110000)
    m.memory[0x000E] = 0x26; m.memory[0x000F] = 0x5C;   // LD H, 5Ch
    m.memory[0x0010] = 0x84;                            // ADD H    (FFh, 10101000)
    m.memory[0x0011] = 0x87;                            // ADD A    (FEh, 10111001)
    m.memory[0x0012] = 0x2E; m.memory[0x0013] = 0x82;   // LD L, 82h
    m.memory[0x0014] = 0x85;                            // ADD L    (80h, 10010001)
    m.memory[0x0015] = 0x87;                            // ADD A    (00h, 01000101)

    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x1818);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2330);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8B8C);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA3B0);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEB9);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8091);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0045);
}

// EOF
// vim: et:sw=4:ts=4
