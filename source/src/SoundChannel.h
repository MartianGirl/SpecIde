#pragma once

/** SoundChannel
 *
 * Plays sound from a sound source.
 *
 * It generates 44100Hz, 16-bit sound.
 *
 */

#include <cstddef>
#include <cstdint>
#include <queue>
#include <vector>

#include <SFML/Audio.hpp>

constexpr size_t MAX_SAMPLES = 2048;
constexpr size_t MAX_BUFFERS = 128;

class SoundChannel : public sf::SoundStream
{
    public:
        // The buffers
        std::vector<std::vector<sf::Int16>> buffers;
        std::queue<size_t> queuedBuffers;
        size_t rdBuffer, wrBuffer;

        size_t millis;  // Not really used.
        size_t sampleRate;
        size_t channels;

        SoundChannel() :
            buffers(MAX_BUFFERS, (std::vector<sf::Int16>())),
            rdBuffer(0), wrBuffer(1) {}

        bool open(unsigned int chan, unsigned int rate)
        {
            sampleRate = rate;
            channels = chan;

            initialize(chan, rate);

            // Reserve buffer space.
            for (std::vector<std::vector<sf::Int16>>::iterator it = buffers.begin();
                    it != buffers.end();
                    ++it)
            {
                it->assign(channels * MAX_SAMPLES, 0);
            }

            setAttenuation(0);
            setVolume(100);
            queuedBuffers.push(126);
            queuedBuffers.push(127);
            cout << "Initialized " << channels << " channels ";
            cout << "at " << sampleRate << " Hz." << endl;
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

        void push(int* samples)
        {
            static size_t wrSample = 0;

            for (size_t i = 0; i < channels; ++i)
                buffers[wrBuffer][(channels * wrSample) + i] =
                    static_cast<sf::Int16>(samples[i]);

            if (++wrSample == MAX_SAMPLES)
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
            data.sampleCount = MAX_SAMPLES * channels;
            data.samples = &(buffers[rdBuffer])[0];
            return true;
        }

        virtual void onSeek(sf::Time offset) 
        {
            millis = offset.asMilliseconds();
        }
};

// vim: et:sw=4:ts=4
