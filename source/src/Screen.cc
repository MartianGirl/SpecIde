#include "Screen.h"

Screen::Screen(size_t scale) :
    GraphicWindow(352 * scale, 312 * scale, "SpecIde"),
    scale(scale),
    xPos(0), yPos(0),
    vSync(false), hSync(false), blank(false), frame(false),
    pixel(sf::Vector2f(scale, scale))
{
    // Define default values for pixels.
    pixel.setOrigin(0, 0);
    pixel.setOutlineColor(sf::Color::Transparent);
    pixel.setOutlineThickness(0);
}

void Screen::update(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b)
{
    // Act on sync pulses rising edges.
    if (!vSync & vSync1d)
    {
        yPos = 0;
        window.display();
    }
    vSync1d = vSync;

    if (!hSync & hSync1d)
    {
        ++yPos;
        xPos = 0;
    }
    hSync1d = hSync;

    ++xPos;

    if (!blank)
    {
        pixel.setPosition(xPos * scale, yPos * scale);
        pixel.setFillColor(sf::Color(r, g, b));
        window.draw(pixel);
    }
}

// vim: et:sw=4:ts=4
