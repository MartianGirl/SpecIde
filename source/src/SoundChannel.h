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

constexpr size_t MAX_BUFFERS = 32;

class SoundChannel : public sf::SoundStream {

    public:
        // The buffers
        std::vector<std::vector<sf::Int16>> buffers;
        std::queue<size_t> queuedBuffers;
        size_t rdBuffer = 0;
        size_t wrBuffer = 0;
        size_t wrSample = 0;

        uint32_t millis;  // Not really used.
        uint32_t sampleRate;
        uint32_t channels;

        uint32_t bufferFill = 6;
        uint32_t bufferSize = 1024;

        bool playing = false;

        SoundChannel() :
            buffers(MAX_BUFFERS, (std::vector<sf::Int16>())) {}

        bool open(unsigned int chan, unsigned int rate) {

            sampleRate = rate;
            channels = chan;

            initialize(chan, rate);

            // Reserve buffer space
            for (std::vector<std::vector<sf::Int16>>::iterator it = buffers.begin();
                    it != buffers.end(); ++it) {
                it->assign(channels * bufferSize, 0);
            }

            setAttenuation(0);
            setVolume(100);
            cout << "Initialized " << channels << " channels ";
            cout << "at " << sampleRate << " Hz." << endl;
            return true;
        }

        void getNextReadBuffer() {

            // If there is no queued buffers, complete current buffer
            if (!queuedBuffers.empty()) {
                rdBuffer = queuedBuffers.front();
                queuedBuffers.pop();
            } else {
                playing = false;
            }
        }

        void getNextWriteBuffer() {

            do {
                wrBuffer = (wrBuffer + 1) % MAX_BUFFERS;
            } while (wrBuffer == rdBuffer);
        }

        bool push(int l, int r) {

            bool complete = false;
            buffers[wrBuffer][2 * wrSample + 0] = static_cast<sf::Int16>(l);
            buffers[wrBuffer][2 * wrSample + 1] = static_cast<sf::Int16>(r);

            if (++wrSample == bufferSize) {
                wrSample = 0;
                queuedBuffers.push(wrBuffer);
                getNextWriteBuffer();
                complete = (queuedBuffers.size() >= bufferFill);
            }

            return complete;
        }

        void reset() {
            stop();
            wrSample = 0;
            wrBuffer = 0;
            rdBuffer = 0;
            while (queuedBuffers.size()) {
                queuedBuffers.pop();
            }
        }

    private:
        virtual bool onGetData(Chunk& data) {

            getNextReadBuffer();
            data.sampleCount = bufferSize * channels;
            data.samples = &(buffers[rdBuffer])[0];
            return playing;
        }

        virtual void onSeek(sf::Time offset) {

            millis = offset.asMilliseconds();
        }
};

// vim: et:sw=4:ts=4
