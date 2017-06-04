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
    ST_RESET,           // 0

    ST_OCF_T1_ADDRWR,   // 1
    ST_OCF_T2_DATARD,   // 2
    ST_OCF_T3_RFSH1,    // 3
    ST_OCF_T4_RFSH2,    // 4

    ST_NMI_T1_ADDRWR,   // 5
    ST_NMI_T2_DATARD,   // 6

    ST_INT_T1_ADDRWR,   // 7
    ST_INT_T2_DATARD,   // 8
    ST_INT_T3_WAIT1,    // 9
    ST_INT_T4_WAIT2,    // 10

    ST_MEMRD_T1_ADDRWR, // 11
    ST_MEMRD_T2_WAITST, // 12
    ST_MEMRD_T3_DATARD, // 13

    ST_MEMWR_T1_ADDRWR, // 14
    ST_MEMWR_T2_WAITST, // 15
    ST_MEMWR_T3_DATAWR, // 16

    ST_IORD_T1_ADDRWR,
    ST_IORD_T2_IORQ,
    ST_IORD_TW_WAITST,
    ST_IORD_T3_DATARD,

    ST_IOWR_T1_ADDRWR,
    ST_IOWR_T2_IORQ,
    ST_IOWR_TW_WAITST,
    ST_IOWR_T3_DATAWR,

    ST_CPU_T0_CPUPROC,
    ST_CPU_T0_WAITST
};

// vim: et:sw=4:ts=4
