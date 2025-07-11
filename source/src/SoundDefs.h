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

#define FILTER_SIZE 4

uint_fast32_t constexpr BASE_CLOCK_48 = 7000000;
uint_fast32_t constexpr BASE_CLOCK_128 = 7093800;
uint_fast32_t constexpr BASE_CLOCK_CPC = 16000000;
int constexpr FRAME_TIME_48 = 19968;
int constexpr FRAME_TIME_128 = 19992;
int constexpr FRAME_TIME_PENTAGON = 20480;
int constexpr FRAME_TIME_50HZ = 20000;
int constexpr FRAME_TIME_CPC = 19968;

uint32_t constexpr SAMPLE_RATE = 44100;

int constexpr ULA_BEEP_VOLUME = 0x19FF;
int constexpr ULA_SAVE_VOLUME = 0x03FF;
int constexpr ULA_LOAD_VOLUME = 0x01FF;

int constexpr COVOX_VOLUME = 0x20;

int constexpr CPC_SAVE_VOLUME = 0x03FF;
int constexpr CPC_LOAD_VOLUME = 0x01FF;

uint_fast32_t constexpr fullerClock = 1638190;  // Credit to balford for obtaining this number.
uint_fast32_t constexpr fullerPeriod = static_cast<uint_fast32_t>(1e14 / static_cast<double>(fullerClock));

enum class SoundRate {
    SOUNDRATE_48K,
    SOUNDRATE_128K,
    SOUNDRATE_PENTAGON,
    SOUNDRATE_CPC
};

enum class StereoMode {
    STEREO_MONO,
    STEREO_ABC,
    STEREO_ACB,
    STEREO_TURBO_MONO,
    STEREO_TURBO_ABC,
    STEREO_TURBO_ACB,
    STEREO_NEXT
};

struct Filter {

    uint_fast32_t ticks[FILTER_SIZE] {0};
    uint_fast32_t adder[FILTER_SIZE] {0};
    uint_fast32_t index {0};

    void add(uint_fast16_t sample) {
        adder[index] += sample;
        ++ticks[index];
    }

    uint_fast16_t get() {

        uint_fast32_t count = 0;
        uint_fast32_t sound = 0;
        for (size_t ii = 0; ii < FILTER_SIZE; ++ii) {
            sound += adder[ii];
            count += ticks[ii];
        }

        index = (index + 1) % FILTER_SIZE;
        adder[index] = 0;
        ticks[index] = 0;
        return static_cast<uint_fast16_t>(sound / count);
    }
};
// vim: et:sw=4:ts=4
