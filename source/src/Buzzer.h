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
constexpr size_t ULA_CLOCK = 7000000;

class Buzzer : public sf::SoundStream
{
    public:
        std::vector<std::vector<sf::Int16>> buffers;
        std::queue<size_t> queuedBuffers;
        size_t rdBuffer, wrBuffer;

        uint_fast8_t *source;
        uint_fast8_t *tapeIn;

        size_t millis;  // Dummy
        size_t skip;

        bool playLoadingSound;

        Buzzer() :
            buffers(MAX_BUFFERS, (std::vector<sf::Int16>(MAX_SAMPLES, 0))),
            rdBuffer(0), wrBuffer(1),
            playLoadingSound(true) {}

        bool open(uint_fast8_t* src, uint_fast8_t* ear, size_t sampleRate)
        {
            source = src;
            tapeIn = ear;
            initialize(1, static_cast<sf::Uint32>(sampleRate));
            setAttenuation(0);
            setVolume(100);
            queuedBuffers.push(126);
            queuedBuffers.push(127);
            skip = ULA_CLOCK / sampleRate;
            return true;
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

            if (++count == skip)
            {
                count = 0;
                buffers[wrBuffer][wrSample] = (*source & 0x10) ? 0x3FFF : 0x0000;
                buffers[wrBuffer][wrSample] += 
                    (playLoadingSound && (*tapeIn & 0x40)) ? 0x03FF : 0x0000;
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
