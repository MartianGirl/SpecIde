#pragma once

/** TTLSignal
 *
 * Definitions for TTL logic.
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

//enum TTLSignal
//{
    //TTL_LOGIC_0,
    //TTL_LOGIC_1,
    //TTL_HIZ = -1
//};

enum class Z80State
{
    ST_RESET,

    ST_M1_T1_ADDRWR,
    ST_M1_T2_DATARD,
    ST_M1_T3_RFSH1,
    ST_M1_T4_RFSH2,

    ST_M2_T1_ADDRWR,
    ST_M2_T2_WAITST,
    ST_M2_T3_DATARD,

    ST_M3_T1_ADDRWR,
    ST_M3_T2_WAITST,
    ST_M3_T3_DATAWR
};

// vim: et:sw=4:ts=4
