#include "GraphicWindow.h"

#include <iostream>

GraphicWindow::GraphicWindow(size_t x, size_t y, std::string const& title) :
    window(sf::VideoMode(x, y), title)
{
    window.clear();
    window.display();
}

void GraphicWindow::open()
{
    window.setVisible(true);
}

void GraphicWindow::close()
{
    window.setVisible(false);
}

void GraphicWindow::update()
{
}

GraphicWindow::~GraphicWindow()
{
    window.close();
}

// vim: et:sw=4:ts=4
