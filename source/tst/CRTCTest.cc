#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CRTC test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include "CRTC.h"

#include <fstream>

void setDefaults(CRTC& crtc) {

    uint_fast8_t values[] = {
        63,     // R0 - Horizontal total character number
        40,     // R1 - Horizontal displayed character number
        46,     // R2 - Position of horizontal sync pulse
        0x8e,   // R3 - Width of horizontal/vertical sync pulses
        38,     // R4 - Vertical total line character counter
        0,      // R5 - Vertical raster adjust
        25,     // R6 - Vertical displayed character number
        30,     // R7 - Position of vertical sync pulse
        0,      // R8 - Interlaced mode
        7,      // R9 - Maximum raster
        0,      // R10 - Cursor start raster
        0,      // R11 - Cursor end raster
        0x20,   // R12 - Display start address (high)
        0x00,   // R13 - Display start address (low)
        0,      // R14 - Cursor address (High)
        0       // R15 - Cursor address (Low)
    };

    for (uint_fast8_t ii = 0; ii < (sizeof(values) / sizeof(uint_fast8_t)); ++ii) {
        crtc.wrAddress(ii);
        crtc.wrRegister(values[ii]);
    }
}

BOOST_AUTO_TEST_CASE(constructors_test) {

    CRTC crtc;
}

/**
 * CRTC type 0. R9 Update, Case 1 (Table 1, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4 => C9 = 0, C4 = C4 + 1. Video Offset not updated.
 * For C9 = 1..6:
 *  C9 = n, C4 = R4 => C9 = n + 1, C4 = R4. Video Offset not updated.
 * C9 = R9, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_r9update_case1) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 0;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4] + 1);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
            case 7:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 0. R9 Update, Case 2 (Table 2, on page 74).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 * For C9 = 1..R9:
 *  C9 = n, C4 = R4-1 => C9 = n + 1, C4 = R4-1. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_r9update_case2) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 0;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4] - 1);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 0. R9 Update, Case 3 (Table 3, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * For C9 = 1..R9-1:
 *  C9 = n, C4 = R4 => C9 = 0, C4 = R4+1. Video Offset not updated.
 * For C9 = R9:
 *  C9 = R9, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_r9update_case3) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 0;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 7:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4] + 1);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 0. R9 Update, Case 4 (Table 4, on page 75).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_r9update_case4) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 0;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
    }
}

/**
 * CRTC type 0. R9 Update, Case 5 (Table 5, on page 75).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 2 when C0 = 0, C9 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4 => C9 = 1, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_r9update_case5) {

    CRTC crtc;
    setDefaults(crtc);
    crtc.type = 0;

    crtc.rCounter = 0;
    crtc.vCounter = crtc.regs[4];
    crtc.hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.rCounter, 1);
    BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
    BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
}

/**
 * CRTC type 0. R9 Update, Case 6 (Table 6, on page 75).
 * Preconditions: C4 = 0. Previous R9 = 0.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = 0 => C9 = 0, C4 = 0. Video Offset updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_r9update_case6) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 0;

        crtc.rCounter = 0;
        crtc.rMax = crtc.regs[9] = 0;
        crtc.vCounter = 0;
        crtc.vTotal = crtc.regs[4] = 0;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
    }
}

/**
 * CRTC type 1. R9 Update, Case 1 (Table 1, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated.
 * For C9 = 1..R9:
 *  C9 = n, C4 = R4 => C9 = n + 1, C4 = R4. Video Offset updated if C4 == 0.
 */
BOOST_AUTO_TEST_CASE(crtc1_r9update_case1) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 1;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 1. R9 Update, Case 2 (Table 2, on page 74).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset updated.
 * For C9 = 1..R9:
 *  C9 = n, C4 = R4-1 => C9 = n+1, C4 = R4-1. Video Offset updated if C4 == 0.
 */
BOOST_AUTO_TEST_CASE(crtc1_r9update_case2) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 1;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4] - 1);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 1. R9 Update, Case 3 (Table 3, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated.
 */
BOOST_AUTO_TEST_CASE(crtc1_r9update_case3) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 1;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
    }
}

/**
 * CRTC type 1. R9 Update, Case 4 (Table 4, on page 75).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc1_r9update_case4) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 1;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
    }
}

/**
 * CRTC type 1. R9 Update, Case 5 (Table 5, on page 75).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 2 when C0 = 0, C9 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4 => C9 = 1, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc1_r9update_case5) {

    CRTC crtc;
    setDefaults(crtc);
    crtc.type = 1;

    crtc.rCounter = 0;
    crtc.vCounter = crtc.regs[4];
    crtc.hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.rCounter, 1);
    BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
    BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
}

/**
 * CRTC type 1. R9 Update, Case 6 (Table 6, on page 75).
 * Preconditions: C4 = 0. Previous R9 = 0.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * For n = 0:
 *  C9 = 0, C4 = 0 => C9 = 0, C4 = 0. Video Offset updated.
 * For n = 1..7:
 *  C9 = 0, C4 = 0 => C9 = 1, C4 = 0. Video Offset updated (C4 = 1).
 */
BOOST_AUTO_TEST_CASE(crtc1_r9update_case6) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 1;

        crtc.rCounter = 0;
        crtc.rMax = crtc.regs[9] = 0;
        crtc.vCounter = 0;
        crtc.vTotal = crtc.regs[4] = 0;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;

            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;
        }
    }
}

