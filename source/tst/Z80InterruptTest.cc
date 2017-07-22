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
    for (size_t i = 0; i < cycles; ++i)
    {
        m.a = z80.a;
        m.as_ = (z80.c & SIGNAL_MREQ_) == SIGNAL_MREQ_;
        m.rd_ = (z80.c & SIGNAL_RD_) == SIGNAL_RD_;
        m.wr_ = (z80.c & SIGNAL_WR_) == SIGNAL_WR_;
        m.d = z80.d;
        m.clock();
        z80.d = m.d;
        z80.clock();
        m.a = z80.a; m.d = z80.d;
        m.as_ = (z80.c & SIGNAL_MREQ_) == SIGNAL_MREQ_;
        m.rd_ = (z80.c & SIGNAL_RD_) == SIGNAL_RD_;
        m.wr_ = (z80.c & SIGNAL_WR_) == SIGNAL_WR_;
        m.d = z80.d;
        m.clock();
        z80.d = m.d;
        z80.clock();
    }
}

void runIntCycles(Z80& z80, uint8_t data, size_t cycles)
{
    for (size_t i = 0; i != cycles; ++i)
    {
        z80.d = data;
        z80.clock();
        z80.d = data;
        z80.clock();
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

BOOST_AUTO_TEST_CASE(nmi_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "0E02"          // LD C, 02h
        "1603"          // LD D, 03h
        "1E04"          // LD E, 04h
        "2605"          // LD H, 05h
        "2E06";         // LD L, 06h

    loadBinary(code, m, 0x0000);

    string nmi =
        "3E08";         // LD A, 08h

    loadBinary(nmi, m, 0x0066);

    startZ80(z80);
    z80.decoder.regs.iff = IFF1 | IFF2;
    runCycles(z80, m, 7);   // Run first instruction.
    runCycles(z80, m, 2);   // Run two cycles of second instruction.
    z80.c &= ~SIGNAL_NMI_;  // Signal NMI.
    runCycles(z80, m, 1);   // Tick.
    z80.c ^= SIGNAL_NMI_;   // Clear NMI.
    runCycles(z80, m, 4);   // Rest of second instruction.
    runCycles(z80, m, 11);  // Execute NMI jump.
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0066);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iff, IFF2);
    runCycles(z80, m, 7);   // Execute instruction at 66h.
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0102);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x08);
}

BOOST_AUTO_TEST_CASE(int_mode_0_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "0E02"          // LD C, 02h
        "1603"          // LD D, 03h
        "1E04"          // LD E, 04h
        "2605"          // LD H, 05h
        "2E06";         // LD L, 06h

    loadBinary(code, m, 0x0000);

    startZ80(z80);
    z80.decoder.regs.iff = IFF1 | IFF2;
    runCycles(z80, m, 7);   // Run first instruction.
    runCycles(z80, m, 2);   // Run two cycles of second instruction.
    z80.c &= ~SIGNAL_INT_;  // Signal INT.
    runCycles(z80, m, 4);   // Rest of second instruction.
    runCycles(z80, m, 1);   // Rest of second instruction.
    z80.c ^= SIGNAL_INT_;   // Clear INT.
    runIntCycles(z80, 0xDD, 6);
    runIntCycles(z80, 0x01, 4);
    runIntCycles(z80, 0xCC, 3);
    runIntCycles(z80, 0xBB, 3);
    runCycles(z80, m, 7);   // Run another instruction.

    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0006);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iff, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xBBCC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x03FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
}

BOOST_AUTO_TEST_CASE(int_mode_1_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "0E02"          // LD C, 02h
        "1603"          // LD D, 03h
        "1E04"          // LD E, 04h
        "2605"          // LD H, 05h
        "2E06";         // LD L, 06h
    loadBinary(code, m, 0x0000);

    string ihnd =
        "21AAAA"        // LD HL, AAAAh
        "ED4D";         // RETI
    loadBinary(ihnd, m, 0x0038);

    startZ80(z80);
    z80.decoder.regs.iff = IFF1 | IFF2;
    z80.decoder.regs.im = 1;
    runCycles(z80, m, 7);   // Run first instruction.
    runCycles(z80, m, 2);   // Run two cycles of second instruction.
    z80.c &= ~SIGNAL_INT_;  // Signal INT.
    runCycles(z80, m, 4);   // Rest of second instruction.
    runCycles(z80, m, 1);   // Rest of second instruction.
    z80.c ^= SIGNAL_INT_;   // Clear INT.
    runCycles(z80, m, 13);  // Execute interrupt jump.
    runCycles(z80, m, 10);  // Execute instruction at 38h.
    runCycles(z80, m, 14);  // Execute RETI.
    runCycles(z80, m, 7);   // Execute one more instruction.

    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0006);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iff, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0102);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x03FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xAAAA);
}

