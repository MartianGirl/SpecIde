/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

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