/**
 * CRTC type 2. R9 Update, Case 1 (Table 1, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated if C0<=R1.
 * For C9 = 1..6:
 *  C9 = n, C4 = R4 => C9 = n + 1, C4 = R4. Video Offset updated if C4 == 0.
 * C9 = R9, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated if C0<=R1.
 */
BOOST_AUTO_TEST_CASE(crtc2_r9update_case1) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 2;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
            case 7: // fall-through
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 2. R9 Update, Case 2 (Table 2, on page 74).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 * For C9 = 1..R9:
 *  C9 = n, C4 = R4-1 => C9 = n+1, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc2_r9update_case2) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 2;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4] - 1);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}

/**
 * CRTC type 2. R9 Update, Case 3 (Table 3, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4 => C9 = 0, C4 = 0. Video Offset when C0 <= R1.
 */
BOOST_AUTO_TEST_CASE(crtc2_r9update_case3) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 2;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
    }
}

/**
 * CRTC type 2. R9 Update, Case 4 (Table 4, on page 75).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc2_r9update_case4) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 2;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
    }
}

/**
 * CRTC type 2. R9 Update, Case 5 (Table 5, on page 75).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 2 when C0 = 0, C9 = 0.
 * Outcomes:
 * C9 = 0, C4 = R4 => C9 = 1, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc2_r9update_case5) {

    CRTC crtc;
    setDefaults(crtc);
    crtc.type = 2;

    crtc.rCounter = 0;
    crtc.vCounter = crtc.regs[4];
    crtc.hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.rCounter, 1);
    BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
    BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
}

/**
 * CRTC type 2. R9 Update, Case 6 (Table 6, on page 75).
 * Preconditions: C4 = 0. Previous R9 = 0.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * For n = 0:
 *  C9 = 0, C4 = 0 => C9 = 0, C4 = 0. Video Offset updated.
 * For n = 1..7:
 *  C9 = 0, C4 = 0 => C9 = 1, C4 = 0. Video Offset updated (C4 = 1).
 */
BOOST_AUTO_TEST_CASE(crtc2_r9update_case6) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 2;

        crtc.rCounter = 0;
        crtc.rMax = crtc.regs[9] = 0;
        crtc.vCounter = 0;
        crtc.vTotal = crtc.regs[4] = 0;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;

            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;
        }
    }
}

/**
 * CRTC type 3, 4. R9 Update, Case 1 (Table 1, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * For C9 = 0..R9:
 *  C9 = 0, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated.
 */
BOOST_AUTO_TEST_CASE(crtc34_r9update_case1) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 4;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
    }
}

/**
 * CRTC type 3, 4. R9 Update, Case 2 (Table 2, on page 74).
 * Preconditions: C4 = R4-1. Previous R9 = 7.
 * Test: Set R9 = 0 when C0 = 0.
 * Outcomes:
 * For C9 = 0..R9:
 * C9 = n, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc34_r9update_case2) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 4;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
    }
}

/**
 * CRTC type 3, 4. R9 Update, Case 3 (Table 3, on page 74).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4 => C9 = 0, C4 = 0. Video Offset updated.
 */
BOOST_AUTO_TEST_CASE(crtc34_r9update_case3) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 4;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4];
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
    }
}

/**
 * CRTC type 3, 4. R9 Update, Case 4 (Table 4, on page 75).
 * Preconditions: C4 != R4. Previous R9 = 7.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * C9 = n, C4 = R4-1 => C9 = 0, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc34_r9update_case4) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 4;

        crtc.rCounter = ii;
        crtc.vCounter = crtc.regs[4] - 1;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
    }
}

/**
 * CRTC type 3, 4. R9 Update, Case 5 (Table 5, on page 75).
 * Preconditions: C4 = R4. Previous R9 = 7.
 * Test: Set R9 = 2 when C0 = 0, C9 = 5.
 * Outcomes:
 * C9 = 5, C4 = R4 => C9 = 1, C4 = R4. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc34_r9update_case5) {

    CRTC crtc;
    setDefaults(crtc);
    crtc.type = 4;

    crtc.rCounter = 5;
    crtc.vCounter = crtc.regs[4];
    crtc.hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.rCounter, 0);
    BOOST_CHECK_EQUAL(crtc.vCounter, 0);
    BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
}

/**
 * CRTC type 3, 4. R9 Update, Case 6 (Table 6, on page 75).
 * Preconditions: C4 = 0. Previous R9 = 0.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * For n = 0:
 *  C9 = 0, C4 = 0 => C9 = 0, C4 = 0. Video Offset updated.
 * For n = 1..7:
 *  C9 = 0, C4 = 0 => C9 = 1, C4 = 0. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc34_r9update_case6) {

    for (uint_fast8_t ii = 0; ii < 8; ii++) {
        CRTC crtc;
        setDefaults(crtc);
        crtc.type = 4;

        crtc.rCounter = 0;
        crtc.rMax = crtc.regs[9] = 0;
        crtc.vCounter = 0;
        crtc.vTotal = crtc.regs[4] = 0;
        crtc.hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, true);
                break;

            default:
                BOOST_CHECK_EQUAL(crtc.rCounter, 1);
                BOOST_CHECK_EQUAL(crtc.vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.updateVideoOffset, false);
                break;
        }
    }
}
// EOF
// vim: et:sw=4:ts=4
