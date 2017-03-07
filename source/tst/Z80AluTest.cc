//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <sstream>
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

BOOST_AUTO_TEST_CASE(add_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "06F3"          // LD B, F3h
        "80"            // ADD A, B     (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "0EF4"          // LD C, F4h
        "81"            // ADD A, C     (00h, 01010001)
        // Test half carry
        "3E08"          // LD A, 08h
        "1628"          // LD D, 28h
        "82"            // ADD A, D     (30h, 00110000)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "1E10"          // LD E, 10h
        "83"            // ADD A, E     (8Fh, 10001100)
        // Test carry
        "3E80"          // LD A, 80h
        "2688"          // LD H, 88h
        "84"            // ADD A, H     (08h, 00001101)
        // Test 80h
        "3E00"          // LD A, 00h
        "2E80"          // LD L, 80h
        "85";           // ADD A, L     (80h, 10000000)

    loadBinary(code, m, 0x0000);
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

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "06F2"          // LD B, F2h
        "88"            // ADC B    (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "0EF3"          // LD C, F3h
        "89"            // ADC C    (00h, 01010001)
        // Test half carry
        "3E07"          // LD A, 07h
        "1628"          // LD D, 28h
        "8A"            // ADC A, D (30h, 00110000)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "1E10"          // LD E, 10h
        "8B"            // ADC A, E (90h, 10010100)
        // Test carry
        "3E80"          // LD A, 80h
        "2688"          // LD H, 88h
        "8C"            // ADC A, H (09h, 00001101)
        // Test 80h
        "3E00"          // LD A, 00h
        "2E7F"          // LD L, 7Fh
        "8D";           // ADC A, L (80h, 10000000)

    loadBinary(code, m, 0x0000);
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

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "060E"          // LD B, 0Eh
        "90"            // SUB B (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "0E0C"          // LD C, 0Ch
        "91"            // SUB C (00h, 01000010)
        // Test half carry
        "3E50"          // LD A, 50h
        "1608"          // LD D, 08h
        "92"            // SUB D (48h, 00011010)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "1E81"          // LD E, 81h
        "93"            // SUB E (FEh, 10101111)
        // Test carry
        "3E81"          // LD A, 81h
        "2602"          // LD H, 02h
        "94"            // SUB H (7Fh, 00111110)
        // Test 0x80
        "3E00"          // LD A, 00h
        "2E80"          // LD L, 80h
        "95";           // SUB L (80h, 10000111)

    loadBinary(code, m, 0x0000);
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

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "060D"          // LD B, 0Dh
        "98"            // SBC B (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "0E0B"          // LD C, 0Bh
        "99"            // SBC C (00h, 01000010)
        // Test half carry
        "3E50"          // LD A, 50h
        "1607"          // LD D, 07h
        "9A"            // SBC D (48h, 00011010)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "1E80"          // LD E, 80h
        "9B"            // SBC E (FEh, 10101111)
        // Test carry
        "3E81"          // LD A, 81h
        "2603"          // LD H, 03h
        "9C"            // SBC H (7Dh, 00111110)
        // Test 0x80
        "3E00"          // LD A, 00h
        "2E80"          // LD L, 80h
        "9D";           // SBC L (7Fh, 00111011)

    loadBinary(code, m, 0x0000);
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

    string code =
        // Test sign
        "3EF3"          // LD A, F3h
        "06A5"          // LD B, A5h
        "A0"            // AND B (A1h, 10110000)
        // Test zero
        "3E76"          // LD A, 76h
        "0E88"          // LD C, 88h
        "A1"            // AND C (00h, 01010100)
        // Test odd parity
        "3E2B"          // LD A, 2Bh
        "167E"          // LD D, 7Eh
        "A2"            // AND D (2Ah, 00111000)
        // Test even parity
        "3E2B"          // LD A, 2Bh
        "1E7C"          // LD E, 7Ch
        "A3";           // AND E (28h, 00111100)

    loadBinary(code, m, 0x0000);
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

    string code =
        // Test sign
        "3EF3"          // LD A, F3h
        "06A5"          // LD B, A5h
        "B0"            // OR B (F7h, 10100000)
        // Test zero
        "3E00"          // LD A, 00h
        "0E00"          // LD C, 00h
        "B1"            // OR C (00h, 01000100)
        // Test odd parity
        "3E38"          // LD A, 38h
        "1604"          // LD D, 04h
        "B2"            // OR D (3Ch, 00101100)
        // Test even parity
        "3E38"          // LD A, 38h
        "1E10"          // LD E, 18h
        "B3";           // OR E (38h, 00101000)

    loadBinary(code, m, 0x0000);
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

    string code =
        // Test sign
        "3EF3"          // LD A, F3h
        "0635"          // LD B, A5h
        "A8"            // XOR B (C6h, 10000100)
        // Test zero
        "3E43"          // LD A, 43h
        "0E43"          // LD C, 43h
        "A9"            // XOR C (00h, 01000100)
        // Test odd parity
        "3E76"          // LD A, 76h
        "165C"          // LD D, 5Ch
        "AA"            // XOR D (2Ah, 00101000)
        // Test even parity
        "3E76"          // LD A, 76h
        "1E58"          // LD E, 58h
        "AB";           // XOR E (2Eh, 00101100)

    loadBinary(code, m, 0x0000);
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

BOOST_AUTO_TEST_CASE(cp_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E73"          // LD A, 73h
        "06A5"          // LD B, A5h
        "B8"            // CP B (10110111)
        // Test zero
        "3E43"          // LD A, 43h
        "0E43"          // LD C, 43h
        "B9"            // CP C (01000010)
        // Test odd parity
        "3E76"          // LD A, 76h
        "165C"          // LD D, 5Ch
        "BA";           // CP D (00011010)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 18);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(inc_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3EFE"          // LD A, FEh
        "47"            // LD B, A
        "04"            // INC B (0xFF, 10101000)
        "48"            // LD C, B
        "0C"            // INC C (0x00, 01010000)
        "51"            // LD D, C
        "14"            // INC D (0x01, 00000000)
        "1E7F"          // LD E, 7Fh
        "1C";           // INC E (0x80, 10010100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEA8);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFF00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE50);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x01FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE00);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x0180);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE94);
}

BOOST_AUTO_TEST_CASE(dec_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E01"          // LD A, 01h
        "47"            // LD B, A
        "05"            // DEC B (0x00, 01000010)
        "48"            // LD C, B
        "0D"            // DEC C (0xFF, 10111010)
        "51"            // LD D, C
        "15"            // DEC D (0xFE, 10101010)
        "1E80"          // LD E, 80h
        "1D";           // DEC E (0x7F, 00111110)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x00FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0142);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x00FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x01BA);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFEFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x01AA);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFE7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x013E);
}

BOOST_AUTO_TEST_CASE(add_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3E0C"        // LD A, 0Ch
        "DD06F3"        // LD B, F3h
        "DD80"          // ADD A, B (FFh, 10101000)
        // Test zero
        "DD3E0C"        // LD A, 0Ch
        "DD0EF4"        // LD C, F4h
        "DD81"          // ADD A, C (00h, 01010001)
        // Test half carry
        "DD3E08"        // LD A, 08h
        "DD1628"        // LD D, 28h
        "DD82"          // ADD A, D (30h, 00110000)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DD1E10"        // LD E, 10h
        "DD83"          // ADD A, E (8Fh, 10001100)
        // Test carry
        "DD3E80"        // LD A, 80h
        "DD2688"        // LD IXh, 88h
        "DD84"          // ADD A, IXh (08h, 00001101)
        // Test 0x80
        "DD3E00"        // LD A, 00h
        "DD2E80"        // LD IXl, 80h
        "DD85";         // ADD A, IXl (80h, 10000000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
}

BOOST_AUTO_TEST_CASE(add_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3E0C"        // LD A, 0Ch
        "FD06F3"        // LD B, F3h
        "FD80"          // ADD A, B (FFh, 10101000)
        // Test zero
        "FD3E0C"        // LD A, 0Ch
        "FD0EF4"        // LD C, F4h
        "FD81"          // ADD A, C (00h, 01010001)
        // Test half carry
        "FD3E08"        // LD A, 08h
        "FD1628"        // LD D, 28h
        "FD82"          // ADD A, D (30h, 00110000)
        // Test overflow
        "FD3E7F"        // LD A, 7Fh
        "FD1E10"        // LD E, 10h
        "FD83"          // ADD A, E (8Fh, 10001100)
        // Test carry
        "FD3E80"        // LD A, 80h
        "FD2688"        // LD IXh, 88h
        "FD84"          // ADD A, IYh (08h, 00001101)
        // Test 0x80
        "FD3E00"        // LD A, 00h
        "FD2E80"        // LD IXl, 80h
        "FD85";         // ADD A, IYl (80h, 10000000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
}

BOOST_AUTO_TEST_CASE(adc_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3E0C"        // LD A, 0Ch
        "DD06F2"        // LD B, F2h
        "DD88"          // ADC B    (FFh, 10101000)
        // Test zero
        "DD3E0C"        // LD A, 0Ch
        "DD0EF3"        // LD C, F3h
        "DD89"          // ADC C    (00h, 01010001)
        // Test half carry
        "DD3E07"        // LD A, 07h
        "DD1628"        // LD D, 28h
        "DD8A"          // ADC A, D (30h, 00110000)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DD1E10"        // LD E, 10h
        "DD8B"          // ADC A, E (90h, 10010100)
        // Test carry
        "DD3E80"        // LD A, 80h
        "DD2688"        // LD H, 88h
        "DD8C"          // ADC A, H (09h, 00001101)
        // Test 0x80
        "DD3E00"        // LD A, 00h
        "DD2E7F"        // LD L, 7Fh
        "DD8D";         // ADC A, L (80h, 10000000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
}

BOOST_AUTO_TEST_CASE(adc_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3E0C"        // LD A, 0Ch
        "FD06F2"        // LD B, F2h
        "FD88"          // ADC B    (FFh, 10101000)
        // Test zero
        "FD3E0C"        // LD A, 0Ch
        "FD0EF3"        // LD C, F3h
        "FD89"          // ADC C    (00h, 01010001)
        // Test half carry
        "FD3E07"        // LD A, 07h
        "FD1628"        // LD D, 28h
        "FD8A"          // ADC A, D (30h, 00110000)
        // Test overflow
        "FD3E7F"        // LD A, 7Fh
        "FD1E10"        // LD E, 10h
        "FD8B"          // ADC A, E (90h, 10010100)
        // Test carry
        "FD3E80"        // LD A, 80h
        "FD2688"        // LD IYh, 88h
        "FD8C"          // ADC A, IYh (09h, 00001101)
        // Test 0x80
        "FD3E00"        // LD A, 00h
        "FD2E7F"        // LD IYl, 7Fh
        "FD8D";         // ADC A, IYl (80h, 10000000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
}

BOOST_AUTO_TEST_CASE(sub_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3E0C"        // LD A, 0Ch
        "DD060E"        // LD B, 0Eh
        "DD90"          // SUB B (FEh, 10111011)
        // Test zero
        "DD3E0C"        // LD A, 0Ch
        "DD0E0C"        // LD C, 0Ch
        "DD91"          // SUB C (00h, 01000010)
        // Test half carry
        "DD3E50"        // LD A, 50h
        "DD1608"        // LD D, 08h
        "DD92"          // SUB D (48h, 00011010)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DD1E81"        // LD E, 81h
        "DD93"          // SUB E (FEh, 10101111)
        // Test carry
        "DD3E81"        // LD A, 81h
        "DD2602"        // LD IXh, 02h
        "DD94"          // SUB IXh (7Fh, 00111110)
        // Test 0x80
        "DD3E00"        // LD A, 00h
        "DD2E80"        // LD IXl, 80h
        "DD95";         // SUB IXl (80h, 10000111)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sub_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3E0C"        // LD A, 0Ch
        "FD060E"        // LD B, 0Eh
        "FD90"          // SUB B (FEh, 10111011)
        // Test zero
        "FD3E0C"        // LD A, 0Ch
        "FD0E0C"        // LD C, 0Ch
        "FD91"          // SUB C (00h, 01000010)
        // Test half carry
        "FD3E50"        // LD A, 50h
        "FD1608"        // LD D, 08h
        "FD92"          // SUB D (48h, 00011010)
        // Test overflow
        "FD3E7F"        // LD A, 7Fh
        "FD1E81"        // LD E, 81h
        "FD93"          // SUB E (FEh, 10101111)
        // Test carry
        "FD3E81"        // LD A, 81h
        "FD2602"        // LD IYh, 02h
        "FD94"          // SUB IYh (7Fh, 00111110)
        // Test 0x80
        "FD3E00"        // LD A, 00h
        "FD2E80"        // LD IYh, 80h
        "FD95";         // SUB IYh (80h, 10000111)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sbc_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3E0C"        // LD A, 0Ch
        "DD060D"        // LD B, 0Dh
        "DD98"          // SBC B (FEh, 10111011)
        // Test zero
        "DD3E0C"        // LD A, 0Ch
        "DD0E0B"        // LD C, 0Bh
        "DD99"          // SBC C (00h, 01000010)
        // Test half carry
        "DD3E50"        // LD A, 50h
        "DD1607"        // LD D, 07h
        "DD9A"          // SBC D (48h, 00011010)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DD1E80"        // LD E, 80h
        "DD9B"          // SBC E (FEh, 10101111)
        // Test carry
        "DD3E81"        // LD A, 81h
        "DD2603"        // LD IXh, 03h
        "DD9C"          // SBC IXh (7Dh, 00111110)
        // Test 0x80
        "DD3E00"        // LD A, 00h
        "DD2E80"        // LD IXl, 80h
        "DD9D";         // SBC IXl (7Fh, 00111011)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(sbc_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3E0C"        // LD A, 0Ch
        "FD060D"        // LD B, 0Dh
        "FD98"          // SBC B (FEh, 10111011)
        // Test zero
        "FD3E0C"        // LD A, 0Ch
        "FD0E0B"        // LD C, 0Bh
        "FD99"          // SBC C (00h, 01000010)
        // Test half carry
        "FD3E50"        // LD A, 50h
        "FD1607"        // LD D, 07h
        "FD9A"          // SBC D (48h, 00011010)
        // Test overflow
        "FD3E7F"        // LD A, 7Fh
        "FD1E80"        // LD E, 80h
        "FD9B"          // SBC E (FEh, 10101111)
        // Test carry
        "FD3E81"        // LD A, 81h
        "FD2603"        // LD IYh, 03h
        "FD9C"          // SBC IYh (7Dh, 00111110)
        // Test 0x80
        "FD3E00"        // LD A, 00h
        "FD2E80"        // LD IYl, 80h
        "FD9D";         // SBC IYl (7Fh, 00111011)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(and_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3EF3"        // LD A, F3h
        "DD06A5"        // LD B, A5h
        "DDA0"          // AND B (A1h, 10110000)
        // Test zero
        "DD3E76"        // LD A, 76h
        "DD0E88"        // LD C, 88h
        "DDA1"          // AND C (00h, 01010100)
        // Test odd parity
        "DD3E2B"        // LD A, 2Bh
        "DD167E"        // LD D, 7Eh
        "DDA2"          // AND D (2Ah, 00111000)
        // Test even parity
        "DD3E2B"        // LD A, 2Bh
        "DD1E7C"        // LD E, 7Ch
        "DDA3";         // AND E (28h, 00111100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(and_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3EF3"        // LD A, F3h
        "FD06A5"        // LD B, A5h
        "FDA0"          // AND B (A1h, 10110000)
        // Test zero
        "FD3E76"        // LD A, 76h
        "FD0E88"        // LD C, 88h
        "FDA1"          // AND C (00h, 01010100)
        // Test odd parity
        "FD3E2B"        // LD A, 2Bh
        "FD167E"        // LD D, 7Eh
        "FDA2"          // AND D (2Ah, 00111000)
        // Test even parity
        "FD3E2B"        // LD A, 2Bh
        "FD1E7C"        // LD E, 7Ch
        "FDA3";         // AND E (28h, 00111100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(or_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3EF3"        // LD A, F3h
        "DD06A5"        // LD B, A5h
        "DDB0"          // OR B (F7h, 10100000)
        // Test zero
        "DD3E00"        // LD A, 00h
        "DD0E00"        // LD C, 00h
        "DDB1"          // OR C (00h, 01000100)
        // Test odd parity
        "DD3E38"        // LD A, 38h
        "DD1604"        // LD D, 04h
        "DDB2"          // OR D (3Ch, 00101100)
        // Test even parity
        "DD3E38"        // LD A, 38h
        "DD1E10"        // LD E, 18h
        "DDB3";         // OR E (38h, 00101000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(or_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3EF3"        // LD A, F3h
        "FD06A5"        // LD B, A5h
        "FDB0"          // OR B (F7h, 10100000)
        // Test zero
        "FD3E00"        // LD A, 00h
        "FD0E00"        // LD C, 00h
        "FDB1"          // OR C (00h, 01000100)
        // Test odd parity
        "FD3E38"        // LD A, 38h
        "FD1604"        // LD D, 04h
        "FDB2"          // OR D (3Ch, 00101100)
        // Test even parity
        "FD3E38"        // LD A, 38h
        "FD1E10"        // LD E, 18h
        "FDB3";         // OR E (38h, 00101000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(xor_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3EF3"        // LD A, F3h
        "DD0635"        // LD B, A5h
        "DDA8"          // XOR B (C6h, 10000100)
        // Test zero
        "DD3E43"        // LD A, 43h
        "DD0E43"        // LD C, 43h
        "DDA9"          // XOR C (00h, 01000100)
        // Test odd parity
        "DD3E76"        // LD A, 76h
        "DD165C"        // LD D, 5Ch
        "DDAA"          // XOR D (2Ah, 00101000)
        // Test even parity
        "DD3E76"        // LD A, 76h
        "DD1E58"        // LD E, 58h
        "DDAB";         // XOR E (2Eh, 00101100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}

BOOST_AUTO_TEST_CASE(xor_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3EF3"        // LD A, F3h
        "FD0635"        // LD B, A5h
        "FDA8"          // XOR B (C6h, 10000100)
        // Test zero
        "FD3E43"        // LD A, 43h
        "FD0E43"        // LD C, 43h
        "FDA9"          // XOR C (00h, 01000100)
        // Test odd parity
        "FD3E76"        // LD A, 76h
        "FD165C"        // LD D, 5Ch
        "FDAA"          // XOR D (2Ah, 00101000)
        // Test even parity
        "FD3E76"        // LD A, 76h
        "FD1E58"        // LD E, 58h
        "FDAB";         // XOR E (2Eh, 00101100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}

BOOST_AUTO_TEST_CASE(cp_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD3E73"        // LD A, 73h
        "DD06A5"        // LD B, A5h
        "DDB8"          // CP B (10110111)
        // Test zero
        "DD3E43"        // LD A, 43h
        "DD0E43"        // LD C, 43h
        "DDB9"          // CP C (01000010)
        // Test odd parity
        "DD3E76"        // LD A, 76h
        "DD165C"        // LD D, 5Ch
        "DDBA";         // CP D (00011010)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(cp_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD3E73"        // LD A, 73h
        "FD06A5"        // LD B, A5h
        "FDB8"          // CP B (10110111)
        // Test zero
        "FD3E43"        // LD A, 43h
        "FD0E43"        // LD C, 43h
        "FDB9"          // CP C (01000010)
        // Test odd parity
        "FD3E76"        // LD A, 76h
        "FD165C"        // LD D, 5Ch
        "FDBA";         // CP D (00011010)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 30);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(inc_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code = 
        "DD3EFE"        // LD A, FEh
        "DD47"          // LD B, A
        "DD04"          // INC B (0xFF, 10101000)
        "DD48"          // LD C, B
        "DD0C"          // INC C (0x00, 01010000)
        "DD51"          // LD D, C
        "DD14"          // INC D (0x01, 00000000)
        "DD1E7F"        // LD E, 7Fh
        "DD1C";         // INC E (0x80, 10010100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 27);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEA8);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFF00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE50);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x01FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE00);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x0180);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE94);
}

BOOST_AUTO_TEST_CASE(inc_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code = 
        "FD3EFE"        // LD A, FEh
        "FD47"          // LD B, A
        "FD04"          // INC B (0xFF, 10101000)
        "FD48"          // LD C, B
        "FD0C"          // INC C (0x00, 01010000)
        "FD51"          // LD D, C
        "FD14"          // INC D (0x01, 00000000)
        "FD1E7F"        // LD E, 7Fh
        "FD1C";         // INC E (0x80, 10010100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 27);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEA8);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFF00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE50);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x01FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE00);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x0180);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFE94);
}

BOOST_AUTO_TEST_CASE(dec_rx_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD3E01"        // LD A, 01h
        "DD47"          // LD B, A
        "DD05"          // DEC B (0x00, 01000010)
        "DD48"          // LD C, B
        "DD0D"          // DEC C (0xFF, 10111010)
        "DD51"          // LD D, C
        "DD15"          // DEC D (0xFE, 10101010)
        "DD1E80"        // LD E, 80h
        "DD1D";         // DEC E (0x7F, 00111110)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 27);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x00FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0142);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x00FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x01BA);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFEFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x01AA);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFE7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x013E);
}

BOOST_AUTO_TEST_CASE(dec_ry_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD3E01"        // LD A, 01h
        "FD47"          // LD B, A
        "FD05"          // DEC B (0x00, 01000010)
        "FD48"          // LD C, B
        "FD0D"          // DEC C (0xFF, 10111010)
        "FD51"          // LD D, C
        "FD15"          // DEC D (0xFE, 10101010)
        "FD1E80"        // LD E, 80h
        "FD1D";         // DEC E (0x7F, 00111110)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 27);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x00FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0142);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x00FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x01BA);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFEFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x01AA);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFE7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x013E);
}

BOOST_AUTO_TEST_CASE(add_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "C6F3"          // ADD A, F3h     (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "C6F4"          // ADD A, F4h     (00h, 01010001)
        // Test half carry
        "DD3E08"        // LD A, 08h
        "DDC628"        // ADD A, 28h     (30h, 00110000)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DDC610"        // ADD A, 10h     (8Fh, 10001100)
        // Test carry
        "FD3E80"        // LD A, 80h
        "FDC688"        // ADD A, 88h     (08h, 00001101)
        // Test 80h
        "FD3E00"        // LD A, 00h
        "FDC680";       // ADD A, 80h     (80h, 10000000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
}

BOOST_AUTO_TEST_CASE(adc_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "CEF2"          // ADC A, F2h (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "CEF3"          // ADC A, F3h (00h, 01010001)
        // Test half carry
        "DD3E07"        // LD A, 07h
        "DDCE28"        // ADC A, 28h (30h, 00110000)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DDCE10"        // ADC A, 10h (90h, 10010100)
        // Test carry
        "FD3E80"        // LD A, 80h
        "FDCE88"        // ADC A, 88h (09h, 00001101)
        // Test 80h
        "FD3E00"        // LD A, 00h
        "FDCE7F";       // ADC A, 7Fh (80h, 10000000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
}

BOOST_AUTO_TEST_CASE(sub_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "D60E"          // SUB 0Eh (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "D60C"          // SUB 0Ch (00h, 01000010)
        // Test half carry
        "DD3E50"        // LD A, 50h
        "DDD608"        // SUB 08h (48h, 00011010)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DDD681"        // SUB 81h (FEh, 10101111)
        // Test carry
        "FD3E81"        // LD A, 81h
        "FDD602"        // SUB 02h (7Fh, 00111110)
        // Test 0x80
        "FD3E00"        // LD A, 00h
        "FDD680";       // SUB 80h (80h, 10000111)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sbc_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E0C"          // LD A, 0Ch
        "DE0D"          // SBC A, 0Dh (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "DE0B"          // SBC A, 0Bh (00h, 01000010)
        // Test half carry
        "DD3E50"        // LD A, 50h
        "DDDE07"        // SBC A, 07h (48h, 00011010)
        // Test overflow
        "DD3E7F"        // LD A, 7Fh
        "DDDE80"        // SBC A, 80h (FEh, 10101111)
        // Test carry
        "FD3E81"        // LD A, 81h
        "FDDE03"        // SBC A, 03h (7Dh, 00111110)
        // Test 0x80
        "FD3E00"        // LD A, 00h
        "FDDE80";       // SBC A, 80h (7Fh, 00111011)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(and_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3EF3"          // LD A, F3h
        "E6A5"          // AND A5h (A1h, 10110000)
        // Test zero
        "3E76"          // LD A, 76h
        "E688"          // AND 88h (00h, 01010100)
        // Test odd parity
        "DD3E2B"        // LD A, 2Bh
        "DDE67E"        // AND 7Eh (2Ah, 00111000)
        // Test even parity
        "FD3E2B"        // LD A, 2Bh
        "FDE67C";       // AND 7Ch (28h, 00111100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(or_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3EF3"          // LD A, F3h
        "F6A5"          // OR A5h (F7h, 10100000)
        // Test zero
        "3E00"          // LD A, 00h
        "F600"          // OR 00h (00h, 01000100)
        // Test odd parity
        "DD3E38"        // LD A, 38h
        "DDF604"        // OR 04h (3Ch, 00101100)
        // Test even parity
        "FD3E38"        // LD A, 38h
        "FDF610";       // OR 18h (38h, 00101000)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(xor_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3EF3"          // LD A, F3h
        "EE35"          // XOR 35h (C6h, 10000100)
        // Test zero
        "3E43"          // LD A, 43h
        "EE43"          // XOR 43h (00h, 01000100)
        // Test odd parity
        "DD3E76"        // LD A, 76h
        "DDEE5C"        // XOR 5Ch (2Ah, 00101000)
        // Test even parity
        "FD3E76"        // LD A, 76h
        "FDEE58";       // XOR 58h (2Eh, 00101100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}

BOOST_AUTO_TEST_CASE(cp_n_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "3E73"          // LD A, 73h
        "FEA5"          // CP A5h (10110111)
        // Test zero
        "DD3E43"        // LD A, 43h
        "DDFE43"        // CP 43h (01000010)
        // Test odd parity
        "FD3E76"        // LD A, 76h
        "FDFE5C";       // CP 5Ch (00011010)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(add_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h     (F3h)
        "3E0C"          // LD A, 0Ch
        "86"            // ADD A, (HL)      (FFh, 10101000)
        // Test zero
        "210101"        // LD HL, 0101h     (F4h)
        "3E0C"          // LD A, 0Ch
        "86"            // ADD A, (HL)      (00h, 01010001)
        // Test half carry
        "210201"        // LD HL, 0102h     (28h)
        "3E08"          // LD A, 08h
        "86"            // ADD A, (HL)      (30h, 00110000)
        // Test overflow
        "210301"        // LD HL, 0103h     (10h)
        "3E7F"          // LD A, 7Fh
        "86"            // ADD A, (HL)      (8Fh, 10001100)
        // Test carry
        "210401"        // LD HL, 0104h     (88h)
        "3E80"          // LD A, 80h
        "86"            // ADD A, (HL)      (08h, 00001101)
        // Test 80h
        "210501"        // LD HL, 0105h     (80h)
        "3E00"          // LD A, 00h
        "86";           // ADD A, (HL)      (80h, 10000000)

    string data = "F3F428108880";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
}

BOOST_AUTO_TEST_CASE(adc_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (F2h)
        "3E0C"          // LD A, 0Ch
        "8E"            // ADC A, (HL)  (FFh, 10101000)
        // Test zero
        "210101"        // LD HL, 0101h (F3h)
        "3E0C"          // LD A, 0Ch
        "8E"            // ADC A, (HL)  (00h, 01010001)
        // Test half carry
        "210201"        // LD HL, 0102h (28h)
        "3E07"          // LD A, 07h
        "8E"            // ADC A, (HL)  (30h, 00110000)
        // Test overflow
        "210301"        // LD HL, 0103h (10h)
        "3E7F"          // LD A, 7Fh
        "8E"            // ADC A, (HL)  (90h, 10010100)
        // Test carry
        "210401"        // LD HL, 0104h (88h)
        "3E80"          // LD A, 80h
        "8E"            // ADC A, (HL)  (09h, 00001101)
        // Test 80h
        "210501"        // LD HL, 0105h (7Fh)
        "3E00"          // LD A, 00h
        "8E";           // ADC A, (HL)  (80h, 10000000)

    string data = "F2F32810887F";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
}

BOOST_AUTO_TEST_CASE(sub_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (0Eh)
        "3E0C"          // LD A, 0Ch
        "96"            // SUB (HL) (FEh, 10111011)
        // Test zero
        "210101"        // LD HL, 0101h (0Ch)
        "3E0C"          // LD A, 0Ch
        "96"            // SUB (HL) (00h, 01000010)
        // Test half carry
        "210201"        // LD HL, 0102h (08h)
        "3E50"          // LD A, 50h
        "96"            // SUB (HL) (48h, 00011010)
        // Test overflow
        "210301"        // LD HL, 0103h (81h)
        "3E7F"          // LD A, 7Fh
        "96"            // SUB (HL) (FEh, 10101111)
        // Test carry
        "210401"        // LD HL, 0104h (02h)
        "3E81"          // LD A, 81h
        "96"            // SUB (HL) (7Fh, 00111110)
        // Test 0x80
        "210501"        // LD HL, 0105h (80h)
        "3E00"          // LD A, 00h
        "96";           // SUB (HL) (80h, 10000111)

    string data = "0E0C08810280";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sbc_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (0Dh)
        "3E0C"          // LD A, 0Ch
        "9E"            // SBC A, (HL) (FEh, 10111011)
        // Test zero
        "210101"        // LD HL, 0101h (0Bh)
        "3E0C"          // LD A, 0Ch
        "9E"            // SBC A, (HL) (00h, 01000010)
        // Test half carry
        "210201"        // LD HL, 0102h (07h)
        "3E50"          // LD A, 50h
        "9E"            // SBC A, (HL) (48h, 00011010)
        // Test overflow
        "210301"        // LD HL, 0103h (80h)
        "3E7F"          // LD A, 7Fh
        "9E"            // SBC A, (HL) (FEh, 10101111)
        // Test carry
        "210401"        // LD HL, 0104h (03h)
        "3E81"          // LD A, 81h
        "9E"            // SBC A, (HL) (7Dh, 00111110)
        // Test 0x80
        "210501"        // LD HL, 0105h (80h)
        "3E00"          // LD A, 00h
        "9E";           // SBC A, (HL) (7Fh, 00111011)

    string data = "0D0B07800380";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(and_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (A5h)
        "3EF3"          // LD A, F3h
        "A6"            // AND (HL) (A1h, 10110000)
        // Test zero
        "210101"        // LD HL, 0101h
        "3E76"          // LD A, 76h
        "A6"            // AND (HL) (00h, 01010100)
        // Test odd parity
        "210201"        // LD HL, 0102h
        "3E2B"          // LD A, 2Bh
        "A6"            // AND (HL) (2Ah, 00111000)
        // Test even parity
        "210301"        // LD HL, 0103h
        "3E2B"          // LD A, 2Bh
        "A6";           // AND (HL) (28h, 00111100)

    string data = "A5887E7C";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(or_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (A5h)
        "3EF3"          // LD A, F3h
        "B6"            // OR (HL) (F7h, 10100000)
        // Test zero
        "210101"        // LD HL, 0101h (00h)
        "3E00"          // LD A, 00h
        "B6"            // OR (HL) (00h, 01000100)
        // Test odd parity
        "210201"        // LD HL, 0102h (04h)
        "3E38"          // LD A, 38h
        "B6"            // OR (HL) (3Ch, 00101100)
        // Test even parity
        "210301"        // LD HL, 0103h (18h)
        "3E38"          // LD A, 38h
        "B6";           // OR (HL) (38h, 00101000)

    string data = "A5000418";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(xor_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (A5h)
        "3EF3"          // LD A, F3h
        "AE"            // XOR (HL) (C6h, 10000100)
        // Test zero
        "210101"        // LD HL, 0101h (43h)
        "3E43"          // LD A, 43h
        "AE"            // XOR (HL) (00h, 01000100)
        // Test odd parity
        "210201"        // LD HL, 0102h (5Ch)
        "3E76"          // LD A, 76h
        "AE"            // XOR (HL) (2Ah, 00101000)
        // Test even parity
        "210301"        // LD HL, 0103h (58h)
        "3E76"          // LD A, 76h
        "AE";           // XOR (HL) (2Eh, 00101100)

    string data = "35435C58";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}

BOOST_AUTO_TEST_CASE(cp_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "210001"        // LD HL, 0100h (A5h)
        "3E73"          // LD A, 73h
        "BE"            // CP (HL) (10110111)
        // Test zero
        "210101"        // LD HL, 0101h (43h)
        "3E43"          // LD A, 43h
        "BE"            // CP (HL) (01000010)
        // Test odd parity
        "210201"        // LD HL, 0102h (5Ch)
        "3E76"          // LD A, 76h
        "BE";           // CP (HL) (00011010)

    string data = "A5435C";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(add_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3E0C"          // LD A, 0Ch
        "DD8600"        // ADD A, (IX+0)    (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "DD8601"        // ADD A, (IX+1)    (00h, 01010001)
        // Test half carry
        "3E08"          // LD A, 08h
        "DD8602"        // ADD A, (IX+2)    (30h, 00110000)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "DD8603"        // ADD A, (IX+3)    (8Fh, 10001100)
        // Test carry
        "3E80"          // LD A, 80h
        "DD8604"        // ADD A, (IX+4)    (08h, 00001101)
        // Test 80h
        "3E00"          // LD A, 00h
        "DD8605";       // ADD A, (IX+5)    (80h, 10000000)

    string data = "F3F428108880";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
}

BOOST_AUTO_TEST_CASE(add_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3E0C"          // LD A, 0Ch
        "FD8600"        // ADD A, (IY+0)    (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "FD8601"        // ADD A, (IY+1)    (00h, 01010001)
        // Test half carry
        "3E08"          // LD A, 08h
        "FD8602"        // ADD A, (IY+2)    (30h, 00110000)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "FD8603"        // ADD A, (IY+3)    (8Fh, 10001100)
        // Test carry
        "3E80"          // LD A, 80h
        "FD8604"        // ADD A, (IY+4)    (08h, 00001101)
        // Test 80h
        "3E00"          // LD A, 00h
        "FD8605";       // ADD A, (IY+5)    (80h, 10000000)

    string data = "F3F428108880";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8F8C);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x080D);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8080);
}

BOOST_AUTO_TEST_CASE(adc_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3E0C"          // LD A, 0Ch
        "DD8E00"        // ADC A, (IX+0) (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "DD8E01"        // ADC A, (IX+1) (00h, 01010001)
        // Test half carry
        "3E07"          // LD A, 07h
        "DD8E02"        // ADC A, (IX+2) (30h, 00110000)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "DD8E03"        // ADC A, (IX+3) (90h, 10010100)
        // Test carry
        "3E80"          // LD A, 80h
        "DD8E04"        // ADC A, (IX+4) (09h, 00001101)
        // Test 80h
        "3E00"          // LD A, 00h
        "DD8E05";       // ADC A, (IX+5) (80h, 10000000)

    string data = "F2F32810887F";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
}

BOOST_AUTO_TEST_CASE(adc_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3E0C"          // LD A, 0Ch
        "FD8E00"        // ADC A, (IY+0) (FFh, 10101000)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "FD8E01"        // ADC A, (IY+1) (00h, 01010001)
        // Test half carry
        "3E07"          // LD A, 07h
        "FD8E02"        // ADC A, (IY+2) (30h, 00110000)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "FD8E03"        // ADC A, (IY+3) (90h, 10010100)
        // Test carry
        "3E80"          // LD A, 80h
        "FD8E04"        // ADC A, (IY+4) (09h, 00001101)
        // Test 80h
        "3E00"          // LD A, 00h
        "FD8E05";       // ADC A, (IY+5) (80h, 10000000)

    string data = "F2F32810887F";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFFA8);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0051);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3030);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9094);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x090D);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8094);
}

