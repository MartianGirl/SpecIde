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
        "010000"        // LD BC, 0000h
        "3EAA"          // LD A, AAh
        "C34000"        // JP 0040h
        "0100EE";
    loadBinary(code_0000h, m, 0x0000);

    string code_0040h =
        "010100"        // LD BC, 0001h
        "A7"            // AND A (A = AAh, NZ, NC)
        "C28000"        // JP NZ, 0080h
        "0101EE";
    loadBinary(code_0040h, m, 0x0040);

    string code_0080h =
        "010200"        // LD BC, 0002h
        "AF"            // XOR A (A = 00h, Z, NC)
        "CAC000"        // JP Z, 00C0h
        "0102EE";
    loadBinary(code_0080h, m, 0x0080);

    string code_00C0h =
        "010300"        // LD BC, 0003h
        "2F"            // CPL (A = FFh, NZ, NC)
        "D20001"        // JP NC, 0100h
        "0103EE";
    loadBinary(code_00C0h, m, 0x00C0);

    string code_0100h =
        "010400"        // LD BC, 0004h
        "37"            // SCF (A = FFh, NZ, C)
        "DA4001"        // JP C, 0140h
        "0104EE";
    loadBinary(code_0100h, m, 0x0100);

    string code_0140h =
        "010500"        // LD BC, 0005h
        "E601"          // AND 01h (A = 01h, NZ, NC, PO)
        "E28001"        // JP PO, 0180h
        "0105EE";
    loadBinary(code_0140h, m, 0x0140);

    string code_0180h =
        "010600"        // LD BC, 0006h
        "F602"          // OR 02h (A = 03h, NZ, NC, PE)
        "EAC001"        // JP PE, 01C0h
        "0106EE";
    loadBinary(code_0180h, m, 0x0180);

    string code_01C0h =
        "010700"        // LD BC, 0007h
        "3C"            // INC A (A = 04h, NZ, NC, PO, P)
        "F20002"        // JP P, 0200h
        "0107EE";
    loadBinary(code_01C0h, m, 0x01C0);

    string code_0200h =
        "010800"        // LD BC, 0008h
        "ED44"          // NEG (A = FCh, NZ, NC, PE, M)
        "FA4002"        // JP M, 0240h
        "0108EE";
    loadBinary(code_0200h, m, 0x0200);
   
    string code_0240h =
        "01AAAA";       // LD BC, AAAAh
    loadBinary(code_0240h, m, 0x0240);

    startZ80(z80);
    runCycles(z80, m, 239);  // Run all the code.
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0243);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
}

BOOST_AUTO_TEST_CASE(jr_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "010000"        // LD BC, 0000h
        "3EAA"          // LD A, AAh
        "1879"          // JR 79h (07h + 79h = 80h)
        "0100EE";
    loadBinary(code_0000h, m, 0x0000);

    string code_0080h =
        "010100"        // LD BC, 0001h
        "A7"            // AND A (A = AAh, NZ, NC)
        "20BA"          // JR NZ, BAh (86h - 46h = 40h)
        "0101EE";
    loadBinary(code_0080h, m, 0x0080);

    string code_0040h =
        "010200"        // LD BC, 0002h
        "AF"            // XOR A (A = 00h, Z, NC)
        "287A"          // JR Z, 79h
        "0102EE";
    loadBinary(code_0040h, m, 0x0040);

    string code_00C0h =
        "010300"        // LD BC, 0003h
        "2F"            // CPL (A = FFh, Z, NC)
        "307A"          // JR NC, 79h
        "0103EE";
    loadBinary(code_00C0h, m, 0x00C0);

    string code_0140h =
        "010400"        // LD BC, 0004h
        "37"            // SCF (A = FFh, Z, C)
        "38BA"          // JR C, BAh
        "0104EE";
    loadBinary(code_0140h, m, 0x0140);

    string code_0100h =
        "B7"            // OR A (NZ, NC)
        "37"            // SCF (NZ, C)
        "30FE"          // JR NC, FEh (reexecute)
        "28FE"          // JR Z, FEh
        "AF"            // XOR A (A = 00h, Z, NC)
        "20FE"          // JR NZ, FEh
        "38FE"          // JR C, FEh
        "01AAAA";       // LD BC, AAAAh
    loadBinary(code_0100h, m, 0x0100);

    startZ80(z80);
    runCycles(z80, m, 29);  // JR n
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0080);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0000);
    runCycles(z80, m, 26);  // JR NZ, n
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0040);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0001);
    runCycles(z80, m, 26);  // JR Z, n
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x00C0);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0002);
    runCycles(z80, m, 26);  // JR NC, n
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0140);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0003);
    runCycles(z80, m, 26);  // JR C, n
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0100);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x0004);
    runCycles(z80, m, 8);   // OR A, SCF
    runCycles(z80, m, 7);   // Negative conditions
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0104);
    runCycles(z80, m, 7);   // Negative conditions
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0106);
    runCycles(z80, m, 4);   // XOR A
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0x00);
    runCycles(z80, m, 7);   // Negative conditions
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0109);
    runCycles(z80, m, 7);   // Negative conditions
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x010B);
    runCycles(z80, m, 10);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x010E);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
}

BOOST_AUTO_TEST_CASE(jp_hl_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "2100FF"        // LD HL, FF00h
        "E9";           // JP (HL)
    loadBinary(code_0000h, m, 0x0000);

    startZ80(z80);
    runCycles(z80, m, 14);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0xFF00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.hl.w, 0xFF00);
}

