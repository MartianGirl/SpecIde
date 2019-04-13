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

/** Z80DDPrefixed.h
 *
 * Z80 instruction table for DD-prefixed opcodes.
 *
 */

#include "Z80Nop.h"

#include "Z80LdAPtrBc.h"
#include "Z80LdAPtrDe.h"
#include "Z80LdAPtrWord.h"

#include "Z80LdPtrBcA.h"
#include "Z80LdPtrDeA.h"
#include "Z80LdPtrWordA.h"

#include "Z80LdRegXByte.h"
#include "Z80LdRegPtrIx.h"
#include "Z80LdRegXRegX.h"

#include "Z80LdPtrIxByte.h"
#include "Z80LdPtrIxReg.h"

#include "Z80LdRegXWord.h"
#include "Z80LdIxPtrWord.h"
#include "Z80LdPtrWordIx.h"
#include "Z80LdSpIx.h"
#include "Z80PushReg.h"
#include "Z80PopReg.h"
#include "Z80PushIx.h"
#include "Z80PopIx.h"

#include "Z80ExDeHl.h"
#include "Z80ExAfAf.h"
#include "Z80Exx.h"
#include "Z80ExPtrSpIx.h"

#include "Z80AddRegX.h"
#include "Z80AdcRegX.h"
#include "Z80SubRegX.h"
#include "Z80SbcRegX.h"
#include "Z80AndRegX.h"
#include "Z80XorRegX.h"
#include "Z80OrRegX.h"
#include "Z80CpRegX.h"
#include "Z80IncRegX.h"
#include "Z80DecRegX.h"

#include "Z80AddByte.h"
#include "Z80AdcByte.h"
#include "Z80SubByte.h"
#include "Z80SbcByte.h"
#include "Z80AndByte.h"
#include "Z80XorByte.h"
#include "Z80OrByte.h"
#include "Z80CpByte.h"

#include "Z80AddPtrIx.h"
#include "Z80AdcPtrIx.h"
#include "Z80SubPtrIx.h"
#include "Z80SbcPtrIx.h"
#include "Z80AndPtrIx.h"
#include "Z80XorPtrIx.h"
#include "Z80OrPtrIx.h"
#include "Z80CpPtrIx.h"
#include "Z80IncPtrIx.h"
#include "Z80DecPtrIx.h"

#include "Z80Daa.h"
#include "Z80Cpl.h"
#include "Z80Ccf.h"
#include "Z80Scf.h"
#include "Z80Halt.h"
#include "Z80Di.h"
#include "Z80Ei.h"

#include "Z80AddIxRegX.h"
#include "Z80IncRegPX.h"
#include "Z80DecRegPX.h"

#include "Z80JpWord.h"
#include "Z80JpCcWord.h"
#include "Z80JrByte.h"
#include "Z80JrCcByte.h"
#include "Z80JpIx.h"
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

#include "Z80PrefixDDCB.h"
#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

// vim: et:sw=4:ts=4
