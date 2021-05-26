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

/** PSG
 *
 * AY-3-8912 implementation.
 *
 */

#include <cassert>
#include <cstdint>
#include <iostream>

#include "SoundDefs.h"

using namespace std;

class PSG
{
    public:
        uint_fast8_t latch_a;
        uint_fast8_t latch_di;
        uint_fast8_t latch_do;

        uint_fast8_t a;
        uint_fast8_t r[16];
        uint_fast8_t m[16];

        bool wr;

        int filterA[FILTER_PSG_SIZE];
        int filterB[FILTER_PSG_SIZE];
        int filterC[FILTER_PSG_SIZE];
        int channelA = 0;
        int channelB = 0;
        int channelC = 0;
        int noise = 0;
        int seed = 0xFFFF;
        int volumeA;
        int volumeB;
        int volumeC;
        int waveA;
        int waveB;
        int waveC;

        int out[32];

        int envSlope, envLevel;
        int envStep;
        bool envHold;
        bool envA;
        bool envB;
        bool envC;

        uint_fast16_t counterA, counterB, counterC, counterN, counterE;
        uint_fast16_t periodA, periodB, periodC, periodN, periodE;

        uint_fast8_t ioA = 0xFF;
        uint_fast8_t ioB = 0xFF;

        size_t count = 0;
        size_t index = 0;
        size_t wait = 0;

        bool playSound;
        bool lchan = false;
        bool rchan = false;

        bool psgIsAY = true;

