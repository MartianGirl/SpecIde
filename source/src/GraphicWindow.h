#pragma once

/** GraphicWindow
 *
 * Base class for all the graphic windows in SpecIde.
 *
 */

#include <SFML/Graphics.hpp>

#include <string>

class GraphicWindow
{
    public:
        GraphicWindow(uint_fast32_t x = 320, uint_fast32_t y = 240,
                std::string const& title = "SpecIde");
        virtual ~GraphicWindow();

        void open();
        void close();
        void update();

        sf::RenderWindow window;

};

// vim: et:sw=4:ts=4