BOOST_AUTO_TEST_CASE(jp_ix_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "DD2100FF"      // LD IX, FF00h
        "DDE9";         // JP (IX)
    loadBinary(code_0000h, m, 0x0000);

    startZ80(z80);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0xFF00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.ix.w, 0xFF00);
}

BOOST_AUTO_TEST_CASE(jp_iy_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "FD2100FF"      // LD IY, FF00h
        "FDE9";         // JP (IY)
    loadBinary(code_0000h, m, 0x0000);

    startZ80(z80);
    runCycles(z80, m, 22);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0xFF00);
    BOOST_CHECK_EQUAL(z80.decoder.regs.iy.w, 0xFF00);
}

BOOST_AUTO_TEST_CASE(djnz_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "01000A"        // LD BC, 0A00h
        "0C"            // INC C
        "10FD"          // DJNZ FDh
        "3EAA";         // LD A, AAh

    loadBinary(code_0000h, m, 0x0000);

    startZ80(z80);
    runCycles(z80, m, 182);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0008);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0x000A);
    BOOST_CHECK_EQUAL(z80.decoder.regs.af.h, 0xAA);
}

BOOST_AUTO_TEST_CASE(call_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "CD0002";       // CALL 0200h
    loadBinary(code_0000h, m, 0x0000);

    string code_0200h =
        "01AAAA"        // LD BC, AAAAh
        "C9";           // RET
    loadBinary(code_0200h, m, 0x0200);

    startZ80(z80);
    runCycles(z80, m, 17);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0200);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFD);
    runCycles(z80, m, 20);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0003);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
}

BOOST_AUTO_TEST_CASE(call_cc_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "010001"        // LD BC, 0000h
        "AF"            // XOR A (PZE0)
        "3C"            // INC A (P0O0)
        "C40001"        // CALL NZ, 0100h
        "010002"        // LD BC, 0001h
        "CC2001"        // CALL Z, 0120h
        "010003"        // LD BC, 0002h
        "D44001"        // CALL NC, 0140h
        "010004"        // LD BC, 0003h
        "DC6001"        // CALL C, 0160h
        "010005"        // LD BC, 0004h
        "E48001"        // CALL PO, 0180h
        "010006"        // LD BC, 0005h
        "ECA001"        // CALL PE, 01A0h
        "010007"        // LD BC, 0006h
        "F4C001"        // CALL P, 01C0h
        "010008"        // LD BC, 0007h
        "FCE001"        // CALL M, 01E0h
        "CC0000"        // CALL Z, 0000h
        "01AAAA";       // LD BC, AAAAh
    loadBinary(code_0000h, m, 0x0000);

    string code_0100h =
        "AF"            // XOR A (PZE0)
        "C8"            // RET Z
        "0101EE";
    loadBinary(code_0100h, m, 0x0100);

    string code_0120h =
        "AF"            // XOR A (PZE0)
        "D0"            // RET NC
        "0102EE";
    loadBinary(code_0120h, m, 0x0120);

    string code_0140h =
        "37"            // SCF (PZEC)
        "D8"            // RET C
        "0103EE";
    loadBinary(code_0140h, m, 0x0140);

    string code_0160h =
        "B0"            // OR B (P0O0)
        "E0"            // RET PO
        "0104EE";
    loadBinary(code_0160h, m, 0x0160);

    string code_0180h =
        "AF"            // XOR A (PZE0)
        "E8"            // RET PE
        "0105EE";
    loadBinary(code_0180h, m, 0x0180);

    string code_01A0h =
        "AF"            // XOR A (PZE0)
        "F0"            // RET P
        "0106EE";
    loadBinary(code_01A0h, m, 0x01A0);

    string code_01C0h =
        "3D"            // DEC D (M0E0)
        "F8"            // RET M
        "0107EE";
    loadBinary(code_01C0h, m, 0x01C0);

    string code_01E0h =
        "3D"            // DEC D (M0O0)
        "C0"            // RET NZ
        "0108EE";
    loadBinary(code_01E0h, m, 0x01E0);
   
    startZ80(z80);
    runCycles(z80, m, 364);  // Run all the code.
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0038);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
}

BOOST_AUTO_TEST_CASE(rst_test)
{
    // Create a Z80 and some memory.
    Z80 z80;
    Memory m(16, false);

    string code_0000h =
        "CF01AAAA"      // RST 08h + LD BC, AAAAh
        "00000000"
        "D7C90000"      // RST 10h + RET
        "00000000"
        "DFC90000"      // RST 18h + RET
        "00000000"
        "E7C90000"      // RST 20h + RET
        "00000000"
        "EFC90000"      // RST 28h + RET
        "00000000"
        "F7C90000"      // RST 30h + RET
        "00000000"
        "FFC90000"      // RST 38h + RET
        "00000000"
        "C9000000";     // RET
    loadBinary(code_0000h, m, 0x0000);

    startZ80(z80);
    runCycles(z80, m, 157);
    BOOST_CHECK_EQUAL(z80.decoder.regs.pc.w, 0x0004);
    BOOST_CHECK_EQUAL(z80.decoder.regs.sp.w, 0xFFFF);
    BOOST_CHECK_EQUAL(z80.decoder.regs.bc.w, 0xAAAA);
}

// EOF
// vim: et:sw=4:ts=4:
