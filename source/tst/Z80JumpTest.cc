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

BOOST_AUTO_TEST_CASE(jp_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "0601"          // LD B, 01h
        "0E02"          // LD C, 02h
        "1603"          // LD D, 03h
        "1E04"          // LD E, 04h
        "2605"          // LD H, 05h
        "2E06"          // LD L, 06h
        "C30001";       // JP 0100h

    loadBinary(code_0000h, m, 0x0000);

    string code_0100h =
        "06BB"          // LD B, BBh
        "0ECC"          // LD C, CCh
        "16DD"          // LD D, DDh
        "1EEE";         // LD E, EEh

    loadBinary(code_0100h, m, 0x0100);

    startZ80(z80);
    runCycles(z80, m, 80);  // Run all the code.
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0108);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xBBCC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0xDDEE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x0506);
}
