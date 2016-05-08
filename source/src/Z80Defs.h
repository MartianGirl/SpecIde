#pragma once

/** TTLSignal
 *
 * Definitions for TTL logic.
 *
 */

#define SIGNAL_WR_      0x0001
#define SIGNAL_RD_      0x0002
#define SIGNAL_MREQ_    0x0004
#define SIGNAL_IOREQ_   0x0008
#define SIGNAL_M1_      0x0010
#define SIGNAL_RFSH_    0x0020
#define SIGNAL_HALT_    0x0040
#define SIGNAL_WAIT_    0x0080
#define SIGNAL_INT_     0x0100
#define SIGNAL_NMI_     0x0200
#define SIGNAL_BUSREQ_  0x0400
#define SIGNAL_BUSAK_   0x0800
#define SIGNAL_RESET_   0x1000

enum TTLSignal
{
    TTL_LOGIC_0,
    TTL_LOGIC_1,
    TTL_HIZ = -1;
};

// vim: et:sw=4:ts=4
