#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
#include <boost/test/unit_test.hpp>
//#include <boost/test/include/unit_test.hpp>

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

/*
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
    m.memory[0x0004] = 0xB0;                            // OR B (F7h, 10100000)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x00;   // LD A, 00h
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x00;   // LD C, 00h
    m.memory[0x0009] = 0xB1;                            // OR C (00h, 01000100)
    // Test odd parity
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x38;   // LD A, 38h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x04;   // LD D, 04h
    m.memory[0x000E] = 0xB2;                            // OR D (3Ch, 00101100)
    // Test even parity
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x38;   // LD A, 38h
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x10;   // LD E, 18h
    m.memory[0x0013] = 0xB3;                            // OR E (38h, 00101000)

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
    m.memory[0x0004] = 0xA8;                            // XOR B (C6h, 10000100)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x43;   // LD A, 43h
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x43;   // LD C, 43h
    m.memory[0x0009] = 0xA9;                            // XOR C (00h, 01000100)
    // Test odd parity
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x76;   // LD A, 76h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x5C;   // LD D, 5Ch
    m.memory[0x000E] = 0xAA;                            // XOR D (2Ah, 00101000)
    // Test even parity
    m.memory[0x000F] = 0x3E; m.memory[0x0010] = 0x76;   // LD A, 76h
    m.memory[0x0011] = 0x1E; m.memory[0x0012] = 0x58;   // LD E, 58h
    m.memory[0x0013] = 0xAB;                            // XOR E (2Eh, 00101100)

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

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x73;   // LD A, 73h
    m.memory[0x0002] = 0x06; m.memory[0x0003] = 0xA5;   // LD B, A5h
    m.memory[0x0004] = 0xB8;                            // CP B (10110111)
    // Test zero
    m.memory[0x0005] = 0x3E; m.memory[0x0006] = 0x43;   // LD A, 43h
    m.memory[0x0007] = 0x0E; m.memory[0x0008] = 0x43;   // LD C, 43h
    m.memory[0x0009] = 0xB9;                            // CP C (01000010)
    // Test odd parity
    m.memory[0x000A] = 0x3E; m.memory[0x000B] = 0x76;   // LD A, 76h
    m.memory[0x000C] = 0x16; m.memory[0x000D] = 0x5C;   // LD D, 5Ch
    m.memory[0x000E] = 0xBA;                            // CP D (00011010)

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

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0xFE;   // LD A, FEh
    m.memory[0x0002] = 0x47;                            // LD B, A
    m.memory[0x0003] = 0x04;                            // INC B (0xFF, 10101000)
    m.memory[0x0004] = 0x48;                            // LD C, B
    m.memory[0x0005] = 0x0C;                            // INC C (0x00, 01010000)
    m.memory[0x0006] = 0x51;                            // LD D, C
    m.memory[0x0007] = 0x14;                            // INC D (0x01, 00000000)
    m.memory[0x0008] = 0x1E; m.memory[0x0009] = 0x7F;   // LD E, 7Fh
    m.memory[0x000A] = 0x1C;                            // INC E (0x80, 10010100)

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

    // Test sign
    m.memory[0x0000] = 0x3E; m.memory[0x0001] = 0x01;   // LD A, 01h
    m.memory[0x0002] = 0x47;                            // LD B, A
    m.memory[0x0003] = 0x05;                            // DEC B (0x00, 01000010)
    m.memory[0x0004] = 0x48;                            // LD C, B
    m.memory[0x0005] = 0x0D;                            // DEC C (0xFF, 10111010)
    m.memory[0x0006] = 0x51;                            // LD D, C
    m.memory[0x0007] = 0x15;                            // DEC D (0xFE, 10101010)
    m.memory[0x0008] = 0x1E; m.memory[0x0009] = 0x80;   // LD E, 80h
    m.memory[0x000A] = 0x1D;                            // DEC E (0x7F, 00111110)

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
*/
// EOF
// vim: et:sw=4:ts=4
