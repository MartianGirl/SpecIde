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

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x0C;   // LD A, 0Ch
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xF3;   // LD B, F3h
    m.memory[0x0004] = 0x80;                            // ADD B    (FFh, 10101000)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x0C;   // LD A, 0Ch
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0xF4;   // LD C, F4h
    m.memory[0x0009] = 0x81;                            // ADD C    (00h, 01010001)
    // Test half carry
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x08;   // LD A, 08h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x28;   // LD D, 28h
    m.memory[0x000E] = 0x82;                            // ADD D    (30h, 00110000)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x10;   // LD E, 10h
    m.memory[0x0013] = 0x83;                            // ADD E    (8Fh, 10001100)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x80;   // LD A, 80h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x88;   // LD H, 88h
    m.memory[0x0018] = 0x84;                            // ADD H    (08h, 00001101)

    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
}

BOOST_AUTO_TEST_CASE(adc_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x0C;   // LD A, 0Ch
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xF2;   // LD B, F2h
    m.memory[0x0004] = 0x88;                            // ADC B    (FFh, 10101000)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x0C;   // LD A, 0Ch
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0xF3;   // LD C, F3h
    m.memory[0x0009] = 0x89;                            // ADC C    (00h, 01010001)
    // Test half carry
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x07;   // LD A, 07h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x28;   // LD D, 28h
    m.memory[0x000E] = 0x8A;                            // ADD D    (30h, 00110000)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x10;   // LD E, 10h
    m.memory[0x0013] = 0x8B;                            // ADD E    (90h, 10010100)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x80;   // LD A, 80h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x88;   // LD H, 88h
    m.memory[0x0018] = 0x8C;                            // ADD H    (09h, 00001101)

    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
}

BOOST_AUTO_TEST_CASE(sub_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x0C;   // LD A, 0Ch
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0x0E;   // LD B, 0Eh
    m.memory[0x0004] = 0x90;                            // SUB B    (FEh, 10101010)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x0C;   // LD A, 0Ch
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x0C;   // LD C, 0Ch
    m.memory[0x0009] = 0x91;                            // SUB C    (00h, 01010011)
    // Test half carry
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x58;   // LD A, 58h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x04;   // LD D, 04h
    m.memory[0x000E] = 0x92;                            // SUB D    (54h, 00010011)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x80;   // LD E, 80h
    m.memory[0x0013] = 0x93;                            // ADD E    (FFh, 10111110)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x80;   // LD A, 80h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x88;   // LD H, 88h
    m.memory[0x0018] = 0x8C;                            // ADD H    (09h, 00001101)

    startZ80(z80);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAA);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0053);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x5413);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
}
// EOF
// vim: et:sw=4:ts=4
