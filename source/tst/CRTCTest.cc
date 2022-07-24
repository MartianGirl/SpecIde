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

void advanceToCharacter(CRTC& crtc, uint_fast8_t n) {

    uint32_t counter = 0;
    do {
        crtc.clock();
        /*
        std::cout << " C0: " << static_cast<uint32_t>(crtc.c0_hCounter)
            << " C4: " << static_cast<uint32_t>(crtc.c4_vCounter)
            << " C9: " << static_cast<uint32_t>(crtc.c9_rCounter)
            << " C5: " << static_cast<uint32_t>(crtc.c5_aCounter)
            << " VP: " << static_cast<uint32_t>(crtc.lineAddress)
            << std::endl;
        */
        ++counter;
    } while (crtc.c0_hCounter != n);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4] + 1);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
                break;
            case 7:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4] - 1);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 7:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4] + 1);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

    crtc.c9_rCounter = 0;
    crtc.c4_vCounter = crtc.regs[4];
    crtc.c0_hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 1);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = 0;
        crtc.r9_rMaxAddress = crtc.regs[9] = 0;
        crtc.c4_vCounter = 0;
        crtc.r4_vTotal = crtc.regs[4] = 0;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4] - 1);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

    crtc.c9_rCounter = 0;
    crtc.c4_vCounter = crtc.regs[4];
    crtc.c0_hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 1);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = 0;
        crtc.r9_rMaxAddress = crtc.regs[9] = 0;
        crtc.c4_vCounter = 0;
        crtc.r4_vTotal = crtc.regs[4] = 0;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;

            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
            case 7: // fall-through
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
                break;
            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, ii + 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4] - 1);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

    crtc.c9_rCounter = 0;
    crtc.c4_vCounter = crtc.regs[4];
    crtc.c0_hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 1);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = 0;
        crtc.r9_rMaxAddress = crtc.regs[9] = 0;
        crtc.c4_vCounter = 0;
        crtc.r4_vTotal = crtc.regs[4] = 0;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;

            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(0);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4];
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = ii;
        crtc.c4_vCounter = crtc.regs[4] - 1;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, crtc.regs[4]);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
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

    crtc.c9_rCounter = 5;
    crtc.c4_vCounter = crtc.regs[4];
    crtc.c0_hCounter = crtc.regs[0] - 1;
    crtc.clock();

    crtc.wrAddress(9); crtc.wrRegister(2);
    crtc.clock();

    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
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

        crtc.c9_rCounter = 0;
        crtc.r9_rMaxAddress = crtc.regs[9] = 0;
        crtc.c4_vCounter = 0;
        crtc.r4_vTotal = crtc.regs[4] = 0;
        crtc.c0_hCounter = crtc.regs[0] - 1;
        crtc.clock();

        crtc.wrAddress(9); crtc.wrRegister(ii);
        crtc.clock();

        switch (ii) {
            case 0:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
                break;

            default:
                BOOST_CHECK_EQUAL(crtc.c9_rCounter, 1);
                BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
                BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
                break;
        }
    }
}

/**
 * CRTC type 0. Additional line management. Page 77.
 * Preconditions: R4=10, R5=16, R9=3, R1=40, R0=64.
 * Test: Set R9 = 0..R9 when C0 = 0.
 * Outcomes:
 * For n = 0:
 *  C9 = 0, C4 = 0 => C9 = 0, C4 = 0. Video Offset updated.
 * For n = 1..7:
 *  C9 = 0, C4 = 0 => C9 = 1, C4 = 0. Video Offset not updated.
 */
