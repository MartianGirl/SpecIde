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

size_t constexpr FILTER_BZZ_SIZE = 96;
size_t constexpr FILTER_PSG_SIZE = 96;
uint32_t constexpr BASE_CLOCK_48 = 7000000;
uint32_t constexpr BASE_CLOCK_128 = 7093800;
uint32_t constexpr BASE_CLOCK_CPC = 16000000;
int constexpr FRAME_TIME_48 = 19968;
int constexpr FRAME_TIME_128 = 19992;
int constexpr FRAME_TIME_PENTAGON = 20480;
int constexpr FRAME_TIME_50HZ = 20000;

uint32_t constexpr SAMPLE_RATE = 44100;

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
// vim: et:sw=4:ts=4