BOOST_AUTO_TEST_CASE(sub_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3E0C"          // LD A, 0Ch
        "DD9600"        // SUB (IX+0) (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "DD9601"        // SUB (IX+1) (00h, 01000010)
        // Test half carry
        "3E50"          // LD A, 50h
        "DD9602"        // SUB (IX+2) (48h, 00011010)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "DD9603"        // SUB (IX+3) (FEh, 10101111)
        // Test carry
        "3E81"          // LD A, 81h
        "DD9604"        // SUB (IX+4) (7Fh, 00111110)
        // Test 0x80
        "3E00"          // LD A, 00h
        "DD9605";       // SUB (IX+5) (80h, 10000111)

    string data = "0E0C08810280";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sub_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3E0C"          // LD A, 0Ch
        "FD9600"        // SUB (IY+0) (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "FD9601"        // SUB (IY+1) (00h, 01000010)
        // Test half carry
        "3E50"          // LD A, 50h
        "FD9602"        // SUB (IY+2) (48h, 00011010)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "FD9603"        // SUB (IY+3) (FEh, 10101111)
        // Test carry
        "3E81"          // LD A, 81h
        "FD9604"        // SUB (IY+4) (7Fh, 00111110)
        // Test 0x80
        "3E00"          // LD A, 00h
        "FD9605";       // SUB (IY+5) (80h, 10000111)

    string data = "0E0C08810280";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3E);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8087);
}

