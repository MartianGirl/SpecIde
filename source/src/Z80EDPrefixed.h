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

/** Z80EDPrefixed.h
 *
 * Z80 instruction table for ED-prefixed opcodes.
 *
 */

#include "Z80Nop.h"

#include "Z80LdAI.h"
#include "Z80LdAR.h"
#include "Z80LdIA.h"
#include "Z80LdRA.h"

#include "Z80LdRegPtrWord.h"
#include "Z80LdPtrWordReg.h"

#include "Z80Ldi.h"
#include "Z80Ldir.h"
#include "Z80Ldd.h"
#include "Z80Lddr.h"
#include "Z80Cpi.h"
#include "Z80Cpir.h"
#include "Z80Cpd.h"
#include "Z80Cpdr.h"

#include "Z80Neg.h"

#include "Z80Im.h"
#include "Z80RetI.h"

#include "Z80InRegPtrC.h"
#include "Z80OutPtrCReg.h"
#include "Z80Ini.h"
#include "Z80Inir.h"
#include "Z80Ind.h"
#include "Z80Indr.h"
#include "Z80Outi.h"
#include "Z80Otir.h"
#include "Z80Outd.h"
#include "Z80Otdr.h"

#include "Z80AdcHlReg.h"
#include "Z80SbcHlReg.h"

#include "Z80Rld.h"
#include "Z80Rrd.h"

// vim: et:sw=4:ts=4
