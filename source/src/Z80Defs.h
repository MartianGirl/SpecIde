#pragma once

/** TTLSignal
 *
 * Definitions for TTL logic.
 *
 */

constexpr uint_fast16_t SIGNAL_WR_ = 0x0001;
constexpr uint_fast16_t SIGNAL_RD_ = 0x0002;
constexpr uint_fast16_t SIGNAL_MREQ_ = 0x0004;
constexpr uint_fast16_t SIGNAL_IOREQ_ = 0x0008;
constexpr uint_fast16_t SIGNAL_M1_ = 0x0010;
constexpr uint_fast16_t SIGNAL_RFSH_ = 0x0020;
constexpr uint_fast16_t SIGNAL_HALT_ = 0x0040;
constexpr uint_fast16_t SIGNAL_WAIT_ = 0x0080;
constexpr uint_fast16_t SIGNAL_INT_ = 0x0100;
constexpr uint_fast16_t SIGNAL_NMI_ = 0x0200;
constexpr uint_fast16_t SIGNAL_BUSREQ_ = 0x0400;
constexpr uint_fast16_t SIGNAL_BUSAK_ = 0x0800;
constexpr uint_fast16_t SIGNAL_RESET_ = 0x1000;

enum TTLSignal
{
    TTL_LOGIC_0,
    TTL_LOGIC_1,
    TTL_HIZ = -1
};

// vim: et:sw=4:ts=4
