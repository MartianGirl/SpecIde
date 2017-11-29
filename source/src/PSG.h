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

        sf::Int16 channelA, channelB, channelC;
        sf::Int16 volumeA, volumeB, volumeC;
        sf::Int16 waveA, waveB, waveC;

        size_t counterA, counterB, counterC;
        size_t periodA, periodB, periodC;

        size_t masterCounter;

        PSG() :
            wr(false),
            channelA(0), channelB(0), channelC(0),
            volumeA(0x10), volumeB(0x10), volumeC(0x10),
            waveA(0), waveB(0), waveC(0),
            counterA(0), counterB(0), counterC(0),
            periodA(0), periodB(0), periodC(0),
            masterCounter(0) {}


        void clock()
        {
            ++masterCounter;

            if ((masterCounter & 0x03) == 0x00)
            {
                a = latch_a;
                latch_do = r[a];

                if (wr)
                {
                    r[a] = latch_di;
                    wr = false;

                    if (a == 0 || a == 1)
                    {
                        periodA = ((r[1] & 0x0F) << 8) | r[0];
                        counterA = 0;
                    }

                    if (a == 2 || a == 3)
                    {
                        periodB = ((r[3] & 0x0F) << 8) | r[2];
                        counterB = 0;
                    }
                    
                    if (a == 4 || a == 5)
                    {
                        periodC = ((r[5] & 0x0F) << 8) | r[4];
                        counterC = 0;
                    }
                }

                if ((masterCounter & 0x3F) == 0x00)
                {
                    if (++counterA == periodA)
                    {
                        waveA = 1 - waveA;
                        channelA = waveA * volumeA; 
                        counterA = 0;
                    }

                    if (++counterB == periodB)
                    {
                        waveB = 1 - waveB;
                        channelB = waveB * volumeB;
                        counterB = 0;
                    }

                    if (++counterC == periodC)
                    {
                        waveC = 1 - waveC;
                        channelC = waveC * volumeC;
                        counterC = 0;
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
