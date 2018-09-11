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

/** Z80Defs
 *
 * Definitions for Z80 signals, states, etc.
 *
 */

// All the signals are active low (hence the _ in the name)
constexpr uint_fast16_t SIGNAL_WR_ = 0x0001;
constexpr uint_fast16_t SIGNAL_RD_ = 0x0002;
constexpr uint_fast16_t SIGNAL_MREQ_ = 0x0004;
constexpr uint_fast16_t SIGNAL_IORQ_ = 0x0008;
constexpr uint_fast16_t SIGNAL_M1_ = 0x0010;
constexpr uint_fast16_t SIGNAL_RFSH_ = 0x0020;
constexpr uint_fast16_t SIGNAL_HALT_ = 0x0040;
constexpr uint_fast16_t SIGNAL_WAIT_ = 0x0080;
constexpr uint_fast16_t SIGNAL_INT_ = 0x0100;
constexpr uint_fast16_t SIGNAL_NMI_ = 0x0200;
constexpr uint_fast16_t SIGNAL_BUSRQ_ = 0x0400;
constexpr uint_fast16_t SIGNAL_BUSAK_ = 0x0800;
constexpr uint_fast16_t SIGNAL_RESET_ = 0x1000;

constexpr uint_fast8_t FLAG_C = 0x01;
constexpr uint_fast8_t FLAG_N = 0x02;
constexpr uint_fast8_t FLAG_PV = 0x04;
constexpr uint_fast8_t FLAG_3 = 0x08;
constexpr uint_fast8_t FLAG_H = 0x10;
constexpr uint_fast8_t FLAG_5 = 0x20;
constexpr uint_fast8_t FLAG_Z = 0x40;
constexpr uint_fast8_t FLAG_S = 0x80;

constexpr uint_fast8_t IFF1 = 0x01;
constexpr uint_fast8_t IFF2 = 0x04;
constexpr uint_fast8_t HALT = 0x80;

constexpr uint_fast8_t PREFIX_NO = 0x00;
constexpr uint_fast8_t PREFIX_CB = 0x01;
constexpr uint_fast8_t PREFIX_DD = 0x02;
constexpr uint_fast8_t PREFIX_ED = 0x04;
constexpr uint_fast8_t PREFIX_FD = 0x08;

enum class Z80State
{
    ST_RESET,

    ST_OCF_T1H_ADDRWR,
    ST_OCF_T1L_ADDRWR,
    ST_OCF_T2H_DATARD,
    ST_OCF_T2L_DATARD,
    ST_OCF_T3H_RFSH1,
    ST_OCF_T3L_RFSH1,
    ST_OCF_T4H_RFSH2,
    ST_OCF_T4L_RFSH2,

    ST_NMI_T1H_ADDRWR,
    ST_NMI_T1L_ADDRWR,
    ST_NMI_T2H_DATARD,
    ST_NMI_T2L_DATARD,

    ST_INT_T1H_ADDRWR,
    ST_INT_T1L_ADDRWR,
    ST_INT_T2H_DATARD,
    ST_INT_T2L_DATARD,
    ST_INT_T3H_WAIT1,
    ST_INT_T3L_WAIT1,
    ST_INT_T4H_WAIT2,
    ST_INT_T4L_WAIT2,

    ST_MEMRD_T1H_ADDRWR,
    ST_MEMRD_T1L_ADDRWR,
    ST_MEMRD_T2H_WAITST,
    ST_MEMRD_T2L_WAITST,
    ST_MEMRD_T3H_DATARD,
    ST_MEMRD_T3L_DATARD,

    ST_MEMWR_T1H_ADDRWR,
    ST_MEMWR_T1L_ADDRWR,
    ST_MEMWR_T2H_WAITST,
    ST_MEMWR_T2L_WAITST,
    ST_MEMWR_T3H_DATAWR,
    ST_MEMWR_T3L_DATAWR,

    ST_IORD_T1H_ADDRWR,
    ST_IORD_T1L_ADDRWR,
    ST_IORD_T2H_IORQ,
    ST_IORD_T2L_IORQ,
    ST_IORD_TWH_WAITST,
    ST_IORD_TWL_WAITST,
    ST_IORD_T3H_DATARD,
    ST_IORD_T3L_DATARD,

    ST_IOWR_T1H_ADDRWR,
    ST_IOWR_T1L_ADDRWR,
    ST_IOWR_T2H_IORQ,
    ST_IOWR_T2L_IORQ,
    ST_IOWR_TWH_WAITST,
    ST_IOWR_TWL_WAITST,
    ST_IOWR_T3H_DATAWR,
    ST_IOWR_T3L_DATAWR,

    ST_CPU_T0H_CPUPROC,
    ST_CPU_T0L_CPUPROC,
    ST_CPU_T0H_WAITST,
    ST_CPU_T0L_WAITST
};

// vim: et:sw=4:ts=4
