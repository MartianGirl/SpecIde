#include "Screen.h"

Screen::Screen(size_t scale) :
    GraphicWindow(320 * scale, 200 * scale, "SpecIde")
{
}

Screen::~Screen()
{
}

// vim: et:sw=4:ts=4