BOOST_AUTO_TEST_CASE(sbc_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h (0Dh)
        "3E0C"          // LD A, 0Ch
        "DD9E00"        // SBC A, (IX+0) (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "DD9E01"        // SBC A, (IX+1) (00h, 01000010)
        // Test half carry
        "3E50"          // LD A, 50h
        "DD9E02"        // SBC A, (IX+2) (48h, 00011010)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "DD9E03"        // SBC A, (IX+3) (FEh, 10101111)
        // Test carry
        "3E81"          // LD A, 81h
        "DD9E04"        // SBC A, (IX+4) (7Dh, 00111110)
        // Test 0x80
        "3E00"          // LD A, 00h
        "DD9E05";       // SBC A, (IX+5) (7Fh, 00111011)

    string data = "0D0B07800380";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(sbc_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h (0Dh)
        "3E0C"          // LD A, 0Ch
        "FD9E00"        // SBC A, (IY+0) (FEh, 10111011)
        // Test zero
        "3E0C"          // LD A, 0Ch
        "FD9E01"        // SBC A, (IY+1) (00h, 01000010)
        // Test half carry
        "3E50"          // LD A, 50h
        "FD9E02"        // SBC A, (IY+2) (48h, 00011010)
        // Test overflow
        "3E7F"          // LD A, 7Fh
        "FD9E03"        // SBC A, (IY+3) (FEh, 10101111)
        // Test carry
        "3E81"          // LD A, 81h
        "FD9E04"        // SBC A, (IY+4) (7Dh, 00111110)
        // Test 0x80
        "3E00"          // LD A, 00h
        "FD9E05";       // SBC A, (IY+5) (7Fh, 00111011)

    string data = "0D0B07800380";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEBB);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0042);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x481A);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xFEAF);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7D3E);
    z80.decoder.regs.af.l |= 0x01;
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x7F3B);
}

