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

constexpr int SOUND_VOLUME = 0x0FFF;
constexpr int SAVE_VOLUME = 0x03FF;
constexpr int LOAD_VOLUME = 0x01FF;

class Buzzer
{
    public:
        int filter[FILTER_BZZ_SIZE];

        uint_fast8_t *source;
        uint_fast8_t *tapeIn;

        int signal;

        bool playSound;
        bool tapeSound;

        Buzzer() :
            filter{0},
            playSound(true), tapeSound(true) {}

        void init(uint_fast8_t* src, uint_fast8_t* ear)
        {
            source = src;
            tapeIn = ear;
        }

        void update()
        {
            static uint_fast32_t index = 0;

            // Smooth the signal directly from the ULA.
            if (playSound)
            {
                filter[index] = (*source & 0x10) ? SOUND_VOLUME : 0;
                if (tapeSound)
                {
                    filter[index] += 
                        + (((*source & 0x18) == 0x18) ? SAVE_VOLUME : 0)
                        + ((*tapeIn & 0x40) ? LOAD_VOLUME : 0);
                }
            }
            else
            {
                filter[index] = 0x00;
            }
            index = (index + 1) % FILTER_BZZ_SIZE;
        }

        void sample()
        {
                signal = 0;
                for (uint_fast32_t i = 0; i < FILTER_BZZ_SIZE; ++i)
                    signal += filter[i];
                signal /= FILTER_BZZ_SIZE;
        }
};

// vim: et:sw=4:ts=4
