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

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <list>
#include <mutex>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

// Number of buffers.
constexpr size_t MAX_BUFFERS = 16;
// Number of samples per buffer.
// Considering a single frame (0.02s) at 44100Hz total 882 samples.
constexpr size_t MAX_SAMPLES = 2048;
// Number of buffers to complete before we start playing.
constexpr uint32_t PRELOAD_BUFFERS = 3;

class SoundChannel : public sf::SoundStream {

    // Private constructor. This is a Singleton now.
    SoundChannel() :
        buffers(MAX_BUFFERS, (std::vector<sf::Int16>())),
        samples(MAX_BUFFERS, 0) {}

    public:
        // The buffers.
        std::vector<std::vector<sf::Int16>> buffers;
        // The number of samples stored in each buffer.
        std::vector<size_t> samples;
        // The buffer currently being read.
        size_t rdBuffer = 0;
        // The buffer currently being written to.
        size_t wrBuffer = 0;
        // The number of samples written.
        size_t wrSample = 0;

        // This list holds the completed buffers.
        std::list<size_t> queuedBuffers;

        std::mutex m;
        std::condition_variable cv;

        uint32_t channels;

        bool destroy = false;

        // Singleton getter.
        static SoundChannel& getChannel() {

            static SoundChannel theChannel;
            return theChannel;
        }

        bool open(unsigned int chan, unsigned int rate) {

            channels = chan;

            initialize(chan, rate);
            queuedBuffers.clear();

            // Reserve buffer space
            for (std::vector<std::vector<sf::Int16>>::iterator it = buffers.begin();
                    it != buffers.end(); ++it) {
                it->assign(channels * MAX_SAMPLES, 0);
            }

            setAttenuation(0);
            setVolume(100);
            return true;
        }

        void setSleepInterval(uint32_t interval) {
            setProcessingInterval(sf::milliseconds(interval));
        }

        void push(int l, int r) {

            buffers[wrBuffer][2 * wrSample + 0] = static_cast<sf::Int16>(l);
            buffers[wrBuffer][2 * wrSample + 1] = static_cast<sf::Int16>(r);
            // If we exceed MAX_SAMPLES, we wrap the buffer around. However, this
            // should not happen, because MAX_SAMPLES is well over the expected
            // number of samples per frame.
            wrSample = (wrSample + 1) % MAX_SAMPLES;
        }

        bool commit() {

            // Lock so we don't interfere with the playing thread.
            std::lock_guard<std::mutex> lock(m);
            // If we've queued every buffer something wrong is happening.
            if (queuedBuffers.size() < MAX_BUFFERS) {
                // Store the number of samples, queue the buffer and reset the index
                // to start with a new buffer.
                samples[wrBuffer] = channels * wrSample;
                queuedBuffers.push_back(wrBuffer);
                wrSample = 0;
                // Select the next buffer to write to. Skip any of the buffers that are
                // already queued.
                do {
                    wrBuffer = (wrBuffer + 1) % MAX_BUFFERS;
                } while ((wrBuffer == rdBuffer)
                        || (find(queuedBuffers.begin(), queuedBuffers.end(), wrBuffer) != queuedBuffers.end()));
            } else {
                // This should only happen if the user is starting and stopping the playback
                // repeatedly. In this case, it is not a problem to lose some audio.
                samples.assign(MAX_BUFFERS, 0);
                wrSample = 0;
                wrBuffer = 0;
                rdBuffer = 0;
                queuedBuffers.clear();
            }
            cv.notify_one();
            return (queuedBuffers.size() >= PRELOAD_BUFFERS);
        }

        void close() {

            destroy = true;
            cv.notify_one();
        }

    private:
        virtual bool onGetData(Chunk& data) {

            std::unique_lock<std::mutex> lock(m);
            while (!destroy && queuedBuffers.empty()) {
                cv.wait(lock);
            }

            bool play = false;
            if (!destroy) {
                rdBuffer = queuedBuffers.front();
                queuedBuffers.pop_front();
                data.sampleCount = samples[rdBuffer];
                data.samples = &(buffers[rdBuffer])[0];
                play = true;
            }
            return play;
        }

        virtual void onSeek(sf::Time offset) {

            (void) offset;
        }
};

// vim: et:sw=4:ts=4
