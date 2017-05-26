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

#include <cassert>
#include <vector>

class Screen : public GraphicWindow
{
    public:
        Screen(size_t scale = 1);

        size_t scale;
        size_t xSize, ySize;

        size_t xPos, yPos;
        bool vSync, vSync1d;
        bool hSync, hSync1d;
        bool blank;
        bool frame;

        size_t texSize;
        sf::Texture scrTexture;
        sf::Sprite scrSprite;
        std::vector<sf::Uint32> pixels;

        void update();
        void setRgbaInput(sf::Uint32* input) { rgbaInput = input; }

        sf::Uint32 *rgbaInput;
};

// vim: et:sw=4:ts=4