BOOST_AUTO_TEST_CASE(and_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3EF3"          // LD A, F3h
        "DDA600"        // AND (IX+0) (A1h, 10110000)
        // Test zero
        "3E76"          // LD A, 76h
        "DDA601"        // AND (IX+1) (00h, 01010100)
        // Test odd parity
        "3E2B"          // LD A, 2Bh
        "DDA602"        // AND (IX+2) (2Ah, 00111000)
        // Test even parity
        "3E2B"          // LD A, 2Bh
        "DDA603";       // AND (IX+3) (28h, 00111100)

    string data = "A5887E7C";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(and_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3EF3"          // LD A, F3h
        "FDA600"        // AND (IY+0) (A1h, 10110000)
        // Test zero
        "3E76"          // LD A, 76h
        "FDA601"        // AND (IY+1) (00h, 01010100)
        // Test odd parity
        "3E2B"          // LD A, 2Bh
        "FDA602"        // AND (IY+2) (2Ah, 00111000)
        // Test even parity
        "3E2B"          // LD A, 2Bh
        "FDA603";       // AND (IY+3) (28h, 00111100)

    string data = "A5887E7C";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xA1B0);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0054);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A38);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x283C);
}

BOOST_AUTO_TEST_CASE(or_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3EF3"          // LD A, F3h
        "DDB600"        // OR (IX+0) (F7h, 10100000)
        // Test zero
        "3E00"          // LD A, 00h
        "DDB601"        // OR (IX+1) (00h, 01000100)
        // Test odd parity
        "3E38"          // LD A, 38h
        "DDB602"        // OR (IX+2) (3Ch, 00101100)
        // Test even parity
        "3E38"          // LD A, 38h
        "DDB603";       // OR (IX+3) (38h, 00101000)

    string data = "A5000418";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(or_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3EF3"          // LD A, F3h
        "FDB600"        // OR (IY+0) (F7h, 10100000)
        // Test zero
        "3E00"          // LD A, 00h
        "FDB601"        // OR (IY+1) (00h, 01000100)
        // Test odd parity
        "3E38"          // LD A, 38h
        "FDB602"        // OR (IY+2) (3Ch, 00101100)
        // Test even parity
        "3E38"          // LD A, 38h
        "FDB603";       // OR (IY+3) (38h, 00101000)

    string data = "A5000418";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xF7A0);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3C2C);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x3828);
}

