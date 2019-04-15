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

/** Z80XXCBPrefixed.h
 *
 * Z80 instruction table for DDCB and FDCB-prefixed opcodes.
 * The type of addressing is determined by the prefix.
 *
 */

#include "Z80Nop.h"

#include "Z80RlcPtrIxIy.h"
#include "Z80RrcPtrIxIy.h"
#include "Z80RlPtrIxIy.h"
#include "Z80RrPtrIxIy.h"
#include "Z80SlaPtrIxIy.h"
#include "Z80SraPtrIxIy.h"
#include "Z80SllPtrIxIy.h"
#include "Z80SrlPtrIxIy.h"

#include "Z80BitNPtrIxIy.h"
#include "Z80ResNPtrIxIy.h"
#include "Z80SetNPtrIxIy.h"

// vim: et:sw=4:ts=4
