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

/** Z80FDPrefixed.h
 *
 * Z80 instruction table for FD-prefixed opcodes.
 *
 */

#include "Z80Nop.h"

#include "Z80LdAPtrBc.h"
#include "Z80LdAPtrDe.h"
#include "Z80LdAPtrWord.h"

#include "Z80LdPtrBcA.h"
#include "Z80LdPtrDeA.h"
#include "Z80LdPtrWordA.h"

#include "Z80LdRegYByte.h"
#include "Z80LdRegPtrIy.h"
#include "Z80LdRegYRegY.h"

#include "Z80LdPtrIyByte.h"
#include "Z80LdPtrIyReg.h"

#include "Z80LdRegYWord.h"
#include "Z80LdIyPtrWord.h"
#include "Z80LdPtrWordIy.h"
#include "Z80LdSpIy.h"
#include "Z80PushReg.h"
#include "Z80PopReg.h"
#include "Z80PushIy.h"
#include "Z80PopIy.h"

#include "Z80ExDeHl.h"
#include "Z80ExAfAf.h"
#include "Z80Exx.h"
#include "Z80ExPtrSpIy.h"

#include "Z80AddRegY.h"
#include "Z80AdcRegY.h"
#include "Z80SubRegY.h"
#include "Z80SbcRegY.h"
#include "Z80AndRegY.h"
#include "Z80XorRegY.h"
#include "Z80OrRegY.h"
#include "Z80CpRegY.h"
#include "Z80IncRegY.h"
#include "Z80DecRegY.h"

#include "Z80AddByte.h"
#include "Z80AdcByte.h"
#include "Z80SubByte.h"
#include "Z80SbcByte.h"
#include "Z80AndByte.h"
#include "Z80XorByte.h"
#include "Z80OrByte.h"
#include "Z80CpByte.h"

#include "Z80AddPtrIy.h"
#include "Z80AdcPtrIy.h"
#include "Z80SubPtrIy.h"
#include "Z80SbcPtrIy.h"
#include "Z80AndPtrIy.h"
#include "Z80XorPtrIy.h"
#include "Z80OrPtrIy.h"
#include "Z80CpPtrIy.h"
#include "Z80IncPtrIy.h"
#include "Z80DecPtrIy.h"

#include "Z80Daa.h"
#include "Z80Cpl.h"
#include "Z80Ccf.h"
#include "Z80Scf.h"
#include "Z80Halt.h"
#include "Z80Di.h"
#include "Z80Ei.h"

#include "Z80AddIyRegY.h"
#include "Z80IncRegPY.h"
#include "Z80DecRegPY.h"

#include "Z80JpWord.h"
#include "Z80JpCcWord.h"
#include "Z80JrByte.h"
#include "Z80JrCcByte.h"
#include "Z80JpIy.h"
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

#include "Z80PrefixFDCB.h"
#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

// vim: et:sw=4:ts=4