BOOST_AUTO_TEST_CASE(xor_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3EF3"          // LD A, F3h
        "DDAE00"        // XOR (IX+0) (C6h, 10000100)
        // Test zero
        "3E43"          // LD A, 43h
        "DDAE01"        // XOR (IX+1) (00h, 01000100)
        // Test odd parity
        "3E76"          // LD A, 76h
        "DDAE02"        // XOR (IX+2) (2Ah, 00101000)
        // Test even parity
        "3E76"          // LD A, 76h
        "DDAE03";       // XOR (IX+3) (2Eh, 00101100)

    string data = "35435C58";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}

BOOST_AUTO_TEST_CASE(xor_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3EF3"          // LD A, F3h
        "FDAE00"        // XOR (IY+0) (C6h, 10000100)
        // Test zero
        "3E43"          // LD A, 43h
        "FDAE01"        // XOR (IY+1) (00h, 01000100)
        // Test odd parity
        "3E76"          // LD A, 76h
        "FDAE02"        // XOR (IY+2) (2Ah, 00101000)
        // Test even parity
        "3E76"          // LD A, 76h
        "FDAE03";       // XOR (IY+3) (2Eh, 00101100)

    string data = "35435C58";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0xC684);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0044);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2A28);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2E2C);
}

BOOST_AUTO_TEST_CASE(cp_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "DD210001"      // LD IX, 0100h
        "3E73"          // LD A, 73h
        "DDBE00"        // CP (IX+0) (10110111)
        // Test zero
        "3E43"          // LD A, 43h
        "DDBE01"        // CP (IX+1) (01000010)
        // Test odd parity
        "3E76"          // LD A, 76h
        "DDBE02";       // CP (IX+2) (00011010)

    string data = "A5435C";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(cp_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        // Test sign
        "FD210001"      // LD IY, 0100h
        "3E73"          // LD A, 73h
        "FDBE00"        // CP (IY+0) (10110111)
        // Test zero
        "3E43"          // LD A, 43h
        "FDBE01"        // CP (IY+1) (01000010)
        // Test odd parity
        "3E76"          // LD A, 76h
        "FDBE02";       // CP (IY+2) (00011010)

    string data = "A5435C";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    runCycles(z80, m, 40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x73B7);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4342);
    runCycles(z80, m, 26);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x761A);
}

