#include "GraphicWindow.h"

using namespace std;

GraphicWindow::GraphicWindow(size_t x, size_t y, 
        std::string const& title,
        bool fullscreen) :
    w(x), h(y),
    window(
        fullscreen ?
        sf::VideoMode::getDesktopMode() :
        sf::VideoMode(static_cast<sf::Uint32>(x), static_cast<sf::Uint32>(y)),
        title,
        fullscreen ?
        sf::Style::Fullscreen :
        sf::Style::Close | sf::Style::Titlebar),
    modes(sf::VideoMode::getFullscreenModes()),
    bestMode(sf::VideoMode::getDesktopMode())
{
    init();

    cout << "Selecting Full Screen Mode: "
        << bestMode.width << "x" << bestMode.height << " - "
        << bestMode.bitsPerPixel << "bpp" << endl;

    adjust();
}

void GraphicWindow::init()
{
    // window.setFramerateLimit(50);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);
    window.clear();
    window.display();
}

void GraphicWindow::adjust()
{
    size_t divider = 0;
    do
    {
        ++divider;
        suggestedScans = bestMode.height / divider;
    } while (suggestedScans > 608); // 312 - 8 VBlank lines.
}

GraphicWindow::~GraphicWindow()
{
    window.close();
}

// vim: et:sw=4:ts=4