BOOST_AUTO_TEST_CASE(crtc0_additional_line_management) {

    CRTC crtc;
    setDefaults(crtc);
    crtc.type = 0;

    crtc.c9_rCounter = 3;
    crtc.r9_rMaxAddress = crtc.regs[9] = 3;

    crtc.c4_vCounter = 10;
    crtc.r4_vTotal = crtc.regs[4] = 10;

    crtc.r5_vAdjust = crtc.regs[5] = 16;
    crtc.r1_hDisplayed = crtc.regs[1] = 40;

    crtc.regs[12] = crtc.regs[13] = 0;
    crtc.lineAddress = 0;
    
    crtc.r0_hTotal = crtc.regs[0] = 63;
    crtc.c0_hCounter = crtc.regs[0] - 1;

    // Make C0=0 and start. Execute lines 0 to 3.
    // Video pointer is incremented at the end. (C9 == R9)
    uint_fast8_t line = 0;
    do {
        advanceToCharacter(crtc, 2);
        BOOST_CHECK_EQUAL(crtc.c9_rCounter, line++);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 11);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
        BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
        BOOST_CHECK_EQUAL(crtc.lineAddress, 0);
    } while (crtc.c9_rCounter < 3);

    // Execute line 4, then do OUT R9,10.
    advanceToCharacter(crtc, 2);
    BOOST_CHECK_EQUAL(crtc.c9_rCounter, line++);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, 11);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
    BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
    BOOST_CHECK_EQUAL(crtc.lineAddress, 40);
    crtc.regs[9] = 10;

    // Execute lines 5 to 10.
    // Video pointer is incremented at the end. (C9 == R9)
    do {
        advanceToCharacter(crtc, 2);
        BOOST_CHECK_EQUAL(crtc.c9_rCounter, line++);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 11);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
        BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
        BOOST_CHECK_EQUAL(crtc.lineAddress, 40);
    } while (crtc.c9_rCounter < 10);
    
    // Execute lines 11 to 16.
    do {
        advanceToCharacter(crtc, 2);
        BOOST_CHECK_EQUAL(crtc.c9_rCounter, line++);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 11);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
        BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
        BOOST_CHECK_EQUAL(crtc.lineAddress, 80);
    } while (crtc.c9_rCounter < 15);

    advanceToCharacter(crtc, 0);
    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
    BOOST_CHECK_EQUAL(crtc.processVAdjust, false);
    BOOST_CHECK_EQUAL(crtc.lineAddress, 0);
}

/**
 * CRTC type 1. Additional line management. Page 77.
 * Preconditions: R4=10, R5=16, R9=3, R1=40, R0=64.
 */
BOOST_AUTO_TEST_CASE(crtc1_additional_line_management) {

    CRTC crtc;
    setDefaults(crtc);
    crtc.type = 1;

    crtc.c9_rCounter = 3;
    crtc.r9_rMaxAddress = crtc.regs[9] = 3;

    crtc.c4_vCounter = 10;
    crtc.r4_vTotal = crtc.regs[4] = 10;

    crtc.r5_vAdjust = crtc.regs[5] = 16;
    crtc.r1_hDisplayed = crtc.regs[1] = 40;

    crtc.regs[12] = crtc.regs[13] = 0;
    crtc.lineAddress = 0;
    
    crtc.r0_hTotal = crtc.regs[0] = 63;
    crtc.c0_hCounter = crtc.regs[0] - 1;

    // Make C0=0 and start. Execute lines 0 to 3.
    // Video pointer is incremented at the end. (C9 == R9)
    uint_fast8_t line = 0;
    uint_fast8_t raster = 0;
    do {
        advanceToCharacter(crtc, 0);
        BOOST_CHECK_EQUAL(crtc.c5_aCounter, line);
        BOOST_CHECK_EQUAL(crtc.c9_rCounter, raster);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 11);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
        BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
        BOOST_CHECK_EQUAL(crtc.lineAddress, 0);
        ++raster;
        ++line;
    } while (raster < 4);

    // Execute line 4, then do OUT R9,10.
    raster = 0;
    advanceToCharacter(crtc, 0);
    BOOST_CHECK_EQUAL(crtc.c5_aCounter, line);
    BOOST_CHECK_EQUAL(crtc.c9_rCounter, raster);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, 12);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
    BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
    BOOST_CHECK_EQUAL(crtc.lineAddress, 40);
    ++raster;
    ++line;
    crtc.regs[9] = 10;

    // Execute lines 5 to 15.
    // Video pointer is incremented at the end. (C9 == R9)
    do {
        advanceToCharacter(crtc, 0);
        BOOST_CHECK_EQUAL(crtc.c5_aCounter, line);
        BOOST_CHECK_EQUAL(crtc.c9_rCounter, raster);
        BOOST_CHECK_EQUAL(crtc.c4_vCounter, 12);
        BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
        BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
        BOOST_CHECK_EQUAL(crtc.lineAddress, 40);
        ++raster;
        ++line;
    } while (raster < 11);
    
    advanceToCharacter(crtc, 0);
    BOOST_CHECK_EQUAL(crtc.c5_aCounter, 15);
    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, 13);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, false);
    BOOST_CHECK_EQUAL(crtc.processVAdjust, true);
    BOOST_CHECK_EQUAL(crtc.lineAddress, 80);

    advanceToCharacter(crtc, 0);
    BOOST_CHECK_EQUAL(crtc.c5_aCounter, 15);
    BOOST_CHECK_EQUAL(crtc.c9_rCounter, 0);
    BOOST_CHECK_EQUAL(crtc.c4_vCounter, 0);
    BOOST_CHECK_EQUAL(crtc.videoOffsetUpdated, true);
    BOOST_CHECK_EQUAL(crtc.processVAdjust, false);
    BOOST_CHECK_EQUAL(crtc.lineAddress, 0);
}
// EOF
// vim: et:sw=4:ts=4
