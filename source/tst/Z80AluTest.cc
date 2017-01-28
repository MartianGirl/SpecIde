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
    m.memory[0x0004] = 0x80;                            // ADD A, B (FFh, 10101000)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x0C;   // LD A, 0Ch
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0xF4;   // LD C, F4h
    m.memory[0x0009] = 0x81;                            // ADD A, C (00h, 01010001)
    // Test half carry
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x08;   // LD A, 08h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x28;   // LD D, 28h
    m.memory[0x000E] = 0x82;                            // ADD A, D (30h, 00110000)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x10;   // LD E, 10h
    m.memory[0x0013] = 0x83;                            // ADD A, E (8Fh, 10001100)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x80;   // LD A, 80h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x88;   // LD H, 88h
    m.memory[0x0018] = 0x84;                            // ADD A, H (08h, 00001101)
    // Test 0x80
    m.memory[0x0019] = 0x3E; m.memory[0x001A] = 0x00;   // LD A, 00h
    m.memory[0x001B] = 0x2E; m.memory[0x001C] = 0x80;   // LD L, 80h
    m.memory[0x001D] = 0x85;                            // ADD A, L

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
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
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
    m.memory[0x000E] = 0x8A;                            // ADC A, D (30h, 00110000)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x10;   // LD E, 10h
    m.memory[0x0013] = 0x8B;                            // ADC A, E (90h, 10010100)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x80;   // LD A, 80h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x88;   // LD H, 88h
    m.memory[0x0018] = 0x8C;                            // ADC A, H (09h, 00001101)
    // Test 0x80
    m.memory[0x0019] = 0x3E; m.memory[0x001A] = 0x00;   // LD A, 00h
    m.memory[0x001B] = 0x2E; m.memory[0x001C] = 0x7F;   // LD L, 7Fh
    m.memory[0x001D] = 0x8D;                            // ADC A, L (80h, 10000000)

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
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
}

BOOST_AUTO_TEST_CASE(sub_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x0C;   // LD A, 0Ch
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0x0E;   // LD B, 0Eh
    m.memory[0x0004] = 0x90;                            // SUB B (FEh, 10111011)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x0C;   // LD A, 0Ch
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x0C;   // LD C, 0Ch
    m.memory[0x0009] = 0x91;                            // SUB C (00h, 01000010)
    // Test half carry
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x50;   // LD A, 50h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x08;   // LD D, 08h
    m.memory[0x000E] = 0x92;                            // SUB D (48h, 00011010)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x81;   // LD E, 81h
    m.memory[0x0013] = 0x93;                            // SUB E (FEh, 10101111)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x81;   // LD A, 81h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x02;   // LD H, 02h
    m.memory[0x0018] = 0x94;                            // SUB H (7Fh, 00111110)
    // Test 0x80
    m.memory[0x0019] = 0x3E; m.memory[0x001A] = 0x00;   // LD A, 00h
    m.memory[0x001B] = 0x2E; m.memory[0x001C] = 0x80;   // LD L, 80h
    m.memory[0x001D] = 0x95;                            // SUB L (80h, 10000111)

    startZ80(z80);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sbc_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x0C;   // LD A, 0Ch
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0x0D;   // LD B, 0Dh
    m.memory[0x0004] = 0x98;                            // SBC B (FEh, 10111011)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x0C;   // LD A, 0Ch
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x0B;   // LD C, 0Bh
    m.memory[0x0009] = 0x99;                            // SBC C (00h, 01000010)
    // Test half carry
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x50;   // LD A, 50h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x07;   // LD D, 07h
    m.memory[0x000E] = 0x9A;                            // SBC D (48h, 00011010)
    // Test overflow
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x7F;   // LD A, 7Fh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x80;   // LD E, 80h
    m.memory[0x0013] = 0x9B;                            // SBC E (FEh, 10101111)
    // Test carry
    m.memory[0x0014] = 0x3E; m.memory[0x0015] = 0x81;   // LD A, 81h
    m.memory[0x0016] = 0x26; m.memory[0x0017] = 0x03;   // LD H, 03h
    m.memory[0x0018] = 0x9C;                            // SBC H (7Dh, 00111110)
    // Test 0x80
    m.memory[0x0019] = 0x3E; m.memory[0x001A] = 0x00;   // LD A, 00h
    m.memory[0x001B] = 0x2E; m.memory[0x001C] = 0x80;   // LD L, 80h
    m.memory[0x001D] = 0x9D;                            // SBC L (7Fh, 00111011)

    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(and_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xF3;   // LD A, F3h
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xA5;   // LD B, A5h
    m.memory[0x0004] = 0xA0;                            // AND B (A1h, 10110000)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x76;   // LD A, 76h
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x88;   // LD C, 88h
    m.memory[0x0009] = 0xA1;                            // AND C (00h, 01010100)
    // Test odd parity
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x2B;   // LD A, 2Bh
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x7E;   // LD D, 7Eh
    m.memory[0x000E] = 0xA2;                            // AND D (2Ah, 00111000)
    // Test even parity
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x2B;   // LD A, 2Bh
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x7C;   // LD E, 7Ch
    m.memory[0x0013] = 0xA3;                            // AND E (28h, 00111100)

    startZ80(z80);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(or_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xF3;   // LD A, F3h
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xA5;   // LD B, A5h
    m.memory[0x0004] = 0xA8;                            // OR B (F7h, 10100000)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x00;   // LD A, 00h
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x00;   // LD C, 00h
    m.memory[0x0009] = 0xA9;                            // OR C (00h, 01000100)
    // Test odd parity
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x38;   // LD A, 38h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x04;   // LD D, 04h
    m.memory[0x000E] = 0xAA;                            // OR D (3Ch, 00101100)
    // Test even parity
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x38;   // LD A, 38h
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x10;   // LD E, 18h
    m.memory[0x0013] = 0xAB;                            // OR E (38h, 00101000)

    startZ80(z80);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(xor_r_test)
{
    Z80 z80;
    Memory m(16, false);

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xF3;   // LD A, F3h
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0x35;   // LD B, A5h
    m.memory[0x0004] = 0xB0;                            // XOR B (C6h, 10000100)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x43;   // LD A, 43h
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x43;   // LD C, 43h
    m.memory[0x0009] = 0xB1;                            // XOR C (00h, 01000100)
    // Test odd parity
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x76;   // LD A, 76h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x5C;   // LD D, 5Ch
    m.memory[0x000E] = 0xB2;                            // XOR D (2Ah, 00101000)
    // Test even parity
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x76;   // LD A, 76h
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x58;   // LD E, 58h
    m.memory[0x0013] = 0xB3;                            // XOR E (2Eh, 00101100)

    startZ80(z80);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}
// EOF
// vim: et:sw=4:ts=4
