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

/** Z80Decoder
 *
 * Z80 Instruction decoder.
 *
 */

#include <cassert>
#include <cstdint>

#include "Z80Defs.h"
#include "Z80Register.h"
#include "Z80RegisterSet.h"
#include "Z80Unprefixed.h"
#include "Z80CBPrefixed.h"
#include "Z80DDPrefixed.h"
#include "Z80EDPrefixed.h"
#include "Z80FDPrefixed.h"
#include "Z80XXCBPrefixed.h"

#include "Z80Nmi.h"
#include "Z80IntMode2.h"

using namespace std;

class Z80Decoder
{
    public:
        Z80Decoder();


        void reset();

        Z80RegisterSet regs;

        Z80Unprefixed unprefixed;
        Z80CBPrefixed CBPrefixed;
        Z80DDPrefixed DDPrefixed;
        Z80EDPrefixed EDPrefixed;
        Z80FDPrefixed FDPrefixed;
        Z80XXCBPrefixed XXCBPrefixed;

        Z80Nmi z80Nmi;
        Z80IntMode2 z80IntMode2;
};

// vim: et:sw=4:ts=4
