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

/** Z80Unprefixed.h
 *
 * Z80 instruction table for unprefixed opcodes.
 *
 */

#include "Z80Nop.h"

#include "Z80LdAPtrBc.h"
#include "Z80LdAPtrDe.h"
#include "Z80LdAPtrWord.h"

#include "Z80LdPtrBcA.h"
#include "Z80LdPtrDeA.h"
#include "Z80LdPtrWordA.h"

#include "Z80LdRegByte.h"
#include "Z80LdRegPtrHl.h"
#include "Z80LdRegReg.h"

#include "Z80LdPtrHlByte.h"
#include "Z80LdPtrHlReg.h"

#include "Z80LdRegWord.h"
#include "Z80LdHlPtrWord.h"
#include "Z80LdPtrWordHl.h"
#include "Z80LdSpHl.h"
#include "Z80PushReg.h"
#include "Z80PopReg.h"

#include "Z80ExDeHl.h"
#include "Z80ExAfAf.h"
#include "Z80Exx.h"
#include "Z80ExPtrSpHl.h"

#include "Z80AddReg.h"
#include "Z80AdcReg.h"
#include "Z80SubReg.h"
#include "Z80SbcReg.h"
#include "Z80AndReg.h"
#include "Z80XorReg.h"
#include "Z80OrReg.h"
#include "Z80CpReg.h"
#include "Z80IncReg.h"
#include "Z80DecReg.h"

#include "Z80AddByte.h"
#include "Z80AdcByte.h"
#include "Z80SubByte.h"
#include "Z80SbcByte.h"
#include "Z80AndByte.h"
#include "Z80XorByte.h"
#include "Z80OrByte.h"
#include "Z80CpByte.h"

#include "Z80AddPtrHl.h"
#include "Z80AdcPtrHl.h"
#include "Z80SubPtrHl.h"
#include "Z80SbcPtrHl.h"
#include "Z80AndPtrHl.h"
#include "Z80XorPtrHl.h"
#include "Z80OrPtrHl.h"
#include "Z80CpPtrHl.h"
#include "Z80IncPtrHl.h"
#include "Z80DecPtrHl.h"

#include "Z80Daa.h"
#include "Z80Cpl.h"
#include "Z80Ccf.h"
#include "Z80Scf.h"
#include "Z80Halt.h"
#include "Z80Di.h"
#include "Z80Ei.h"

#include "Z80AddHlReg.h"
#include "Z80IncRegP.h"
#include "Z80DecRegP.h"

#include "Z80JpWord.h"
#include "Z80JpCcWord.h"
#include "Z80JrByte.h"
#include "Z80JrCcByte.h"
#include "Z80JpHl.h"
#include "Z80Djnz.h"

#include "Z80Call.h"
#include "Z80Ret.h"
#include "Z80CallCc.h"
#include "Z80RetCc.h"
#include "Z80Rst.h"

#include "Z80InAPtrByte.h"
#include "Z80OutPtrByteA.h"

#include "Z80Rlca.h"
#include "Z80Rla.h"
#include "Z80Rrca.h"
#include "Z80Rra.h"

#include "Z80PrefixCB.h"
#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

// vim: et:sw=4:ts=4
