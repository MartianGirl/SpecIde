#include "GraphicWindow.h"

using namespace std;

GraphicWindow::GraphicWindow(size_t x, size_t y, std::string const& title) :
    w(x), h(y),
    window(
        sf::VideoMode(static_cast<sf::Uint32>(x), static_cast<sf::Uint32>(y)),
        title),
    modes(sf::VideoMode::getFullscreenModes())
{
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(50);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    window.clear();
    window.display();

    // List video modes
    //for (size_t i = 0; i < modes.size(); ++i)
    //{
        //sf::VideoMode mode = modes[i];
        //cout << "Mode #" << i << ": "
            //<< mode.width << "x" << mode.height << " - "
            //<< mode.bitsPerPixel << "bpp" << endl;
    //}
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
