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
// #include <random>

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

        bool wr;

        int signalA, signalB, signalC;
        int filterA[FILTER_PSG_SIZE];
        int filterB[FILTER_PSG_SIZE];
        int filterC[FILTER_PSG_SIZE];
        int channelA, channelB, channelC;
        int noise, seed;
        int volumeA, volumeB, volumeC;
        int waveA, waveB, waveC;

        int out[32];

        int envSlope, envLevel;
        int envStep;
        bool envHold;
        bool envA, envB, envC;

        uint_fast32_t counterA, counterB, counterC, counterN, counterE;
        uint_fast32_t periodA, periodB, periodC, periodN, periodE;

        bool playSound;

        // random_device rd;
        // mt19937 gen;
        // uniform_int_distribution<> uniform;

        PSG() :
            r{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            wr(false),
            signalA(0), signalB(0), signalC(0),
            channelA(0), channelB(0), channelC(0),
            noise(0), seed(0xFFFF),
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
            // gen(rd()), uniform(0, 1) {} 

        void clock()
        {
            static uint_fast32_t count = 0;
            static uint_fast16_t index = 0;

            ++count;

            a = latch_a;
            latch_do = r[a];

            if (wr)
            {
                // Write registers.
                r[a] = latch_di;
                wr = false;

                switch (a)
                {
                    case 000:
                    case 001:
                        // Update tone period for channel A.
                        periodA = (((r[1] & 0x0F) << 8) + r[0]);
                        break;

                    case 002:
                    case 003:
                        // Update tone period for channel B.
                        periodB = (((r[3] & 0x0F) << 8) + r[2]);
                        break;

                    case 004:
                    case 005:
                        // Update tone period for channel C.
                        periodC = (((r[5] & 0x0F) << 8) + r[4]);
                        break;

                    case 006:
                        // Update noise period.
                        periodN = (r[6] & 0x1F);
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
                        periodE = ((r[12] * 0x100) + r[11]);
                        break;

                    case 015:
                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        if (r[13] != 0xFF)
                        {
                            envSlope = ((r[13] & 0x04) == 0x00) ? -1 : 1;
                            envLevel = ((r[13] & 0x04) == 0x00) ? 0x1F : 0x00;
                            restartEnvelope();
                        }
                        break;

                    default:
                        break;
                }
            }

            // Because period means a complete wave cycle (high/low)
            if ((count & 0x07) == 0x00)
            {
                if (++counterA >= periodA)
                {
                    waveA = 1 - waveA;
                    counterA = 0;
                }

                if (++counterB >= periodB)
                {
                    waveB = 1 - waveB;
                    counterB = 0;
                }

                if (++counterC >= periodC)
                {
                    waveC = 1 - waveC;
                    counterC = 0;
                }

                if (++counterN >= 2 * periodN)
                {
                    noise = generateNoise();
                    counterN = 0;
                }

                if (++counterE >= periodE)
                {
                    counterE = 0;

                    if (envHold == false)
                    {
                        if (++envStep >= 0x20)   // We've finished a cycle.
                        {
                            envStep = 0x00;

                            // Continue = 1: Cycle pattern controlled by Hold.
                            if ((r[13] & 0x08) == 0x08)
                            {
                                // Hold & Alternate
                                if ((r[13] & 0x01) == 0x01)
                                    envHold = true;

                                // If Alternate != Hold, change slope. :)
                                if (((r[13] & 0x02) >> 1) != (r[13] & 0x01))
                                    envSlope = -envSlope;
                            }
                            else
                            {
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

            signalA = signalB = signalC = 1;

            if (playSound)
            {
                if ((r[7] & 0x01) == 0) signalA = waveA;
                if ((r[7] & 0x02) == 0) signalB = waveB;
                if ((r[7] & 0x04) == 0) signalC = waveC;
                if ((r[7] & 0x08) == 0) signalA += noise;
                if ((r[7] & 0x10) == 0) signalB += noise;
                if ((r[7] & 0x20) == 0) signalC += noise;
                signalA *= out[envA ? envLevel : volumeA];
                signalB *= out[envB ? envLevel : volumeB];
                signalC *= out[envC ? envLevel : volumeC];
            }

            filterA[index] = signalA;
            filterB[index] = signalB;
            filterC[index] = signalC;
            index = (index + 1) % FILTER_PSG_SIZE;
        }

        void sample()
        {
            channelA = channelB = channelC = 0;
            for (uint_fast16_t i = 0; i < FILTER_PSG_SIZE; ++i)
            {
                channelA += filterA[i];
                channelB += filterB[i];
                channelC += filterC[i];
            }
            channelA /= FILTER_PSG_SIZE;
            channelB /= FILTER_PSG_SIZE;
            channelC /= FILTER_PSG_SIZE;
        }


        uint_fast8_t read() { return latch_do; }

        void write(uint_fast8_t byte)
        {
            static uint_fast8_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                latch_di = byte;
                wr = true;
            }
        }

        void addr(uint_fast8_t byte)
        {
            static uint_fast8_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                if ((byte & 0x0F) == byte)
                    latch_a = byte;
            }
        }

        void setVolumeLevels(bool ay)
        {
            if (ay)    // AY-3-8912
            {
                int arr[32] = {
                    0x000, 0x000, 0x013, 0x013, 0x049, 0x049, 0x0A4, 0x0A4,
                    0x124, 0x124, 0x1C7, 0x1C7, 0x290, 0x290, 0x37C, 0x37C,
                    0x48D, 0x48D, 0x5C3, 0x5C3, 0x71C, 0x71C, 0x89B, 0x89B,
                    0xA3D, 0xA3D, 0xC04, 0xC04, 0xDF0, 0xDF0, 0xFFF, 0xFFF};

                for (uint_fast8_t i = 0; i < 32; ++i)
                    out[i] = arr[i];
                cout << "PSG is AY-3-8912." << endl;
            }
            else        // YM-2149
            {
                int arr[32] = {
                    0x000, 0x005, 0x012, 0x027, 0x045, 0x06B, 0x09A, 0x0D1,
                    0x111, 0x15A, 0x1AB, 0x204, 0x266, 0x2D1, 0x344, 0x3BF,
                    0x443, 0x4D0, 0x565, 0x603, 0x6A9, 0x758, 0x80F, 0x8CF,
                    0x997, 0xA68, 0xB41, 0xC23, 0xD0D, 0xE00, 0xEFC, 0xFFF};

                for (uint_fast8_t i = 0; i < 32; ++i)
                    out[i] = arr[i];
                cout << "PSG is YM-2149." << endl;
            }
        }

        void reset()
        {
            for (uint_fast8_t i = 0; i < 16; ++i)
                r[i] = 0;

            periodA = periodB = periodC = 0;
            periodE = 0;
            periodN = 0;
            volumeA = volumeB = volumeC = 0;
            seed = 0xFFFF;
        }

        void restartEnvelope()
        {
            envStep = 0;
            envHold = false;
            counterE = 0;
        }

        int generateNoise()
        {
            // GenNoise (c) Hacker KAY & Sergey Bulba
            seed = (seed * 2 + 1) ^ (((seed >> 16) ^ (seed >> 13)) & 1);
            return ((seed >> 16) & 1);
        }
};

// vim: et:sw=4:ts=4
