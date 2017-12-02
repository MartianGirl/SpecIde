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

#include <SFML/Audio.hpp>

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

        sf::Int16 channelA, channelB, channelC, channelN;
        sf::Int16 volumeA, volumeB, volumeC;
        sf::Int16 waveA, waveB, waveC;

        sf::Int16 envIncrement, envStart, envLevel;
        size_t envStep;

        size_t counterA, counterB, counterC, counterN, counterE;
        size_t periodA, periodB, periodC, periodN, periodE;

        size_t masterCounter;

        random_device rd;
        mt19937 gen;
        uniform_int_distribution<> uniform;

        PSG() :
            wr(false),
            channelA(0), channelB(0), channelC(0), channelN(0),
            volumeA(0x0F), volumeB(0x0F), volumeC(0x0F),
            waveA(0), waveB(0), waveC(0),
            counterA(0), counterB(0), counterC(0), counterN(0), counterE(0),
            periodA(0), periodB(0), periodC(0), periodN(0), periodE(0),
            masterCounter(0),
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

                    // When writing, update the operation mode:
                    // Update tone period for channel A.
                    if (a == 0 || a == 1)
                    {
                        periodA = (((r[1] & 0x0F) << 8) + r[0]) / 2;
                        counterA = 0;
                        counterE = 0;
                        envStep = 0;
                    }

                    // Update tone period for channel B.
                    if (a == 2 || a == 3)
                    {
                        periodB = (((r[3] & 0x0F) << 8) + r[2]) / 2;
                        counterB = 0;
                        counterE = 0;
                        envStep = 0;
                    }
                    
                    // Update tone period for channel C.
                    if (a == 4 || a == 5)
                    {
                        periodC = (((r[5] & 0x0F) << 8) + r[4]) / 2;
                        counterC = 0;
                        counterE = 0;
                        envStep = 0;
                    }

                    // Update noise period.
                    if (a == 6)
                    {
                        periodN = (r[6] & 0x1F);
                        counterN = 0;
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
                            counterE = 0;
                            envStep = 0;
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
                            counterE = 0;
                            envStep = 0;
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
                            counterE = 0;
                            envStep = 0;
                        }

                    }

                    // Update period for Envelope generator.
                    if (a == 11 || a == 12)
                    {
                        periodE = ((r[12] << 8) + r[11]);
                        counterE = 0;
                        envStep = 0;
                    }

                    // Update parameters for envelope generator.
                    if (a == 13)
                    {
                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        envStart = ((r[13] & 0x04) == 0x00) ? 0x0F : 0x00;
                        envIncrement = ((r[13] & 0x04) == 0x00) ? -1 : 1;
                        envLevel = envStart;
                        envStep = 0;
                        counterE = 0;

                    }
                }

                if ((masterCounter & 0x3F) == 0x00)
                {
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

                    if (periodA && (++counterA == periodA))
                    {
                        waveA = 1 - waveA;
                        counterA = 0;
                    }

                    if (periodB && (++counterB == periodB))
                    {
                        waveB = 1 - waveB;
                        counterB = 0;
                    }

                    if (periodC && (++counterC == periodC))
                    {
                        waveC = 1 - waveC;
                        counterC = 0;
                    }

                    if (periodN && (++counterN == periodN))
                    {
                        channelN = uniform(gen);
                        counterN = 0;
                    }

                    channelA = (r[7] & 0x01) ? 0 : waveA;
                    channelB = (r[7] & 0x02) ? 0 : waveB;
                    channelC = (r[7] & 0x04) ? 0 : waveC;
                    if ((r[7] & 0x08) == 0) channelA += channelN;
                    if ((r[7] & 0x10) == 0) channelB += channelN;
                    if ((r[7] & 0x20) == 0) channelC += channelN;
                    channelA *= volumeA;
                    channelB *= volumeB;
                    channelC *= volumeC;

                    if (periodE && (++counterE == periodE))
                    {
                        counterE = 0;
                        if (envStep == 0x0F)    // We've finished a cycle.
                        {
                            // Continue = 1: Cycle pattern controlled by Hold.
                            // Continue = 0: Just one cycle, return to 0000.
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
                                        envStart = 0x0F - envStart;
                                        envIncrement = -envIncrement;
                                    }

                                    envStep = 0x00;
                                    envLevel = envStart;
                                }

                            }
                            else
                            {
                                envLevel = 0x00;
                            }
                        }
                        else    // We are in the middle of a cycle.
                        {
                            ++envStep;
                            envLevel += envIncrement;
                        }
                    }
                }
            }
        }

        uint_fast8_t read() { return latch_do; }

        void write(uint_fast8_t byte)
        {
            static size_t wrWait = 0;

            if (++wrWait == 5)
            {
                wrWait = 0;
                latch_di = byte;
                wr = true;
            }
        }

        void addr(uint_fast8_t byte)
        {
            static size_t wrWait = 0;

            if (++wrWait == 5)
            {
                wrWait = 0;
                latch_a = byte;
            }
        };
};

// vim: et:sw=4:ts=4
