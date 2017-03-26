//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

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

void runIntCycles(Z80& z80, uint8_t data, size_t cycles)
{
    for (size_t i = 0; i != cycles; ++i)
    {
        z80.clock();
        z80.d = data;
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
    BOOST_CHECK_EQUAL(z80.nmiProcess, false);
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

    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0004);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iff, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xBBCC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFFFF);
}
