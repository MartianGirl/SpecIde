#pragma once

/** GraphicWindow
 *
 * Base class for all the graphic windows in SpecIde.
 *
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cstddef>
#include <string>

class GraphicWindow
{
    public:
        GraphicWindow(size_t x = 320, size_t y = 240,
                std::string const& title = "SpecIde");
        virtual ~GraphicWindow();

        void open();
        void close();
        void update();

        sf::RenderWindow window;
};

// vim: et:sw=4:ts=4
