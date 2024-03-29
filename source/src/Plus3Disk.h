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

/** Plus3Disk
 *
 * Plus3 Disk drive emulation.
 *
 */

#include "DSKFile.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class Plus3Disk {

    public:
        Plus3Disk(bool ready = false) :
            ready(ready) {}

        bool track0 = true;     // Head is at track 0 (center of the disk)
        bool fault = false;     // Error
        bool disk = false;      // Disk is in drive
        bool writeprot = false; // Disk is write protected
        bool ready = false;     // Drive exists
        bool motor = false;     // Drive motor is spinning.

        size_t cylinder = 0;
        size_t sector = 0;
        size_t hole = 0;
        size_t next[2];

        uint_fast8_t idTrack;
        uint_fast8_t idHead;
        uint_fast8_t idSector;
        uint_fast8_t idSize;
        uint_fast8_t statusReg1;
        uint_fast8_t statusReg2;
        uint_fast16_t length;
        uint_fast8_t filler;
        uint_fast8_t gap;
        vector<uint8_t> buffer;

        vector<DSKFile> images;
        vector<string> imageNames;
        size_t currentImage;

        /**
         * Advance disk to next sector.
         */
        void nextSector() {

            if (motor) {
                ++sector;
            }

            if (disk) {
                size_t tr = (images[currentImage].numSides * cylinder);

                if (images[currentImage].tracks[tr].trackSize) {
                    for (size_t ii = 0; ii < images[currentImage].numSides; ++ii) {
                        next[ii] = sector % images[currentImage].tracks[tr + ii].numSectors;
                    }
                } else {
                    next[0] = 0;
                    next[1] = 0;
                }
            }

            if (next[0] == 0) {
                countHole();
            }
        }

        /**
         * Seek next track.
         */
        void nextTrack() {

            size_t limit = disk ? images[currentImage].numTracks - 1 : 76;

            if (++cylinder > limit) {
                cylinder = limit;
            }
            track0 = (cylinder == 0);
        }

        /**
         * Seek previous track.
         */
        void prevTrack() {

            if (cylinder > 0) {
                --cylinder;
            }
            track0 = (cylinder == 0);
        }

        /**
         * Return disk drive status byte.
         */
        uint_fast8_t senseStatus() {

            return ((disk && images[currentImage].numSides == 2) ? 0x08 : 0x00)
                | (track0 ? 0x10 : 0x00)
                | (ready ? 0x20 : 0x00)
                | (writeprot ? 0x40 : 0x00)
                | (fault ? 0x80 : 0x00);        // Don't really know...
        }

        /**
         * Write sector data to disk.
         *
         * This function writes data from the disk drive buffer to
         * the disk's current sector.
         *
         */
        void writeSector(int head) {

            if (disk && head < images[currentImage].numSides) {
                size_t tr = (images[currentImage].numSides * cylinder) + head;
                size_t sc = sector % images[currentImage].tracks[tr].numSectors;

                // If the track is formatted, write.
                if ((tr < images[currentImage].numTracks * images[currentImage].numSides)
                        && images[currentImage].tracks[tr].trackSize) {
                    images[currentImage].tracks[tr].sectors[sc].data = buffer;
                    images[currentImage].tracks[tr].sectors[sc].fdcStatusReg1 = statusReg1;
                    images[currentImage].tracks[tr].sectors[sc].fdcStatusReg2 = statusReg2;
                }
            }
        }

        /**
         * Read sector data into buffer.
         *
         * This function reads the data from the current sector, and
         * copies it to the disk drive buffer.
         */
        void readSector(int head) {

            if (disk && head < images[currentImage].numSides) {
                size_t tr = (images[currentImage].numSides * cylinder) + head;
                size_t sc = sector % images[currentImage].tracks[tr].numSectors;

                // If the track is formatted, read.
                if ((tr < images[currentImage].numTracks * images[currentImage].numSides)
                        && images[currentImage].tracks[tr].trackSize) {
                    idTrack = images[currentImage].tracks[tr].sectors[sc].track;
                    idHead = images[currentImage].tracks[tr].sectors[sc].side;
                    idSector = images[currentImage].tracks[tr].sectors[sc].sectorId;
                    idSize = images[currentImage].tracks[tr].sectors[sc].sectorSize;
                    statusReg1 = images[currentImage].tracks[tr].sectors[sc].fdcStatusReg1;
                    statusReg2 = images[currentImage].tracks[tr].sectors[sc].fdcStatusReg2;
                    buffer = images[currentImage].tracks[tr].sectors[sc].data;
                    length = images[currentImage].tracks[tr].sectors[sc].sectorLength;
                    gap = images[currentImage].tracks[tr].gapLength;
                    filler = images[currentImage].tracks[tr].fillerByte;
                } else {
                    idTrack = rand() & 0xFF;
                    idHead = rand() & 0xFF;
                    idSector = rand() & 0xFF;
                    idSize = rand() & 0xFF;
                    statusReg1 = 0x25;  // 00100101: DE, ND, MAM
                    statusReg2 = 0x20;  // 00110011: DD, WC, BC, MD
                    length = 0;
                    filler = 0;
                }
            }
            // Should plan for no disk or wrong head.
        }

        void formatTrack(int head,
                uint_fast8_t trackNumber, uint_fast8_t sideNumber,
                uint_fast8_t sectorSize, uint_fast8_t numSectors,
                uint_fast8_t gapLength, uint_fast8_t fillerByte) {

            size_t tr = (images[currentImage].numSides * cylinder) + head;

            while (tr >= images[currentImage].tracks.size()) {
                images[currentImage].tracks.push_back(DSKFile::Track());
            }
            
            images[currentImage].tracks[tr].trackNumber = trackNumber;
            images[currentImage].tracks[tr].sideNumber = sideNumber;
            images[currentImage].tracks[tr].sectorSize = sectorSize;
            images[currentImage].tracks[tr].numSectors = numSectors;
            images[currentImage].tracks[tr].gapLength = gapLength;
            images[currentImage].tracks[tr].fillerByte = fillerByte;
            images[currentImage].tracks[tr].sectors.assign(numSectors, DSKFile::Track::Sector(
                        trackNumber, sideNumber, 0x00, sectorSize, 0x00, 0x00));
            images[currentImage].tracks[tr].trackSize = ((0x80 << sectorSize) * numSectors) + 0x100;
            images[currentImage].tracks[tr].magicOk = true;
        }

        void formatSector(int head,
                uint_fast8_t idTr, uint_fast8_t idHd,
                uint_fast8_t idSc, uint_fast8_t idSz) {

            if (disk && head < images[currentImage].numSides) {
                size_t tr = (images[currentImage].numSides * cylinder) + head;
                size_t sc = sector % images[currentImage].tracks[tr].numSectors;

                images[currentImage].tracks[tr].sectors[sc].track = idTr;
                images[currentImage].tracks[tr].sectors[sc].side = idHd;
                images[currentImage].tracks[tr].sectors[sc].sectorId = idSc;
                images[currentImage].tracks[tr].sectors[sc].sectorSize = idSz;
                images[currentImage].tracks[tr].sectors[sc].fdcStatusReg1 = 0x00;
                images[currentImage].tracks[tr].sectors[sc].fdcStatusReg2 = 0x00;
                images[currentImage].tracks[tr].sectors[sc].sectorLength = buffer.size();
                images[currentImage].tracks[tr].sectors[sc].data = buffer;
            }
        }

        void recalibrate() {

            // Maybe put here a delay.
            if (cylinder < 78) {
                cylinder = 0;
                track0 = true;
            } else {
                cylinder -= 77;
            }
        }

        /**
         * Increase the counter of hole sightings.
         *
         * Disks have a small hole near the center of the magnetic surface
         * that is used by disk drives for counting entire turns. Operations
         * that must be performed along an entire track finish when the hole
         * has been detected twice, which ensures that a track has been
         * under the writing head from beginning to end.
         *
         */
        void countHole() {

            sector = 0;
            ++hole;
        }

        void nextDisk() {
            if (images.size() > 0) {
                ++currentImage;
                if (currentImage == images.size()) {
                    currentImage = 0;
                }
                cout << "Currently inserted disk: " << imageNames[currentImage] << endl;
            }
        }

        void prevDisk() {

            if (images.size() > 0) {
                if (currentImage == 0) {
                    currentImage = images.size();
                }
                --currentImage;
                cout << "Currently inserted disk: " << imageNames[currentImage] << endl;
            }
        }

        void emptyDisk() {

            static size_t disks = 0;

            stringstream ss;
            ss << "Empty Disk " << disks;
            ++disks;

            DSKFile dsk;
            dsk.makeEmpty();
            images.push_back(dsk);
            imageNames.push_back(ss.str());
            disk = true;

            currentImage = images.size() - 1;
            cout << "Currently inserted disk: " << imageNames[currentImage] << endl;
        }

        void saveDisk() {

            static size_t disks = 0;

            if (disk) {
                stringstream ss;
                ss << "savedisk" << dec << setw(2) << setfill('0') << disks << ".dsk";
                string name = ss.str();

                cout << "Saving to " << name << endl;
                images[currentImage].save(name);

                disks = (disks + 1) % 100;
            }
        }
};

// vim: et:sw=4:ts=4
