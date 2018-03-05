#pragma once

/** PSG
 *
 * AY-3-8912 implementation.
 *
 */

#include <cassert>
#include <cstdint>
#include <random>

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
        int noise;
        int volumeA, volumeB, volumeC;
        int waveA, waveB, waveC;

        int out[16];

        int envIncrement, envStart, envLevel;
        int envSlope, envCycle;
        uint_fast8_t envStep;
        bool envA, envB, envC;

        uint_fast32_t counterA, counterB, counterC, counterN, counterE;
        uint_fast32_t periodA, periodB, periodC, periodN, periodE;

        bool playSound;

        random_device rd;
        mt19937 gen;
        uniform_int_distribution<> uniform;

        PSG() :
            r{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            wr(false),
            signalA(0), signalB(0), signalC(0),
            channelA(0), channelB(0), channelC(0),
            noise(0),
            volumeA(0), volumeB(0), volumeC(0),
            waveA(1), waveB(1), waveC(1),
            out{0x000, 0x012, 0x049, 0x0A4, 0x123, 0x1C7, 0x28F, 0x37C,
                0x48D, 0x5C2, 0x71C, 0x89A, 0xA3D, 0xC04, 0xDEF, 0xFFF},
            envIncrement(1), envStart(0), envLevel(0), envStep(0),
            envA(false), envB(false), envC(false),
            counterA(0), counterB(0), counterC(0), counterN(0), counterE(0),
            periodA(1), periodB(1), periodC(1), periodN(1), periodE(1),
            playSound(true),
            gen(rd()), uniform(0, 1) {} 

        void clock()
        {
            static uint_fast16_t index = 0;
            static uint_fast32_t count = 0;

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
                        if ((r[8] & 0x10) == 0x00)
                            volumeA = r[8] & 0x0F;
                        envA = ((r[8] & 0x10) == 0x10);
                        break;

                    case 011:
                        // Update volume for channel B.
                        if ((r[9] & 0x10) == 0x00)
                            volumeB = r[9] & 0x0F;
                        envB = ((r[9] & 0x10) == 0x10);
                        break;

                    case 012:
                        // Update volume for channel C.
                        if ((r[10] & 0x10) == 0x00)
                            volumeC = r[10] & 0x0F;
                        envC = ((r[10] & 0x10) == 0x10);
                        break;

                    case 013:
                    case 014:
                        // Update period for Envelope generator.
                        periodE = ((r[12] << 8) + r[11]);
                        break;

                    case 015:
                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        envStart = ((r[13] & 0x04) == 0x00) ? 0x0F : 0x00;
                        envIncrement = ((r[13] & 0x04) == 0x00) ? -1 : 1;
                        restartEnvelope();
                        break;

                    default:
                        break;
                }
            }

            // Because period means a complete wave cycle (high/low)
            if ((count & 0x07) == 0x00)
            {
                if (counterA-- == 0)
                {
                    waveA = 1 - waveA;
                    counterA = periodA;
                }

                if (counterB-- == 0)
                {
                    waveB = 1 - waveB;
                    counterB = periodB;
                }

                if (counterC-- == 0)
                {
                    waveC = 1 - waveC;
                    counterC = periodC;
                }

                if ((count & 0x0F) == 0x00)
                {
                    if (counterN-- == 0)
                    {
                        noise = uniform(gen);
                        counterN = periodN;
                    }

                    if (counterE-- == 0)
                    {
                        counterE = periodE;

                        if (envStep == 0x0F)    // We've finished a cycle.
                        {
                            // Continue = 1: Cycle pattern controlled by Hold.
                            if ((r[13] & 0x08) == 0x08)
                            {
                                // Hold = 1: Keep last level.
                                // Hold = 0: Repeat the cycle.
                                if ((r[13] & 0x01) == 0x01)
                                {
                                    // Alternate and hold: Reset level to
                                    // the starting value, hold.
                                    if ((r[13] & 0x02) == 0x02)
                                    {
                                        envLevel = envStart;
                                    }
                                }
                                else
                                {
                                    // Alternate = 1: Invert direction.
                                    if ((r[13] & 0x02) == 0x02)
                                    {
                                        envCycle = 0x0F - envCycle;
                                        envSlope = -envSlope;
                                    }

                                    envStep = 0x00;
                                    envLevel = envCycle;
                                }
                            }
                            else
                            {
                                // Continue = 0: Just one cycle, return to 0000.
                                //               Hold.
                                envLevel = 0x00;
                            }
                        }
                        else    // We are in the middle of a cycle.
                        {
                            ++envStep;
                            envLevel += envSlope;
                        }
                    }

                    if (envA) volumeA = envLevel;
                    if (envB) volumeB = envLevel;
                    if (envC) volumeC = envLevel;
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
                signalA *= out[volumeA];
                signalB *= out[volumeB];
                signalC *= out[volumeC];
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
                latch_a = byte;
            }
        }

        void setVolumeLevels(bool sqr)
        {
            if (sqr)    // Square root (quieter)
            {
                int arr[16] = {
                    0x000, 0x012, 0x049, 0x0A4, 0x123, 0x1C7, 0x28F, 0x37C,
                    0x48D, 0x5C2, 0x71C, 0x89A, 0xA3D, 0xC04, 0xDEF, 0xFFF};

                for (uint_fast8_t i = 0; i < 16; ++i)
                    out[i] = arr[i];
            }
            else        // Linear (louder)
            {
                for (uint_fast8_t i = 0; i < 16; ++i)
                    out[i] = static_cast<int>(0x222 * i);
            }
        }

        void reset()
        {
            for (uint_fast8_t i = 0; i < 16; ++i)
                r[i] = 0;

            periodA = periodB = periodC = 1;
            periodE = 1;
            periodN = 1;
            volumeA = volumeB = volumeC = 0;
        }

        void restartEnvelope()
        {
            counterE = 0;
            envStep = 0;
            envCycle = envStart;
            envSlope = envIncrement;
            envLevel = envStart;
        }
};

// vim: et:sw=4:ts=4
