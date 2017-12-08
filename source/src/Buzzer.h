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

#include <SFML/Audio.hpp>

constexpr size_t MAX_SAMPLES = 2048;
constexpr size_t MAX_BUFFERS = 128;
constexpr size_t FILTER_SIZE = 352;

constexpr size_t ULA_CLOCK_48 = 7000000;
constexpr size_t ULA_CLOCK_128 = 7093800;

constexpr sf::Int16 SOUND_VOLUME = 0x10;
constexpr sf::Int16 SAVE_VOLUME = 0x08;
constexpr sf::Int16 LOAD_VOLUME = 0x04;

class Buzzer : public sf::SoundStream
{
    public:
        std::vector<std::vector<sf::Int16>> buffers;
        std::queue<size_t> queuedBuffers;
        size_t rdBuffer, wrBuffer;
        int filter[FILTER_SIZE];

        uint_fast8_t *source;
        uint_fast8_t *tapeIn;
        int *psg;

        size_t millis;  // Dummy
        size_t skip;
        size_t rate;

        bool playSound;
        bool tapeSound;
        bool hasPsg;

        Buzzer() :
            buffers(MAX_BUFFERS, (std::vector<sf::Int16>(MAX_SAMPLES, 0))),
            rdBuffer(0), wrBuffer(1),
            filter{0},
            playSound(true), tapeSound(true), hasPsg(false) {}

        bool open(uint_fast8_t* src, uint_fast8_t* ear, size_t sampleRate)
        {
            source = src;
            tapeIn = ear;
            initialize(1, static_cast<sf::Uint32>(sampleRate));
            setAttenuation(0);
            setVolume(100);
            queuedBuffers.push(126);
            queuedBuffers.push(127);
            rate = sampleRate;
            skip = ULA_CLOCK_48 / rate;
            return true;
        }

        void setPsg(int* sound)
        {
            psg = sound;
        }

        void set128K(bool select128)
        {
            skip = (select128 ? ULA_CLOCK_128 : ULA_CLOCK_48) / rate;
            hasPsg = select128;
        }

        void getNextReadBuffer()
        {
            if (queuedBuffers.empty() == false)
            {
                rdBuffer = queuedBuffers.front();
                queuedBuffers.pop();
            }
        }

        void getNextWriteBuffer()
        {
            do
            {
                wrBuffer = (wrBuffer + 1) % MAX_BUFFERS;
            } while (wrBuffer == rdBuffer);
        }

        void sample()
        {
            static size_t count = 0;
            static size_t wrSample = 0;
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
                buffers[wrBuffer][wrSample] = static_cast<sf::Int16>(s);
                ++wrSample;
                if (wrSample == MAX_SAMPLES)
                {
                    wrSample = 0;
                    queuedBuffers.push(wrBuffer);
                    getNextWriteBuffer();
                }
            }
        }

    private:
        virtual bool onGetData(Chunk& data)
        {
            getNextReadBuffer();
            data.sampleCount = MAX_SAMPLES;
            data.samples = &(buffers[rdBuffer])[0];
            return true;
        }

        virtual void onSeek(sf::Time offset) 
        {
            millis = offset.asMilliseconds();
        }
};

// vim: et:sw=4:ts=4
