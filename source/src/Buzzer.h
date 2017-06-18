#pragma once

/** Buzzer
 *
 * Plays sound from an input BuzzerStream.
 *
 * At the moment it generates 16-bit audio with a 44100Hz sampling rate.
 *
 */

#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

#include <SFML/Audio.hpp>

constexpr size_t MAX_SAMPLES = 2048;
constexpr size_t MAX_BUFFERS = 128;

class Buzzer : public sf::SoundStream
{
    public:
        std::vector<std::vector<sf::Int16>> buffers;
        std::queue<size_t> queuedBuffers;
        size_t rdBuffer, wrBuffer;
        size_t wrSample;

        uint_fast8_t *source;

        size_t millis;  // Dummy

        Buzzer() :
            buffers(MAX_BUFFERS, (std::vector<sf::Int16>(MAX_SAMPLES, 0))),
            rdBuffer(0), wrBuffer(1),
            wrSample(0) {}

        bool open(uint_fast8_t* src, size_t sampleRate)
        {
            source = src;
            initialize(1, sampleRate);
            setAttenuation(0);
            setVolume(100);
            queuedBuffers.push(126);
            queuedBuffers.push(127);
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
            buffers[wrBuffer][wrSample] = 0x0000;
            buffers[wrBuffer][wrSample] += (*source & 0x10) ? 0x3FFF : -0x3FFF;
            buffers[wrBuffer][wrSample] += (*source & 0x08) ? 0x07FF : -0x07FF;
            ++wrSample;
            if (wrSample == MAX_SAMPLES)
            {
                wrSample = 0;
                queuedBuffers.push(wrBuffer);
                getNextWriteBuffer();
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
