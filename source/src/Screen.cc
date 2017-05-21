#include "Screen.h"

Screen::Screen(size_t scale) :
    GraphicWindow(352 * scale, 312 * scale, "SpecIde"),
    scale(scale),
    xSize(352), ySize(312),
    xPos(0), yPos(0),
    vSync(false), hSync(false), blank(false), frame(false),
    texSize(xSize * ySize),
    pixels(texSize, 0xFF)
{
    if (!scrTexture.create(xSize, ySize)) assert(false);
    scrTexture.setRepeated(false);
    scrSprite.setTexture(scrTexture);
    scrSprite.setScale(sf::Vector2f(scale, scale));
}

void Screen::update(uint8_t r, uint8_t g, uint8_t b)
{
    // If not blanking, draw.
    if (!blank)
    {
#if SPECIDE_BYTE_ORDER == 1
        sf::Uint32 pixel = r << 24 | g << 16 | b << 8 | 0xFF;
#else
        sf::Uint32 pixel = 0xFF << 24 | b << 16 | g << 8 | r;
#endif
        *(&pixels[0] + yPos * xSize + xPos) = pixel;
        ++xPos;
    }

    // Act on sync pulses falling edges:
    // VSYNC falling edge restores the beam to the top of the screen.
    if (!vSync && vSync1d)
    {
        yPos = 0;
        scrTexture.update(reinterpret_cast<sf::Uint8*>(&pixels[0]));
        window.clear(sf::Color::Black);
        window.draw(scrSprite);
        window.display();
#if SPECIDE_BYTE_ORDER == 1
        pixels.assign(texSize, 0x000000FF);
#else
        pixels.assign(texSize, 0xFF000000);
#endif
    }
    vSync1d = vSync;

    // HSYNC falling edge restores the beam to the beginning of the next line.
    if (!hSync && hSync1d)
    {
        xPos = 0;
        ++yPos;
    }
    hSync1d = hSync;

    // Move to next pixel with each clock.
}

// vim: et:sw=4:ts=4
