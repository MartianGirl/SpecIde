#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Z80 test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE(rlca_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E01"          // LD A, 01h
        "07"            // RLCA
        "07"            // RLCA
        "07"            // RLCA
        "07"            // RLCA
        "07"            // RLCA
        "07"            // RLCA
        "07"            // RLCA
        "07"            // RLCA
        "07";           // RLCA

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0200);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0400);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0808);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x1000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2020);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0101);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0200);
}

BOOST_AUTO_TEST_CASE(rla_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E01"          // LD A, 01h
        "17"            // RLA
        "17"            // RLA
        "17"            // RLA
        "17"            // RLA
        "17"            // RLA
        "17"            // RLA
        "17"            // RLA
        "17"            // RLA
        "17";           // RLA

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0200);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0400);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0808);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x1000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2020);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0001);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
}

BOOST_AUTO_TEST_CASE(rrca_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E01"          // LD A, 01h
        "0F"            // RRCA
        "0F"            // RRCA
        "0F"            // RRCA
        "0F"            // RRCA
        "0F"            // RRCA
        "0F"            // RRCA
        "0F"            // RRCA
        "0F"            // RRCA
        "0F";           // RRCA

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8001);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2020);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x1000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0808);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0400);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0200);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8001);
}

BOOST_AUTO_TEST_CASE(rra_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "3E01"          // LD A, 01h
        "1F"            // RRA
        "1F"            // RRA
        "1F"            // RRA
        "1F"            // RRA
        "1F"            // RRA
        "1F"            // RRA
        "1F"            // RRA
        "1F"            // RRA
        "1F";           // RRA

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0001);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x8000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x4000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x2020);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x1000);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0808);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0400);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0200);
    runCycles(z80, m, 4);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x0100);
}

BOOST_AUTO_TEST_CASE(rlc_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00"          // RLC B
        "CB00";         // RLC B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x80);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x01);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(rl_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10"          // RL B
        "CB10";         // RL B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x80);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(rrc_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08"          // RRC B
        "CB08";         // RRC B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x81);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x81);
}

BOOST_AUTO_TEST_CASE(rr_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0601"          // LD B, 01h
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18"          // RR B
        "CB18";         // RR B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x80);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(sla_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "06FF"          // LD B, FFh
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20"          // SLA B
        "CB20";         // SLA B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA9);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xF8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA9);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xF0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA5);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xE0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA1);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xC0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x85);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x81);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x44);
}

BOOST_AUTO_TEST_CASE(sll_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0600"          // LD B, 00h
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30"          // SLL B
        "CB30";         // SLL B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x03);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x04);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x07);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x0F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x0C);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x1F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x3F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x2C);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x28);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAC);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
}

BOOST_AUTO_TEST_CASE(sra_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "0680"          // LD B, 80h
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28"          // SRA B
        "CB28";         // SRA B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xC0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x84);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xE0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA0);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xF0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA4);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xF8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAC);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAC);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
}

BOOST_AUTO_TEST_CASE(srl_r_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "06FF"          // LD B, FFh
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38"          // SRL B
        "CB38";         // SRL B

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 7);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x29);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x3F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x2D);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x1F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x09);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x0F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x0D);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x07);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x01);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x03);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x05);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x01);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.h, 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x44);
}

BOOST_AUTO_TEST_CASE(rlc_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3601"          // LD (HL), 01h
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06"          // RLC (HL)
        "CB06";         // RLC (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x80);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x01);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(rl_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3601"          // LD (HL), 01h
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16"          // RL (HL)
        "CB16";         // RL (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x80);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(rrc_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3601"          // LD (HL), 01h
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E"          // RRC (HL)
        "CB0E";         // RRC (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x81);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x81);
}

BOOST_AUTO_TEST_CASE(rr_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3601"          // LD (HL), 01h
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E"          // RR (HL)
        "CB1E";         // RR (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x80);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x40);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x20);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x08);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x04);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x02);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
}