        PSG() :
            r{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            m{0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0x1F, 0xFF,
                0x1F, 0x1F, 0x1F, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF},
            wr(false),
            volumeA(0), volumeB(0), volumeC(0),
            waveA(1), waveB(1), waveC(1),
            out{0x000, 0x000, 0x013, 0x013, 0x049, 0x049, 0x0A4, 0x0A4,
                0x124, 0x124, 0x1C7, 0x1C7, 0x290, 0x290, 0x37C, 0x37C,
                0x48D, 0x48D, 0x5C3, 0x5C3, 0x71C, 0x71C, 0x89B, 0x89B,
                0xA3D, 0xA3D, 0xC04, 0xC04, 0xDF0, 0xDF0, 0xFFF, 0xFFF},
            envSlope(1), envLevel(0), envStep(0), envHold(false),
            envA(false), envB(false), envC(false),
            counterA(0), counterB(0), counterC(0), counterN(0), counterE(0),
            periodA(0), periodB(0), periodC(0), periodN(0), periodE(0),
            playSound(true) {}

        void clock() {

            int signalA = 1;
            int signalB = 1;
            int signalC = 1;

            ++count;

            a = latch_a;
            latch_do = r[a];

            if (wr) {
                // Write registers (on AY, take only used bits)
                r[a] = latch_di & (psgIsAY ? m[a] : 0xFF);

                wr = false;

                switch (a) {
                    case 000:
                    case 001:
                        // Update tone period for channel A.
                        periodA = (r[1] & 0x0F) * 0x100 + r[0];
                        break;

                    case 002:
                    case 003:
                        // Update tone period for channel B.
                        periodB = (r[3] & 0x0F) * 0x100 + r[2];
                        break;

                    case 004:
                    case 005:
                        // Update tone period for channel C.
                        periodC = (r[5] & 0x0F) * 0x100 + r[4];
                        break;

                    case 006:
                        // Update noise period.
                        periodN = r[6] & 0x1F;
                        break;

                    case 010:
                        // Update volume for channel A.
                        volumeA = 2 * (r[8] & 0x0F) + 1;
                        envA = ((r[8] & 0x10) == 0x10);
                        break;

                    case 011:
                        // Update volume for channel B.
                        volumeB = 2 * (r[9] & 0x0F) + 1;
                        envB = ((r[9] & 0x10) == 0x10);
                        break;

                    case 012:
                        // Update volume for channel C.
                        volumeC = 2 * (r[10] & 0x0F) + 1;
                        envC = ((r[10] & 0x10) == 0x10);
                        break;

                    case 013:
                    case 014:
                        // Update period for Envelope generator.
                        periodE = r[12] * 0x100 + r[11];
                        break;

                    case 015:
                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        if (r[13] != 0xFF) {
                            envSlope = ((r[13] & 0x04) == 0x00) ? -1 : 1;
                            envLevel = ((r[13] & 0x04) == 0x00) ? 0x1F : 0x00;
                            restartEnvelope();
                        }
                        break;

                    case 016:
                        // I/O port A (as output).
                        if ((r[7] & 0x40) == 0x40) {
                            ioA = r[14];
                        }
                        break;

                    case 017:
                        // I/O port B (as output).
                        if ((r[7] & 0x80) == 0x80) {
                            ioB = r[15];
                        }
                        break;

                    default:
                        break;
                }
            }

            // Because period means a complete wave cycle (high/low)
            if (!(count & 0x07)) {
                if (++counterA >= periodA) {
                    waveA = 1 - waveA;
                    counterA = 0;
                }

                if (++counterB >= periodB) {
                    waveB = 1 - waveB;
                    counterB = 0;
                }

                if (++counterC >= periodC) {
                    waveC = 1 - waveC;
                    counterC = 0;
                }

                if (++counterN >= 2 * periodN) {
                    noise = generateNoise();
                    counterN = 0;
                }

                if (++counterE >= periodE) {
                    counterE = 0;

                    if (!envHold) {
                        if (++envStep >= 0x20) { // We've finished a cycle.
                            envStep = 0x00;

                            // Continue = 1: Cycle pattern controlled by Hold.
                            if (r[13] & 0x08) {
                                // Hold & Alternate
                                if (r[13] & 0x01) {
                                    envHold = true;
                                }

                                // If Alternate != Hold, change slope. :)
                                if (((r[13] & 0x02) >> 1) != (r[13] & 0x01)) {
                                    envSlope = -envSlope;
                                }
                            } else {
                                // Continue = 0: Just one cycle, return to 0000.
                                //               Hold.
                                envHold = true;
                                envSlope = 1;
                            }
                        }
                        envLevel = (envSlope > 0) ? envStep : (0x1F - envStep);
                    }
                }
            }

            if (playSound) {
                signalA = (r[7] & 0x01) ? 1 : waveA;
                signalB = (r[7] & 0x02) ? 1 : waveB;
                signalC = (r[7] & 0x04) ? 1 : waveC;
                if (!(r[7] & 0x08)) signalA += noise;
                if (!(r[7] & 0x10)) signalB += noise;
                if (!(r[7] & 0x20)) signalC += noise;
                signalA *= out[envA ? envLevel : volumeA];
                signalB *= out[envB ? envLevel : volumeB];
                signalC *= out[envC ? envLevel : volumeC];
            }

            filterA[index] = signalA;
            filterB[index] = signalB;
            filterC[index] = signalC;
            index = (index + 1) % FILTER_PSG_SIZE;
        }

        void sample() {

            channelA = channelB = channelC = 0;

            for (uint_fast16_t i = 0; i < FILTER_PSG_SIZE; ++i) {
                channelA += filterA[i];
                channelB += filterB[i];
                channelC += filterC[i];
            }

            channelA /= FILTER_PSG_SIZE;
            channelB /= FILTER_PSG_SIZE;
            channelC /= FILTER_PSG_SIZE;
        }


        uint_fast8_t read() { return latch_do; }

        void write(uint_fast8_t byte) {

            latch_di = byte;
            wr = true;
        }

        void addr(uint_fast8_t byte) {

            if ((byte & 0x0F) == byte) {
                latch_a = byte;
            }
        }

        void setVolumeLevels(bool ay) {

            psgIsAY = ay;

            if (ay) {  // AY-3-8912 (Values by Hacker KAY)
                int arr[32] = {
                    0x000, 0x000, 0x034, 0x034, 0x04B, 0x04B, 0x06E, 0x06E,
                    0x0A3, 0x0A3, 0x0F2, 0x0F2, 0x151, 0x151, 0x227, 0x227,
                    0x289, 0x289, 0x413, 0x413, 0x5B2, 0x5B2, 0x726, 0x726,
                    0x906, 0x906, 0xB54, 0xB54, 0xD78, 0xD78, 0xFFF, 0xFFF};

                for (uint_fast8_t i = 0; i < 32; ++i) {
                    out[i] = arr[i];
                }
            } else {      // YM-2149 (Values by Hacker KAY)
                int arr[32] = {
                    0x000, 0x000, 0x00F, 0x01C, 0x029, 0x033, 0x03F, 0x04D,
                    0x060, 0x077, 0x090, 0x0A4, 0x0C3, 0x0EC, 0x113, 0x13A,
                    0x174, 0x1BF, 0x20D, 0x259, 0x2C9, 0x357, 0x3E5, 0x476,
                    0x54F, 0x661, 0x773, 0x883, 0xA1D, 0xC0F, 0xE08, 0xFFF};

                for (uint_fast8_t i = 0; i < 32; ++i) {
                    out[i] = arr[i];
                }
            }
        }

        void reset() {

            for (uint_fast8_t i = 0; i < 16; ++i) {
                r[i] = 0;
            }

            periodA = periodB = periodC = 0;
            periodE = 0;
            periodN = 0;
            volumeA = volumeB = volumeC = 0;
            seed = 0xFFFF;
        }

        void restartEnvelope() {

            envStep = 0;
            envHold = false;
            counterE = 0;
        }

        int generateNoise() {

            // GenNoise (c) Hacker KAY & Sergey Bulba
            seed = (seed * 2 + 1) ^ (((seed >> 16) ^ (seed >> 13)) & 1);
            return ((seed >> 16) & 1);
        }

        void setPortA(uint_fast8_t byte) {

            // Copy value in port A to R14 if it is set as Input
            if ((r[7] & 0x40) == 0x00) {
                r[14] = ioA = byte;
            }

        }

        void setPortB(uint_fast8_t byte) {

            // Copy value in port B to R15 if it is set as Input
            if ((r[7] & 0x80) == 0x00) {
                r[15] = ioB = byte;
            }
        }
};
// vim: et:sw=4:ts=4
