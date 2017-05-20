#pragma once

/** Screen
 *
 * Default Screen implementation.
 *
 * This class emulates a CRT. It draws an image by scanning the pixels
 * in a zig-zag way, just like a TV screen does.
 *
 */

#include "GraphicWindow.h"

class Screen : public GraphicWindow
{
    public:
        Screen(size_t scale = 1);

        size_t scale;
        size_t xPos, yPos;
        bool vSync, vSync1d;
        bool hSync, hSync1d;
        bool blank;
        bool frame;

        sf::RectangleShape pixel;

        void update(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b);





};

// vim: et:sw=4:ts=4
