#include "Screen.h"

Screen::Screen(size_t scale) :
    GraphicWindow(352 * scale, 312 * scale, "SpecIde"),
    scale(scale),
    xSize(352), ySize(312),
    xPos(0), yPos(8),
    vSync(false), vSync1d(false),
    hSync(false), hSync1d(false),
    blank(false), 
    frame(0),
    texSize(xSize * ySize),
#if SPECIDE_BYTE_ORDER == 1
    pixels(texSize, 0xFF000000)
#else
    pixels(texSize, 0x000000FF)
#endif
{
    if (!scrTexture.create(static_cast<sf::Uint32>(xSize), static_cast<sf::Uint32>(ySize)))
        assert(false);
    scrTexture.setRepeated(false);
    scrSprite.setTexture(scrTexture);
    scrSprite.setScale(sf::Vector2f(static_cast<float>(scale), static_cast<float>(scale)));
}

void Screen::update()
{
    // If not blanking, draw.
    if (!(*blankInput))
    {
        pixels[yPos * xSize + xPos] = *rgbaInput;
        ++xPos;
    }

    // Act on sync pulses falling edges:
    // VSYNC falling edge restores the beam to the top of the screen.
    if (!(*vSyncInput) && vSync1d)
    {
        yPos = 0;

        scrTexture.update(reinterpret_cast<sf::Uint8*>(&pixels[0]));
        // window.clear(sf::Color::Black);
        window.draw(scrSprite);
        window.display();
    }
    vSync1d = (*vSyncInput);

    // HSYNC falling edge restores the beam to the beginning of the next line.
    if (!(*hSyncInput) && hSync1d)
    {
        xPos = 0;
        ++yPos;
    }
    hSync1d = (*hSyncInput);

    // Move to next pixel with each clock.
}

// vim: et:sw=4:ts=4
