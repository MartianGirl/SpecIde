#pragma once

/** FDC
 *
 * FDC765 implementation.
 *
 * - Operates in DMA/non-DMA mode. I will implement first (only?) non-DMA,
 *   which is needed for Spectrum +3 and Amstrad CPC 6128.
 */

#include <cassert>
#include <cstdint>

using namespace std;

enum class FDCState
{
    FDC_STATE_IDLE,
    FDC_STATE_COMMAND,
    FDC_STATE_EXECUTION,
    FDC_STATE_RESULT
};

constexpr uint_fast8_t SREG_DB0 = 1 << 0;
constexpr uint_fast8_t SREG_DB1 = 1 << 1;
constexpr uint_fast8_t SREG_DB2 = 1 << 2;
constexpr uint_fast8_t SREG_DB3 = 1 << 3;
constexpr uint_fast8_t SREG_CB = 1 << 4;
constexpr uint_fast8_t SREG_EXM = 1 << 5;
constexpr uint_fast8_t SREG_DIO = 1 << 6;
constexpr uint_fast8_t SREG_RQM = 1 << 7;

class FDC
{
    public:
        uint_fast8_t statusReg = 0x00;
        uint_fast8_t sRegs[4];

        uint_fast8_t cmdBuffer[16];
        uint_fast8_t resBuffer[16];

        unsigned int cmdIndex;
        unsigned int resIndex;

        unsigned int cmdBytes;
        unsigned int resBytes;

        bool byte = false;

        FDC() {} 

        FDCState state;

        void clock()
        {
            switch (state)
            {
                case FDC_STATE_IDLE:
                    statusReg = SREG_RQM;
                    if (byte)
                    {
                        byte = false;
                        if (checkCommand())
                            state = FDC_STATE_COMMAND;
                        else
                            reset();
                    }
                    break;

                case FDC_STATE_COMMAND:
                    statusReg = SREG_RQM | SREG_CB;
                    if (byte)
                    {
                        byte = false;
                        if (cmdIndex == cmdBytes)
                            state = FDC_STATE_EXECUTION;
                        else if (cmdIndex > cmdBytes)
                            reset();
                    }
                    break;

                case FDC_STATE_EXECUTION:
                    statusReg = SREQ_EXM | SREQ_CB;
                    execute();
                    break;

                case FDC_STATE_RESULT:
                    statusReg = SREQ_RQM | SREQ_DIO | SREQ_CB;
                    if (byte)
                    {
                        byte = false;
                        if (resIndex >= resBytes)
                            reset();
                    }
                    break;
            }
        }

        bool checkCommand()
        {
            switch (cmdBuffer[0] & 0x1F)
            {
                case 0x02:  // Read Track
                    cmdBytes = 9;   // 02+MF+SK    HU TR HD ?? SZ NM GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD NM SZ
                    return true;

                case 0x03:  // Specify SPD/DMA
                    cmdBytes = 3;   // 03          XX YY
                    resBytes = 0;   //
                    return true;

                case 0x04:  // Sense drive state
                    cmdBytes = 2;   // 04          HU
                    resBytes = 1;   //             S3
                    return true;

                case 0x05:  // Write sector(s)
                    cmdBytes = 9;   // 05+MT+MF    HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x06:  // Read sector(s)
                    cmdBytes = 9;   // 06+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x07:  // Recalibrate and seek physical track 0
                    cmdBytes = 2;   // 07          HU
                    resBytes = 0;
                    return true;

                case 0x08:  // Sense internal state
                    cmdBytes = 1;   // 08
                    resBytes = 2;   //             S0 TP
                    return true;

                case 0x09:  // Write deleted sector(s)
                    cmdBytes = 9;   // 09+MT+MF    HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x0A:  // Read ID
                    cmdBytes = 2;   // 0A+MF       HU
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x0C:  // Read deleted sector(s)
                    cmdBytes = 9;   // 0C+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x0D:  // Format track
                    cmdBytes = 6;   // 0D+MF       HU SZ NM GP FB
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x0F:  // Seek track N
                    cmdBytes = 3;   // 0F          HU TP
                    resBytes = 0;
                    return true;

                case 0x11:  // Scan equal
                    cmdBytes = 9;   // 11+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x19:  // Scan low or equal
                    cmdBytes = 9;   // 19+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                case 0x1D:  // Scan high or equal
                    cmdBytes = 9;   // 1D+MT+MF+SK HU TR HD SC SZ LS GP SL
                    resBytes = 7;   //             S0 S1 S2 TR HD LS SZ
                    return true;

                default:
                    cmdBytes = 0;
                    resBytes = 0;
                    return false;
            }
        }

        void execute()
        {

        }

        void reset()
        {
            state = FDC_STATE_IDLE;
            byte = false;
            cmdIndex = resIndex = 0;
            cmdBytes = resBytes = 0;
            statusReg = SREG_RQM;
        }

        uint_fast8_t status() { return statusReg; }

        uint_fast8_t read()
        {
            static uint_fast8_t retval;

            if (byte == false)
            {
                retval = resBuffer[resIndex++];
                byte = true;
            }

            return retval;
        }

        void write(uint_fast8_t byte)
        {
            static uint_fast8_t wait = 0;

            if (byte == false && ++wait == 5)
            {
                wait = 0;
                cmdBuffer[cmdIndex++] = byte;
                byte = true;
            }
        }
};

// vim: et:sw=4:ts=4