BOOST_AUTO_TEST_CASE(sla_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "36FF"          // LD (HL), FFh
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26"          // SLA (HL)
        "CB26";         // SLA (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA9);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xF8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA9);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xF0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA5);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xE0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA1);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xC0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x85);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x81);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x44);
}

BOOST_AUTO_TEST_CASE(sll_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3600"          // LD (HL), 00h
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36"          // SLL (HL)
        "CB36";         // SLL (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x03);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x04);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x07);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x0F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x0C);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x1F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x08);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x3F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x2C);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x28);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAC);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
}

BOOST_AUTO_TEST_CASE(sra_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "3680"          // LD (HL), 80h
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E"          // SRA (HL)
        "CB2E";         // SRA (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x80);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xC0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x84);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xE0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA0);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xF0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA4);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xF8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFC);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAC);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFE);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xA8);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAC);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0xAD);
}

BOOST_AUTO_TEST_CASE(srl_ptrhl_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "210001"        // LD HL, 0100h
        "36FF"          // LD (HL), FFh
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E"          // SRL (HL)
        "CB3E";         // SRL (HL)

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0xFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x00);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x7F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x29);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x3F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x2D);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x1F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x09);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x0F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x0D);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x07);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x01);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x03);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x05);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x01);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x45);
    runCycles(z80, m, 15);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x44);
}

BOOST_AUTO_TEST_CASE(shift_rotation_ptrix_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "DD210001"      // LD IX, 0100h
        "DDCB0000"      // RLC (IX + 0), B
        "DDCB0109"      // RRC (IX + 1), C
        "DDCB0212"      // RL (IX + 2), D
        "DDCB031B"      // RR (IX + 3), E
        "DDCB0424"      // SLA (IX + 4), H
        "DDCB052D"      // SRA (IX + 5), L
        "DDCB073F"      // SRL (IX + 7), A
        "DDCB0636";     // SLL (IX + 6)

    string data = "07070F031F7FFF07";

    loadBinary(code, m, 0x0000);
    loadBinary(data, m, 0x0100);
    startZ80(z80);
    z80.decoder.regs.af.l = 0x00;
    runCycles(z80, m, 198);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0E83);
    BOOST_CHECK_EQUAL(z80.decoder.regs.de.w, 0x1F01);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0x3E3F);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x03AD);
    BOOST_CHECK_EQUAL(m.memory[0x0100], 0x0E);
    BOOST_CHECK_EQUAL(m.memory[0x0101], 0x83);
    BOOST_CHECK_EQUAL(m.memory[0x0102], 0x1F);
    BOOST_CHECK_EQUAL(m.memory[0x0103], 0x01);
    BOOST_CHECK_EQUAL(m.memory[0x0104], 0x3E);
    BOOST_CHECK_EQUAL(m.memory[0x0105], 0x3F);
    BOOST_CHECK_EQUAL(m.memory[0x0106], 0xFF);
    BOOST_CHECK_EQUAL(m.memory[0x0107], 0x03);
}

BOOST_AUTO_TEST_CASE(bit_manipulation_reg_test)
{
    Z80 z80;
    Memory m(16, false);

    string code =
        "AF"        // XOR A
        "CBC7"      // SET 0, A
        "CBD7"      // SET 2, A
        "CBE7"      // SET 4, A
        "CBF7"      // SET 6, A
        "47"        // LD B, A
        "4F"        // LD C, A
        "CB80"      // RES 0, B
        "CB90"      // RES 2, B
        "CBA1"      // RES 4, C
        "CBB1"      // RES 6, C
        "CB00"      // RLC B
        "CB01"      // RLC C
        "CB68"      // BIT 5, B
        "CB59"      // BIT 3, C
        "CB70"      // BIT 6, B
        "CB50";     // BIT 2, C

    loadBinary(code, m, 0x0000);
    startZ80(z80);
    runCycles(z80, m, 76);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x5544);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x5005);
    runCycles(z80, m, 16);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.w, 0x550C);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xA00A);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x30);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x18);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x54);
    runCycles(z80, m, 8);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.l, 0x54);
}
// EOF
// vim: et:sw=4:ts=4
