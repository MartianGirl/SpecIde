#include "Screen.h"

using namespace sf;

Screen::Screen(size_t scale) :
    GraphicWindow(328 * scale, 264 * scale, "SpecIde"),
    done(false), reset(false),
    rewind(false), play(false),
    scale(scale),
    xSize(328), ySize(264)
{
    if (!scrTexture.create(static_cast<Uint32>(xSize), static_cast<Uint32>(ySize)))
        assert(false);
    scrTexture.setRepeated(false);
    scrSprite.setTexture(scrTexture);
    scrSprite.setScale(Vector2f(static_cast<float>(scale), static_cast<float>(scale)));
}

bool Screen::update()
{
    bool tick = false;

    static size_t xPos = 0;
    static size_t yPos = 0;

    static bool hSyncDelayed = false;
    static bool vSyncDelayed = false;

#if SPECIDE_BYTE_ORDER == 1
    static std::vector<sf::Uint32> pixels(xSize * ySize, 0xFF000000);
#else
    static std::vector<sf::Uint32> pixels(xSize * ySize, 0x000000FF);
#endif

    // If not blanking, draw.
    if (!(*hBlankInput || *vBlankInput))
    {
        if ((yPos > 23 && yPos < 288) && (xPos > 23 && xPos < 352))
            pixels[(yPos - 24) * xSize + (xPos - 24)] = *rgbaInput;
        ++xPos;
    }

    // Act on sync pulses falling edges:
    // VSYNC falling edge restores the beam to the top of the screen.
    if (!*vSyncInput && vSyncDelayed)
    {
        yPos = 0;

        scrTexture.update(reinterpret_cast<Uint8*>(&pixels[0]));
        window.clear(Color::Black);
        window.draw(scrSprite);
        window.display();
        tick = true;

        pollEvents();
    }
    vSyncDelayed = (*vSyncInput);

    // HSYNC falling edge restores the beam to the beginning of the next line.
    if ((*hSyncInput == false) && (hSyncDelayed == true))
    {
        xPos = 0;
        if (*vBlankInput == false)
            ++yPos;
    }
    hSyncDelayed = (*hSyncInput);

    return tick;
}

void Screen::pollEvents()
{
    // Poll events.
    Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case Event::Closed:
                done = true;
                break;

            case Event::KeyPressed:
            case Event::KeyReleased:
                if ((Keyboard::isKeyPressed(Keyboard::F10))) done = true;
                if ((Keyboard::isKeyPressed(Keyboard::F5))) reset = true;
                if ((Keyboard::isKeyPressed(Keyboard::F11))) play = true;
                if ((Keyboard::isKeyPressed(Keyboard::F12))) rewind = true;

                keyboardDataOut[0] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::B)) ? 0xEF : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::N)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Comma)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::M)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Period)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::LControl)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::RControl)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::LAlt)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Comma)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Period)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Quote)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Space)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Escape)) ? 0xFE : 0xFF);
                keyboardDataOut[1] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::H)) ? 0xEF : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::J)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::K)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::L)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Return)) ? 0xFE : 0xFF);
                keyboardDataOut[2] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::Y)) ? 0xEF : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::U)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::I)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::O)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::P)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Quote)) ? 0xFE : 0xFF);
                keyboardDataOut[3] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::Num6)) ? 0xEF : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Down)) ? 0xEF : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num7)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Up)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num8)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Right)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num9)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Insert)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num0)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::BackSpace)) ? 0xFE : 0xFF);
                keyboardDataOut[4] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::Num1)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Delete)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num2)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Tab)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num3)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::End)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num4)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Home)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Num5)) ? 0xEF : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Left)) ? 0xEF : 0xFF);
                keyboardDataOut[5] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::Q)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::W)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::E)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::R)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::T)) ? 0xEF : 0xFF);
                keyboardDataOut[6] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::A)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::S)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::D)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::F)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::G)) ? 0xEF : 0xFF);
                keyboardDataOut[7] = 0xFF &
                    ((Keyboard::isKeyPressed(Keyboard::LShift)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::RShift)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::BackSpace)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Escape)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Up)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Down)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Left)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Right)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::LAlt)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Insert)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::End)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Home)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Tab)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Delete)) ? 0xFE : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::Z)) ? 0xFD : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::X)) ? 0xFB : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::C)) ? 0xF7 : 0xFF) &
                    ((Keyboard::isKeyPressed(Keyboard::V)) ? 0xEF : 0xFF);
                break;

            default:
                break;
        }
    }
}

// vim: et:sw=4:ts=4
