/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/** Z80
 *
 * Zilog Z80 CPU emulation.
 *
 */

#include <cassert>
#include <cstdint>

#include "Z80Defs.h"
#include "Z80Register.h"

using namespace std;

class Z80
{
    public:
        Z80() :
#include "Z80UnprefixedTable.h"
#include "Z80CBPrefixedTable.h"
#include "Z80DDPrefixedTable.h"
#include "Z80EDPrefixedTable.h"
#include "Z80FDPrefixedTable.h"
#include "Z80XXCBPrefixedTable.h"
        reg8{&bc.b.h, &bc.b.l, &de.b.h, &de.b.l, &hl.b.h, &hl.b.l, nullptr, &af.b.h},
        regx8{&bc.b.h, &bc.b.l, &de.b.h, &de.b.l, &ix.b.h, &ix.b.l, nullptr, &af.b.h},
        regy8{&bc.b.h, &bc.b.l, &de.b.h, &de.b.l, &iy.b.h, &iy.b.l, nullptr, &af.b.h},
        regp{&bc.w, &de.w, &hl.w, &sp.w},
        regp2{&bc.w, &de.w, &hl.w, &af.w},
        regpx{&bc.w, &de.w, &ix.w, &sp.w},
        regpy{&bc.w, &de.w, &iy.w, &sp.w}
        {
            if (!flagsReady)
            {
                loadAddFlags();
                loadSubFlags();
                loadAndFlags();
                loadOrFlags();
                loadXorFlags();
                loadCpFlags();
                loadIncFlags();
                loadDecFlags();
                loadRlFlags();
                loadRrFlags();
                loadRlcFlags();
                loadRrcFlags();
                loadSlaFlags();
                loadSllFlags();
                loadSraFlags();
                loadSrlFlags();
                loadDaaTable();
                flagsReady = true;
            }
        }

        void reset();
        void clock();

        void start();

        void decode(uint_fast8_t byte);
        void startInstruction();

        uint_fast16_t getAddress();

        void readMem(uint_fast8_t byte);
        uint_fast8_t writeMem();

        void readIo(uint_fast8_t byte);
        uint_fast8_t writeIo();

        void cpuProcCycle();

        bool execute();
        bool executeNmi();
        bool executeInt();

        typedef bool (Z80::*Z80Instruction)();
        Z80Instruction unprefixed[0x100];
        Z80Instruction cbprefixed[0x100];
        Z80Instruction ddprefixed[0x100];
        Z80Instruction edprefixed[0x100];
        Z80Instruction fdprefixed[0x100];
        Z80Instruction xxcbprefixed[0x100];

#include "Z80Unprefixed.h"
#include "Z80CBPrefixed.h"
#include "Z80DDPrefixed.h"
#include "Z80EDPrefixed.h"
#include "Z80FDPrefixed.h"
#include "Z80XXCBPrefixed.h"

#include "Z80Nmi.h"
#include "Z80IntMode2.h"

        // Signals
        uint_fast16_t a = 0xFFFF;
        uint_fast8_t d = 0xFF;
        uint_fast16_t c = 0xFFFF;

        // State
        Z80State state = Z80State::ST_RESET;

        // Registers
        Z80Register pc;
        Z80Register ir;
        Z80Register sp;
        Z80Register ix;
        Z80Register iy;

        Z80Register af, af_;
        Z80Register bc, bc_;
        Z80Register de, de_;
        Z80Register hl, hl_;
        Z80Register wz, wz_;

        uint8_t* reg8[8];
        uint8_t* regx8[8];
        uint8_t* regy8[8];
        uint16_t* regp[4];
        uint16_t* regp2[4];
        uint16_t* regpx[4];
        uint16_t* regpy[4];

        // Internals
        Z80Register iReg;
        Z80Register oReg;
        Z80Register addr;
        Z80Register tmp;
        Z80Register acc;

        uint_fast8_t flg;
        uint_fast8_t iff;
        uint_fast8_t im;
        uint_fast8_t intNotReady;
        uint_fast8_t zeroByte = 0x00;  // For OUT(C), 0

        uint_fast8_t x = 0;
        uint_fast8_t y = 0;
        uint_fast8_t z = 0;
        uint_fast8_t p = 0;
        uint_fast8_t q = 0;
        uint_fast8_t opcode = 0x00;
        uint_fast8_t prefix = 0x00;
        uint_fast8_t executionStep = 0;
        uint_fast8_t skipCycles = 0;
        uint_fast8_t memRdCycles = 0;
        uint_fast8_t memWrCycles = 0;
        uint_fast8_t ioRdCycles = 0;
        uint_fast8_t ioWrCycles = 0;
        uint_fast8_t cpuProcCycles = 0;
        uint_fast32_t memAddrMode = 0x00000000;

        bool nmiAccept = false;
        bool nmiProcess = false;
        bool intAccept = false;
        bool intProcess = false;
        bool access = false;
        bool rd = false;
        bool wr = false;

        uint_fast16_t c_d = 0xFFFF;
        uint_fast8_t iff_d = 0x00;
        uint_fast8_t dout = 0xFF;

        // Precalculated flags
        static uint8_t addFlags[2][256][256];
        static uint8_t subFlags[2][256][256];
        static uint8_t andFlags[256][256];
        static uint8_t orFlags[256][256];
        static uint8_t xorFlags[256][256];
        static uint8_t cpFlags[256][256];
        static uint8_t incFlags[256];
        static uint8_t decFlags[256];
        static uint8_t rlFlags[2][256];
        static uint8_t rrFlags[2][256];
        static uint8_t rlcFlags[256];
        static uint8_t rrcFlags[256];
        static uint8_t slaFlags[256];
        static uint8_t sllFlags[256];
        static uint8_t sraFlags[256];
        static uint8_t srlFlags[256];
        static uint16_t daaTable[65536];
        static bool flagsReady;

        void loadAddFlags();
        void loadSubFlags();
        void loadSbcFlags();
        void loadAndFlags();
        void loadOrFlags();
        void loadXorFlags();
        void loadCpFlags();
        void loadIncFlags();
        void loadDecFlags();
        void loadRlFlags();
        void loadRrFlags();
        void loadRlcFlags();
        void loadRrcFlags();
        void loadSlaFlags();
        void loadSllFlags();
        void loadSraFlags();
        void loadSrlFlags();
        void loadDaaTable();
};

// vim: et:sw=4:ts=4