BOOST_AUTO_TEST_CASE(inc_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "36FE"          // LD (HL), FEh
        "34"            // INC (HL) (0xFF, 10101000)
        "34"            // INC (HL) (0x00, 01010000)
        "34"            // INC (HL) (0x01, 00000000)
        "367F"          // LD (HL), 7Fh
        "34";           // INC (HL) (0x80, 10010100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 31);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x50);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 21);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x94);
}

BOOST_AUTO_TEST_CASE(dec_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3601"          // LD (HL), 01h
        "35"            // DEC (HL) (0x00, 01000010)
        "35"            // DEC (HL) (0xFF, 10111010)
        "35"            // DEC (HL) (0xFE, 10101010)
        "3680"          // LD (HL), 80h
        "35";           // DEC (HL) (0x7F, 00111110)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 31);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x42);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xBA);
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAA);
    runCycles(z80, m, 21);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x3E);
}

BOOST_AUTO_TEST_CASE(inc_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD210001"      // LD IX, 0100h
        "DD3604FE"      // LD (IX+4), FEh
        "DD3404"        // INC (IX+4) (0xFF, 10101000)
        "DD3404"        // INC (IX+4) (0x00, 01010000)
        "DD3404"        // INC (IX+4) (0x01, 00000000)
        "DD36047F"      // LD (IX+4), 7Fh
        "DD3404";       // INC (IX+4) (0x80, 10010100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 56);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x50);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 42);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x94);
}

