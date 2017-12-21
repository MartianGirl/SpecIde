#pragma once

/** PSG
 *
 * AY-3-8912 implementation.
 *
 */

#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
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

        int channelA, channelB, channelC;
        int noise;
        int volumeA, volumeB, volumeC;
        int waveA, waveB, waveC;

        int signal;

        int out[16];

        int envIncrement, envStart, envLevel;
        int envSlope, envCycle;
        size_t envStep;
        bool envHold;

        size_t counterA, counterB, counterC, counterN, counterE;
        size_t periodA, periodB, periodC, periodN, periodE;

        size_t masterCounter;

        bool playSound;

        random_device rd;
        mt19937 gen;
        uniform_int_distribution<> uniform;

        PSG() :
            r{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            wr(false),
            channelA(0), channelB(0), channelC(0),
            noise(0),
            volumeA(0), volumeB(0), volumeC(0),
            waveA(1), waveB(1), waveC(1),
            signal(0),
            out{0x000, 0x012, 0x049, 0x0A4, 0x123, 0x1C7, 0x28F, 0x37C,
                0x48D, 0x5C2, 0x71C, 0x89A, 0xA3D, 0xC04, 0xDEF, 0xFFF},
            envIncrement(1), envStart(0), envLevel(0), envStep(0),
            counterA(0), counterB(0), counterC(0), counterN(0), counterE(0),
            periodA(1), periodB(1), periodC(1), periodN(1), periodE(1),
            masterCounter(0),
            playSound(true),
            gen(rd()), uniform(0, 1) {} 
        void clock()
        {
            ++masterCounter;

            if ((masterCounter & 0x03) == 0x00)
            {
                a = latch_a;
                latch_do = r[a];

                if (wr)
                {
                    // Write registers.
                    r[a] = latch_di;
                    wr = false;

                    // Update tone period for channel A.
                    if (a == 0 || a == 1)
                    {
                        periodA = (((r[1] & 0x0F) << 8) + r[0]);
                        if (periodA == 0) periodA = 1;
                        counterA = 0;
                        restartEnvelope();
                    }

                    // Update tone period for channel B.
                    if (a == 2 || a == 3)
                    {
                        periodB = (((r[3] & 0x0F) << 8) + r[2]);
                        if (periodB == 0) periodB = 1;
                        counterB = 0;
                        restartEnvelope();
                    }

                    // Update tone period for channel C.
                    if (a == 4 || a == 5)
                    {
                        periodC = (((r[5] & 0x0F) << 8) + r[4]);
                        if (periodC == 0) periodC = 1;
                        counterC = 0;
                        restartEnvelope();
                    }

                    // Update noise period.
                    if (a == 6)
                    {
                        periodN = (r[6] & 0x1F);
                        if (periodN == 0) periodN = 1;
                        counterN = 0;
                        restartEnvelope();
                    }

                    // Update period for Envelope generator.
                    if (a >= 11 || a <= 13)
                    {
                        periodE = ((r[12] << 8) + r[11]);
                        if (periodE == 0) periodE = 1;

                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        envStart = ((r[13] & 0x04) == 0x00) ? 0x0F : 0x00;
                        envIncrement = ((r[13] & 0x04) == 0x00) ? -1 : 1;
                        restartEnvelope();
                    }

                    // Update volume for channel A.
                    if (a == 8)
                    {
                        if ((r[8] & 0x10) == 0x00)
                        {
                            volumeA = r[8] & 0x0F;
                        }
                        else
                        {
                            volumeA = envStart;
                            restartEnvelope();
                        }
                    }

                    // Update volume for channel B.
                    if (a == 9)
                    {
                        if ((r[9] & 0x10) == 0x00)
                        {
                            volumeB = r[9] & 0x0F;
                        }
                        else
                        {
                            volumeB = envStart;
                            restartEnvelope();
                        }
                    }

                    // Update volume for channel C.
                    if (a == 10)
                    {
                        if ((r[10] & 0x10) == 0x00)
                        {
                            volumeC = r[10] & 0x0F;
                        }
                        else
                        {
                            volumeC = envStart;
                            restartEnvelope();
                        }
                    }
                }

                // Because period means a complete wave cycle (high/low)
                if ((masterCounter & 0x1F) == 0x00)
                {
                    if (++counterA == periodA)
                    {
                        waveA = 1 - waveA;
                        counterA = 0;
                    }

                    if (++counterB == periodB)
                    {
                        waveB = 1 - waveB;
                        counterB = 0;
                    }

                    if (++counterC == periodC)
                    {
                        waveC = 1 - waveC;
                        counterC = 0;
                    }
                }

                if ((masterCounter & 0x3F) == 0x00)
                {
                    if (++counterN == periodN)
                    {
                        noise = uniform(gen);
                        counterN = 0;
                    }

                    if (envHold == false && ++counterE == periodE)
                    {
                        counterE = 0;
                        if (envStep == 0x0F)    // We've finished a cycle.
                        {
                            // Continue = 1: Cycle pattern controlled by Hold.
                            if ((r[13] & 0x08) == 0x08)
                            {
                                // Hold = 1: Keep last level.
                                // Hold = 0: Repeat the cycle.
                                if ((r[13] & 0x01) == 0x01)
                                {
                                    envHold = true;

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
                                envHold = true;
                            }
                        }
                        else    // We are in the middle of a cycle.
                        {
                            ++envStep;
                            envLevel += envSlope;
                        }
                    }

                    if ((r[8] & 0x10) == 0x10)
                    {
                        volumeA = envLevel;
                    }

                    if ((r[9] & 0x10) == 0x10)
                    {
                        volumeB = envLevel;
                    }

                    if ((r[10] & 0x10) == 0x10)
                    {
                        volumeC = envLevel;
                    }
                }

            }
        }

        void sample()
        {
            // Update PSG channels.
            if (playSound)
            {
                channelA = (r[7] & 0x01) ? 1 : waveA;
                channelB = (r[7] & 0x02) ? 1 : waveB;
                channelC = (r[7] & 0x04) ? 1 : waveC;
                if ((r[7] & 0x08) == 0) channelA += noise;
                if ((r[7] & 0x10) == 0) channelB += noise;
                if ((r[7] & 0x20) == 0) channelC += noise;
                channelA *= out[volumeA];
                channelB *= out[volumeB];
                channelC *= out[volumeC];
                signal = channelA + channelB + channelC;
            }
            else
            {
                signal = 0;
            }
        }

        uint_fast8_t read() { return latch_do; }

        void write(uint_fast8_t byte)
        {
            static size_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                latch_di = byte;
                wr = true;
            }
        }

        void addr(uint_fast8_t byte)
        {
            static size_t wait = 0;

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

                for (size_t i = 0; i < 16; ++i)
                    out[i] = arr[i];
            }
            else        // Linear (louder)
            {
                for (size_t i = 0; i < 16; ++i)
                    out[i] = static_cast<int>(0x111 * i);
            }
        }

        void reset()
        {
            for (size_t i = 0; i < 16; ++i)
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
            envHold = false;
        }
};

// vim: et:sw=4:ts=4
