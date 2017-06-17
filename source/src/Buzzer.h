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
#include <vector>

#include <SFML/Audio.hpp>

constexpr size_t MAX_SAMPLES = 15000;
constexpr size_t SAMPLES_PER_CHUNK = 1500;
constexpr size_t CHUNKS = 10;

class Buzzer : public sf::SoundStream
{
    public:
        std::vector<sf::Int16> samples;
        size_t numSamples;
        size_t frame;

        uint_fast8_t *source;

        Buzzer() :
            samples(MAX_SAMPLES, 0),
            numSamples(1500), frame(0) {}


        bool open(uint_fast8_t* src)
        {
            source = src;
            initialize(1, 44100);
            setAttenuation(0);
            setVolume(50);
            return true;
        }

        void sample()
        {
            samples[numSamples] = (*source & 0x18) ? 0xBFFF : 0x0000;
            numSamples = (numSamples + 1) % MAX_SAMPLES;
        }

    private:
        virtual bool onGetData(Chunk& data)
        {
            data.sampleCount = SAMPLES_PER_CHUNK;
            data.samples = &samples[frame * SAMPLES_PER_CHUNK];
            frame = (frame + 1) % CHUNKS;
            return true;
        }

        virtual void onSeek(sf::Time offset) {}
};

// vim: et:sw=4:ts=4
