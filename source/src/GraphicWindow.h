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
        GraphicWindow(size_t x = 320, size_t y = 240,
                std::string const& title = "SpecIde");
        virtual ~GraphicWindow();

        void open();
        void close();
        void update() {}

        size_t w, h;
        sf::RenderWindow window;
        std::vector<sf::VideoMode> modes;
        sf::VideoMode bestMode;
        size_t suggestedScans;
};

// vim: et:sw=4:ts=4
