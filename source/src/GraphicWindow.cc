#include "GraphicWindow.h"

using namespace std;

GraphicWindow::GraphicWindow(size_t x, size_t y, std::string const& title) :
    w(x), h(y),
    window(
        sf::VideoMode(static_cast<sf::Uint32>(x), static_cast<sf::Uint32>(y)),
        title, sf::Style::Close | sf::Style::Titlebar),
    modes(sf::VideoMode::getFullscreenModes())
{
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);
    window.clear();
    window.display();

    // List video modes
    // for (size_t i = 0; i < modes.size(); ++i)
    // {
        // sf::VideoMode mode = modes[i];
        // cout << "Mode #" << i << ": "
            // << mode.width << "x" << mode.height << " - "
            // << mode.bitsPerPixel << "bpp" << endl;
    // }
    bestMode = modes[0];
    cout << "Selecting Full Screen Mode: "
        << bestMode.width << "x" << bestMode.height << " - "
        << bestMode.bitsPerPixel << "bpp" << endl;

    size_t divider = 0;
    do
    {
        ++divider;
        suggestedScans = bestMode.height / divider;
    } while (suggestedScans > 304); // 312 - 8 VBlank lines.
    cout << "Using a texture " << suggestedScans << " lines high." << endl;
}


void GraphicWindow::open()
{
    window.setVisible(true);
}

void GraphicWindow::close()
{
    window.setVisible(false);
}

GraphicWindow::~GraphicWindow()
{
    window.close();
}

// vim: et:sw=4:ts=4
