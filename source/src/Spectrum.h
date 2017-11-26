#pragma once

/** Spectrum
 *
 * A Spectrum computer
 *
 */

#include "Memory.h"
#include "ULA.h"
#include "Z80.h"
#include "Z80Defs.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

class Spectrum
{
    public:
        Spectrum();

        // Required hardware.
        Z80 z80;
        ULA ula;

        // Kempston Joystick.
        uint_fast8_t joystick;
        bool kempston;
        bool spectrum128K;
        bool spectrumPlus2;
        uint_fast8_t idle;
        uint_fast8_t paging;

        size_t ramBank;
        size_t romBank;
        size_t scrBank;
        bool contendedRam;
        bool contendedRom;

        // I'm going to exceed the original requirements, and then limit the
        // configuration to the required model.
        Memory ram[64]; // 1024KB
        Memory rom[16]; // 256KB - Speccy uses 16K, 128K uses 32K, +3 uses 64K
        Memory* map[4];

        // sf::SoundBuffer buzzer;

        // This one is going to be called at 7MHz, and is going to:
        // 1. Clock the ULA. This starts the ULA counters.
        // 2. Access memory for the ULA, if the ULA is not high impedance.
        // 3. Clock the Z80, if the ULA says so.
        // 4. Access the memory for the Z80.
        void clock();
        void reset();

        // sf::SoundBuffer ear;
        // sf::SoundBuffer mic;

        void loadRoms(size_t model);
        void initMems(size_t model);
        void set128K();
        void setPlus2();
        void updatePage();
};

// vim: et:sw=4:ts=4
