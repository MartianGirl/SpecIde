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

    ST_M1_T1_ADDRWR,
    ST_M1_T2_DATARD,
    ST_M1_T3_RFSH1,
    ST_M1_T4_RFSH2,

    ST_NMI_T1_ADDRWR,
    ST_NMI_T2_DATARD,

    ST_INT_T1_ADDRWR,
    ST_INT_T2_DATARD,
    ST_INT_T3_WAIT1,
    ST_INT_T4_WAIT2,

    ST_M2_T1_ADDRWR,
    ST_M2_T2_WAITST,
    ST_M2_T3_DATARD,

    ST_M3_T1_ADDRWR,
    ST_M3_T2_WAITST,
    ST_M3_T3_DATAWR,

    ST_M0_T0_WAITST
};

// vim: et:sw=4:ts=4
