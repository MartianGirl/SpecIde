#pragma once

/** GraphicWindow
 *
 * Base class for all the graphic windows in SpecIde.
 *
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class GraphicWindow
{
    public:
        GraphicWindow(size_t x, size_t y);
        virtual ~GraphicWindow();

        void adjust();

        size_t w, h;
        sf::RenderWindow window;
        std::vector<sf::VideoMode> modes;
        sf::VideoMode bestMode;
        size_t suggestedScansSingle;
        size_t suggestedScansDouble;
};

// vim: et:sw=4:ts=4
