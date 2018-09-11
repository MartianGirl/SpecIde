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

#include "GraphicWindow.h"

using namespace std;

GraphicWindow::GraphicWindow(size_t x, size_t y) :
    w(x), h(y),
    window(),
    modes(sf::VideoMode::getFullscreenModes()),
    bestMode(sf::VideoMode::getDesktopMode())
{
    cout << "Selected Full Screen Mode: " << bestMode.width
        << "x" << bestMode.height << "-" << bestMode.bitsPerPixel << endl;

    adjust();
}

void GraphicWindow::adjust()
{
    size_t divider = 0;

    do
    {
        ++divider;
        suggestedScansSingle = bestMode.height / divider;
    } while (suggestedScansSingle > 304); // 312 - 8 VBlank lines.
    cout << "Selected " << suggestedScansSingle
        << " scans for single scan mode." << endl;

    divider = 0;
    do
    {
        ++divider;
        suggestedScansDouble = bestMode.height / divider;
    } while (suggestedScansDouble > 608); // 624 - 16 VBlank lines.
    cout << "Selected " << suggestedScansDouble
        << " scans for double scan mode." << endl;
}

GraphicWindow::~GraphicWindow()
{
    window.close();
}

// vim: et:sw=4:ts=4
