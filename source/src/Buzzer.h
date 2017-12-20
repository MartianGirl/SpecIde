#pragma once

/** Buzzer
 *
 * Plays sound from an input BuzzerStream.
 *
 * At the moment it generates 16-bit audio with a 44100Hz sampling rate.
 *
 */

#include <cstdint>
#include <cstddef>
#include <queue>
#include <vector>

#include "SoundDefs.h"

constexpr int SOUND_VOLUME = 0x2E;
constexpr int SAVE_VOLUME = 0x17;
constexpr int LOAD_VOLUME = 0x0B;

class Buzzer
{
    public:
        int filter[FILTER_SIZE];

        uint_fast8_t *source;
        uint_fast8_t *tapeIn;

        size_t skip;
        size_t rate;

        int signal;

        bool playSound;
        bool tapeSound;

        Buzzer() :
            filter{0},
            playSound(true), tapeSound(true) {}

        void init(uint_fast8_t* src, uint_fast8_t* ear, size_t sampleRate)
        {
            source = src;
            tapeIn = ear;
            rate = sampleRate;
            skip = ULA_CLOCK_48 / rate;
        }

        void set128K(bool select128)
        {
            skip = (select128 ? ULA_CLOCK_128 : ULA_CLOCK_48) / rate;
        }

        bool sample()
        {
            static size_t count = 0;
            static size_t index = 0;

            // Smooth the signal directly from the ULA.
            if (playSound)
            {
                // I'm going to try a trick for keeping the volume high,
                // while not causing samples to overflow. The buzzer and
                // PSG channel A are going to be positive, while channels
                // B and C are going to be negative. Since I'm just inverting
                // two waveforms, the generated sound should be the same
                // to the ear, but since all signals are positive, the sum
                // will not overflow.
                filter[index] = (*source & 0x10) ? SOUND_VOLUME : 0;
                if (tapeSound)
                {
                    filter[index] += 
                        + ((*source & 0x18) ? SAVE_VOLUME : 0)
                        + ((*tapeIn & 0x40) ? LOAD_VOLUME : 0);
                }
            }
            else
            {
                filter[index] = 0x00;
            }
            index = (index + 1) % FILTER_SIZE;

            if (++count == skip)
            {
                count = 0;
                signal = 0;
                for (size_t i = 0; i < FILTER_SIZE; ++i)
                    signal += filter[i];

                return true;
            }

            return false;
        }
};

// vim: et:sw=4:ts=4
