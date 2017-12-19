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

#include "SoundChannel.h"

constexpr size_t FILTER_SIZE = 352;

constexpr size_t ULA_CLOCK_48 = 7000000;
constexpr size_t ULA_CLOCK_128 = 7093800;

constexpr int SOUND_VOLUME = 0x10;
constexpr int SAVE_VOLUME = 0x0C;
constexpr int LOAD_VOLUME = 0x04;

class Buzzer
{
    public:
        SoundChannel channel;

        int filter[FILTER_SIZE];

        uint_fast8_t *source;
        uint_fast8_t *tapeIn;
        int *psg;

        size_t skip;

        bool playSound;
        bool tapeSound;
        bool hasPsg;

        Buzzer() :
            filter{0},
            playSound(true), tapeSound(true), hasPsg(false) {}

        bool open(uint_fast8_t* src, uint_fast8_t* ear, size_t sampleRate)
        {
            source = src;
            tapeIn = ear;
            skip = ULA_CLOCK_48 / sampleRate;
            return channel.open(sampleRate);
        }

        void setPsg(int* sound)
        {
            psg = sound;
        }

        void set128K(bool select128)
        {
            skip = (select128 ? ULA_CLOCK_128 : ULA_CLOCK_48) / channel.sampleRate;
            hasPsg = select128;
        }

        void sample()
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
                int s = 0;
                for (size_t i = 0; i < FILTER_SIZE; ++i)
                    s += filter[i];
                if (hasPsg)
                    s += *psg;

                channel.push(static_cast<int_fast16_t>(s));
            }
        }

        void play() { channel.play(); }
        void stop() { channel.stop(); }
};

// vim: et:sw=4:ts=4
