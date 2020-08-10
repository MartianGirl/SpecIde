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

enum class AccessType
{
    CRTC_NA,
    CRTC_RO = 1,
    CRTC_WO,
    CRTC_RW
};

class CRTC
{
    public:

        uint_fast8_t type;
        uint_fast8_t index;
        uint_fast8_t regs[32];
        uint_fast8_t dirs[32];

        CRTC(uint_fast8_t type);

        void wrAddress(uint_fast8_t byte);
        void wrRegister(uint_fast8_t byte);
        uint_fast8_t rdStatus();
        uint_fast8_t rdRegister();
        
        void clock();

        uint_fast8_t hCounter = 0;
        uint_fast8_t hswCounter = 0;

        uint_fast8_t vCounter = 0;
        uint_fast8_t vswCounter = 0;
        uint_fast8_t rasterCounter = 0;

        uint_fast16_t byteAddress;
        uint_fast16_t scanAddress;

        bool hDisplay = false;
        bool vDisplay = false;

        bool hSync = false;
        bool vSync = false;
        bool dispEn = false;
};


// vim: et:sw=4:ts=4
