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

/** Z80Instruction
 *
 * Base class for all instruction functors.
 *
 */

#include "Z80Defs.h"
#include "Z80RegisterSet.h"

class Z80Instruction
{
    public:
        Z80Instruction() {}
        virtual bool operator()(Z80RegisterSet* r) = 0;
};

// vim: et:sw=4:ts=4