BOOST_AUTO_TEST_CASE(int_mode_2_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "0E02"          // LD C, 02h
        "1603"          // LD D, 03h
        "1E04"          // LD E, 04h
        "2605"          // LD H, 05h
        "2E06";         // LD L, 06h
    loadBinary(code, m, 0x0000);

    string ivec = "0002";
    loadBinary(ivec, m, 0x4055);

    string ihnd =
        "21AAAA"        // LD HL, AAAAh
        "ED4D";         // RETI
    loadBinary(ihnd, m, 0x0200);

    startZ80(z80);
    z80.decoder.regs.ir.h = 0x40;
    z80.decoder.regs.iff = IFF1 | IFF2;
    z80.decoder.regs.im = 2;
    runCycles(z80, m, 7);   // Run first instruction.
    runCycles(z80, m, 2);   // Run two cycles of second instruction.
    z80.c &= ~SIGNAL_INT_;  // Signal INT.
    runCycles(z80, m, 4);   // Rest of second instruction.
    runCycles(z80, m, 1);   // Rest of second instruction.
    z80.c ^= SIGNAL_INT_;   // Clear INT.
    runIntCycles(z80, 0x55, 6);  // Place interrupt vector in bus.
    runCycles(z80, m, 13);
    runCycles(z80, m, 10);  // Execute instruction at 0200h
    runCycles(z80, m, 14);  // Execute RETI.
    runCycles(z80, m, 7);   // Execute one more instruction.

    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0006);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iff, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0102);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x03FF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xAAAA);
}

BOOST_AUTO_TEST_CASE(halt_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "76"
        "76"
        "01AAAA";
    loadBinary(code, m, 0x0000);

    string nmihnd =
        "11AAAA"
        "ED45";
    loadBinary(nmihnd, m, 0x0066);

    string inthnd =
        "21AAAA"
        "ED4D";
    loadBinary(inthnd, m, 0x0038);

    startZ80(z80);
    z80.decoder.regs.im = 1;
    z80.decoder.regs.iff = IFF1 | IFF2;

    runCycles(z80, m, 2000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK((z80.c & SIGNAL_HALT_) == 0);

    runCycles(z80, m, 2);
    z80.c &= ~SIGNAL_NMI_;
    runCycles(z80, m, 2);
    z80.c ^= SIGNAL_NMI_;
    runCycles(z80, m, 11);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0066);
    BOOST_CHECK((z80.c & SIGNAL_HALT_) == SIGNAL_HALT_);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0001);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xAAAA);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);

    runCycles(z80, m, 2000);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0002);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xAAAA);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
    BOOST_CHECK((z80.c & SIGNAL_HALT_) == 0);

    runCycles(z80, m, 2);
    z80.c &= ~SIGNAL_INT_;
    runCycles(z80, m, 2);
    z80.c ^= SIGNAL_INT_;
    runCycles(z80, m, 13);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0038);
    BOOST_CHECK((z80.c & SIGNAL_HALT_) == SIGNAL_HALT_);
    runCycles(z80, m, 34);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0005);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xAAAA);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xAAAA);
}

BOOST_AUTO_TEST_CASE(ei_di_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "FB"
        "01AAAA"
        "F3"
        "01CCCC"
        "01DDDD";
    loadBinary(code, m, 0x0000);

    string ihnd =
        "01BBBB"
        "ED4D";
    loadBinary(ihnd, m, 0x0038);

    startZ80(z80);

    z80.decoder.regs.im = 1;
    z80.c &= ~SIGNAL_INT_;
    runCycles(z80, m, 14);
    z80.c |= SIGNAL_INT_;
    runCycles(z80, m, 13);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0038);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
    runCycles(z80, m, 24);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0004);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xBBBB);

    z80.c &= ~SIGNAL_INT_;
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0008);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xCCCC);
    z80.c |= SIGNAL_INT_;
    runCycles(z80, m, 10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x000B);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xDDDD);
}

BOOST_AUTO_TEST_CASE(im_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code =
        "ED46"
        "ED4E"
        "ED56"
        "ED5E"
        "ED66"
        "ED6E"
        "ED76"
        "ED7E";
    loadBinary(code, m, 0x0000);

    startZ80(z80);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 0);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 0);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 1);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 2);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 0);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 0);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 1);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.im, 2);
}
// EOF
// vim: et:sw=4:ts=4:
