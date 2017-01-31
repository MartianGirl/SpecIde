#pragma once

/** Screen
 *
 * Default Screen implementation.
 */

#include "GraphicWindow.h"

class Screen : public GraphicWindow
{
    public:
        Screen(uint_fast32_t scale = 1);
        virtual ~Screen();
};

// vim: et:sw=4:ts=4