BOOST_AUTO_TEST_CASE(inc_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD210001"      // LD IY, 0100h
        "FD3604FE"      // LD (IY+4), FEh
        "FD3404"        // INC (IY+4) (0xFF, 10101000)
        "FD3404"        // INC (IY+4) (0x00, 01010000)
        "FD3404"        // INC (IY+4) (0x01, 00000000)
        "FD36047F"      // LD (IY+4), 7Fh
        "FD3404";       // INC (IY+4) (0x80, 10010100)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 56);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x50);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 42);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x94);
}

BOOST_AUTO_TEST_CASE(dec_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD210001"      // LD IX, 0100h
        "DD360401"      // LD (IX+4), 01h
        "DD3504"        // DEC (IX+4) (0x00, 01000010)
        "DD3504"        // DEC (IX+4) (0xFF, 10111010)
        "DD3504"        // DEC (IX+4) (0xFE, 10101010)
        "DD360480"      // LD (IX+4), 80h
        "DD3504";       // DEC (IX+4) (0x7F, 00111110)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 56);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x42);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xBA);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAA);
    runCycles(z80, m, 42);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x3E);
}

BOOST_AUTO_TEST_CASE(dec_ptriy_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "FD210001"      // LD IY, 0100h
        "FD360401"      // LD (IY+4), 01h
        "FD3504"        // DEC (IY+4) (0x00, 01000010)
        "FD3504"        // DEC (IY+4) (0xFF, 10111010)
        "FD3504"        // DEC (IY+4) (0xFE, 10101010)
        "FD360480"      // LD (IY+4), 80h
        "FD3504";       // DEC (IY+4) (0x7F, 00111110)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 56);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x42);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xBA);
    runCycles(z80, m, 23);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAA);
    runCycles(z80, m, 42);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x3E);
}

BOOST_AUTO_TEST_CASE(daa_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "27";           // DAA

    loadBinary(code, m, 0x0000);

    // H:0-9 L:0-9 CF:0 HF:0 -> 00 CF':0 HF':0
    startZ80(z80);
    z80.decoder.regs.af.w = 0x9900;
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x998C);

    // H:0-9 L:0-9 CF:0 HF:1 -> 06 CF':0 HF':0
    startZ80(z80);
    z80.decoder.regs.af.w = 0x9110;
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x9780);

    // H:0-8 L:A-F CF:0 HF:0 -> 06 CF':0 HF':1
    startZ80(z80);
    z80.decoder.regs.af.w = 0x3F00;
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4510);

    // H:0-8 L:A-F CF:0 HF:1 -> 06 CF':0 HF':1
    startZ80(z80);
    z80.decoder.regs.af.w = 0x3F10;
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4510);
}

// EOF
// vim: et:sw=4:ts=4
