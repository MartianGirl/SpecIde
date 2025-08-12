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
        /** Selected register address. */
        uint_fast8_t a = 0;
        /** PSG register bank. */
        uint_fast8_t r[16] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        /** PSG register valid bits mask. */
        uint_fast8_t m[16] {0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0x1F, 0xFF,
            0x1F, 0x1F, 0x1F, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF};

        /** Output DAC volume levels. */
        int out[32] {0x000, 0x000, 0x013, 0x013, 0x049, 0x049, 0x0A4, 0x0A4,
            0x124, 0x124, 0x1C7, 0x1C7, 0x290, 0x290, 0x37C, 0x37C,
            0x48D, 0x48D, 0x5C3, 0x5C3, 0x71C, 0x71C, 0x89B, 0x89B,
            0xA3D, 0xA3D, 0xC04, 0xC04, 0xDF0, 0xDF0, 0xFFF, 0xFFF};

        /** Channel A tone counter. */
        uint_fast16_t counterA = 0;
        /** Channel B tone counter. */
        uint_fast16_t counterB = 0;
        /** Channel C tone counter. */
        uint_fast16_t counterC = 0;
        /** Noise generator period counter. */
        uint_fast16_t counterN = 0;
        /** Envelope counter. */
        uint_fast16_t counterE = 0;

        /** Channel A wave period register. */
        uint_fast16_t periodA = 0;
        /** Channel B wave period register. */
        uint_fast16_t periodB = 0;
        /** Channel C wave period register. */
        uint_fast16_t periodC = 0;
        /** Noise generator period register. */
        uint_fast16_t periodN = 0;
        /** Envelope period register. */
        uint_fast16_t periodE = 0;

        /** Channel A tone waveform. */
        int waveA = 1;
        /** Channel B tone waveform. */
        int waveB = 1;
        /** Channel C tone waveform. */
        int waveC = 1;

        /** Channel A volume level. */
        int volumeA = 0;
        /** Channel B volume level. */
        int volumeB = 0;
        /** Channel C volume level. */
        int volumeC = 0;

        /** Envelope slope. (Ascending = 1, descending = -1) */
        int envSlope = 1;
        /** Envelope volume level. */
        int envLevel = 0;
        /** Envelope step. The envelope level is derived from this. */
        int envStep = 0;
        /** Hold the value of the envelope after one cycle. */
        bool envHold = false;
        /** Apply envelope volume to channel A. */
        bool envA = false;
        /** Apply envelope volume to channel B. */
        bool envB = false;
        /** Apply envelope volume to channel C. */
        bool envC = false;

        /** I/O port A. */
        uint_fast8_t ioA = 0xFF;
        /** I/O port B. */
        uint_fast8_t ioB = 0xFF;

        /** Clock counter. */
        uint_fast32_t count = 0;

        /** Filter array for channel A. */
        Filter filterA;
        /** Filter array for channel B. */
        Filter filterB;
        /** Filter array for channel C. */
        Filter filterC;

        /** Channel A sample. */
        int channelA = 0;
        /** Channel B sample. */
        int channelB = 0;
        /** Channel C sample. */
        int channelC = 0;
        /** Noise current value. */
        int noise = 0;
        /** Noise current seed. */
        int seed = 0xFFFF;

        /** Generate sound. */
        bool playSound = true;
        /** Behave as a AY-3-8912 (oppossed to a YM-2194) */
        bool psgIsAY = true;

        /** Output this PSG to the left channel. Only for Next mode. */
        bool lchan = false;
        /** Output this PSG to the right channel. Only for Next mode. */
        bool rchan = false;

        void clock() {

            // A period means a complete wave cycle (high/low)
            // Thus, the clock is not scaled further.
            if (!(++count & 0x07)) {
                // Count up, so if the period changes to a lower value than
                // current we end the pulse.
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
                            if (r[0xD] & 0x08) {
                                // Hold & Alternate
                                if (r[0xD] & 0x01) {
                                    envHold = true;
                                }

                                // If Alternate != Hold, change slope. :)
                                if (((r[0xD] & 0x02) >> 1) != (r[0xD] & 0x01)) {
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

            int signalA = 1;
            int signalB = 1;
            int signalC = 1;

            if (playSound) {
                signalA = (r[0x7] & 0x01) ? 1 : waveA;
                signalB = (r[0x7] & 0x02) ? 1 : waveB;
                signalC = (r[0x7] & 0x04) ? 1 : waveC;
                if (!(r[0x7] & 0x08)) signalA &= noise;
                if (!(r[0x7] & 0x10)) signalB &= noise;
                if (!(r[0x7] & 0x20)) signalC &= noise;
                signalA *= out[envA ? envLevel : volumeA];
                signalB *= out[envB ? envLevel : volumeB];
                signalC *= out[envC ? envLevel : volumeC];
            }

            filterA.add(signalA);
            filterB.add(signalB);
            filterC.add(signalC);
        }

        void sample() {

            channelA = filterA.get();
            channelB = filterB.get();
            channelC = filterC.get();
        }

        uint_fast8_t read() {

            return (!(a & 0xF0)) ? r[a] : 0xFF; }

        void write(uint_fast8_t byte) {

            if (!(a & 0xF0)) {
                // Write registers (on AY, take only used bits)
                r[a] = byte & (psgIsAY ? m[a] : 0xFF);

                switch (a) {
                    case 0x0:
                    case 0x1:
                        // Update tone period for channel A.
                        periodA = (r[0x1] & 0x0F) * 0x100 + r[0x0];
                        break;

                    case 0x2:
                    case 0x3:
                        // Update tone period for channel B.
                        periodB = (r[0x3] & 0x0F) * 0x100 + r[0x2];
                        break;

                    case 0x4:
                    case 0x5:
                        // Update tone period for channel C.
                        periodC = (r[0x5] & 0x0F) * 0x100 + r[0x4];
                        break;

                    case 0x6:
                        // Update noise period.
                        periodN = r[0x6] & 0x1F;
                        break;

                    case 0x8:
                        // Update volume for channel A.
                        volumeA = 2 * (r[0x8] & 0x0F) + 1;
                        envA = ((r[0x8] & 0x10) == 0x10);
                        break;

                    case 0x9:
                        // Update volume for channel B.
                        volumeB = 2 * (r[0x9] & 0x0F) + 1;
                        envB = ((r[0x9] & 0x10) == 0x10);
                        break;

                    case 0xA:
                        // Update volume for channel C.
                        volumeC = 2 * (r[0xA] & 0x0F) + 1;
                        envC = ((r[0xA] & 0x10) == 0x10);
                        break;

                    case 0xB:
                    case 0xC:
                        // Update period for Envelope generator.
                        periodE = r[0xC] * 0x100 + r[0xB];
                        break;

                    case 0xD:
                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        if (r[0xD] != 0xFF) {
                            envSlope = ((r[0xD] & 0x04) == 0x00) ? -1 : 1;
                            envLevel = ((r[0xD] & 0x04) == 0x00) ? 0x1F : 0x00;
                            restartEnvelope();
                        }
                        break;

                    case 0xE:
                        // I/O port A (as output).
                        if ((r[0x7] & 0x40) == 0x40) {
                            ioA = r[0xE];
                        }
                        break;

                    case 0xF:
                        // I/O port B (as output).
                        if ((r[0x7] & 0x80) == 0x80) {
                            ioB = r[0xF];
                        }
                        break;

                    default:
                        break;
                }
            }
        }

        void addr(uint_fast8_t byte) {

            a = byte;
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
            if ((r[0x7] & 0x40) == 0x00) {
                r[0xE] = ioA = byte;
            }
        }

        void setPortB(uint_fast8_t byte) {

            // Copy value in port B to R15 if it is set as Input
            if ((r[0x7] & 0x80) == 0x00) {
                r[0xF] = ioB = byte;
            }
        }
};
// vim: et:sw=4:ts=4
