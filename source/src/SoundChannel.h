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

constexpr size_t MAX_BUFFERS = 16;
constexpr size_t MAX_SAMPLES = 2048;

class SoundChannel : public sf::SoundStream {

    public:
        // The buffers
        std::vector<std::vector<sf::Int16>> buffers;
        std::vector<size_t> samples;
        size_t rdBuffer = 0;
        size_t wrBuffer = 0;
        size_t wrSample = 0;

        std::list<size_t> bufList;
        std::mutex m;
        std::condition_variable cv;

        uint32_t channels;

        bool destroy = false;

        SoundChannel() :
            buffers(MAX_BUFFERS, (std::vector<sf::Int16>())),
            samples(MAX_BUFFERS, 0) {}

        bool open(unsigned int chan, unsigned int rate) {

            channels = chan;

            initialize(chan, rate);
            bufList.clear();

            // Reserve buffer space
            for (std::vector<std::vector<sf::Int16>>::iterator it = buffers.begin();
                    it != buffers.end(); ++it) {
                it->assign(channels * MAX_SAMPLES, 0);
            }

            setAttenuation(0);
            setVolume(100);
            cout << "Initialized " << channels << " channels ";
            cout << "at " << rate << " Hz." << endl;
            return true;
        }

        void push(int l, int r) {

            buffers[wrBuffer][2 * wrSample + 0] = static_cast<sf::Int16>(l);
            buffers[wrBuffer][2 * wrSample + 1] = static_cast<sf::Int16>(r);
            wrSample = (wrSample + 1) % MAX_SAMPLES;
        }

        void reset() {

            std::lock_guard<std::mutex> lock(m);
            samples.assign(MAX_BUFFERS, 0);
            wrSample = 0;
            wrBuffer = 0;
            rdBuffer = 0;
            bufList.clear();
            cv.notify_one();
        }

        bool commit() {

            std::lock_guard<std::mutex> lock(m);

            samples[wrBuffer] = channels * wrSample;
            wrSample = 0;

            if (bufList.size() < MAX_BUFFERS) {
                bufList.push_back(wrBuffer);
                do {
                    wrBuffer = (wrBuffer + 1) % MAX_BUFFERS;
                } while ((wrBuffer == rdBuffer)
                        || (find(bufList.begin(), bufList.end(), wrBuffer) != bufList.end()));
            } else {
                // This should only happen if the user is starting and stopping the playback
                // repeatedly. In this case, it is not a problem to lose some audio.
                reset();
            }

            bool awake = (bufList.size() >= 3);
            cv.notify_one();

            return awake;
        }

        void close() {

            destroy = true;
            cv.notify_one();
        }

    private:
        virtual bool onGetData(Chunk& data) {

            std::unique_lock<std::mutex> lock(m);

            while (!destroy && bufList.empty()) {
                cv.wait(lock);
            }

            if (destroy) return false;

            rdBuffer = bufList.front();
            bufList.pop_front();

            data.sampleCount = samples[rdBuffer];
            data.samples = &(buffers[rdBuffer])[0];
            return true;
        }

        virtual void onSeek(sf::Time offset) {

            (void) offset;
        }
};

// vim: et:sw=4:ts=4
