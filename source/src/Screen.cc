#include "Screen.h"

#include <iostream>

using namespace std;
using namespace sf;

Screen::Screen(size_t scale) :
    GraphicWindow(328 * scale, 264 * scale, "SpecIde"),
    done(false), reset(false),
    rewind(false), play(false),
    scale(scale),
    xSize(328), ySize(264),
    keyboardMask{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
{
    if (!scrTexture.create(static_cast<Uint32>(xSize), static_cast<Uint32>(ySize)))
        assert(false);
    scrTexture.setRepeated(false);
    scrTexture.setSmooth(false);
    scrSprite.setTexture(scrTexture);
    scrSprite.setScale(Vector2f(static_cast<float>(scale), static_cast<float>(scale)));
    scrSprite.setPosition(0, 0);

    window.setJoystickThreshold(0.5);
}

bool Screen::update()
{
    bool tick = false;

    static size_t xPos = 0;
    static size_t yPos = 0;

    static bool hSyncDelayed = false;
    static bool vSyncDelayed = false;

#if SPECIDE_BYTE_ORDER == 1
    static vector<sf::Uint32> pixels(xSize * ySize, 0xFF000000);
#else
    static vector<sf::Uint32> pixels(xSize * ySize, 0x000000FF);
#endif

    // If not blanking, draw.
    if (!(*hBlankInput || *vBlankInput))
    {
        if ((yPos > 23 && yPos < 288) && (xPos > 15 && xPos < 344))
            pixels[(yPos - 24) * xSize + (xPos - 16)] = *rgbaInput;
        ++xPos;
    }

    // Act on sync pulses falling edges:
    // VSYNC falling edge restores the beam to the top of the screen.
    if (!*vSyncInput && vSyncDelayed)
    {
        yPos = 0;

        scrTexture.update(reinterpret_cast<Uint8*>(&pixels[0]));
        // window.clear(Color::Black);
        window.draw(scrSprite);
        window.display();
        tick = true;

        pollEvents();
    }
    vSyncDelayed = *vSyncInput;

    // HSYNC falling edge restores the beam to the beginning of the next line.
    if (!*hSyncInput && hSyncDelayed)
    {
        xPos = 0;
        if (*vBlankInput == false)
            ++yPos;
    }
    hSyncDelayed = *hSyncInput;

    return tick;
}

void Screen::setFullScreen(bool fs)
{
    if (fs)
    {
        sf::VideoMode mode = modes[0];  // Best mode
        window.create(mode, "SpecIDE", sf::Style::Fullscreen);
        float xScale = mode.width / static_cast<float>(xSize);
        float yScale = mode.height / static_cast<float>(ySize);
        float sScale;
        if (xScale < yScale)
        {
            sScale = xScale;
            xOffset = 0;
            yOffset = (mode.height - (ySize * sScale)) / 2;
        }
        else
        {
            sScale = yScale;
            xOffset = (mode.width - (xSize * sScale)) / 2;
            yOffset = 0;
        }
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(sScale, sScale));
    }
    else
    {
        window.create(
                sf::VideoMode(static_cast<sf::Uint32>(w), static_cast<sf::Uint32>(h)),
                "SpecIDE", sf::Style::Close | sf::Style::Titlebar);
        xOffset = yOffset = 0;
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(static_cast<float>(scale), static_cast<float>(scale)));
    }

    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);
    window.setJoystickThreshold(0.5);
}

void Screen::setSmooth(bool sm)
{
    scrTexture.setSmooth(sm);
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
                switch (event.key.code)
                {
                    // Scan function keys
                    case Keyboard::F2:
                        if (event.key.shift)
                            smooth = true;
                        else
                            fullscreen = true;
                        break;
                    case Keyboard::F5:
                        reset = true;
                        break;
                    case Keyboard::F10:
                        done = true;
                        break;
                    case Keyboard::F11:
                        if (event.key.shift)
                            resetCounter = true;
                        else
                            play = true;
                        break;
                    case Keyboard::F12:
                        if (event.key.shift)
                            rewindToCounter = true;
                        else
                            rewind = true;
                        break;

                        // Scan Spectrum keyboard
                    default:
                        scanKeys(event);
                        keyboardDataOut[0] &= keyboardMask[0];
                        keyboardDataOut[1] &= keyboardMask[1];
                        keyboardDataOut[2] &= keyboardMask[2];
                        keyboardDataOut[3] &= keyboardMask[3];
                        keyboardDataOut[4] &= keyboardMask[4];
                        keyboardDataOut[5] &= keyboardMask[5];
                        keyboardDataOut[6] &= keyboardMask[6];
                        keyboardDataOut[7] &= keyboardMask[7];
                        break;
                }
                break;

            case Event::KeyReleased:
                scanKeys(event);
                keyboardDataOut[0] |= ~keyboardMask[0];
                keyboardDataOut[1] |= ~keyboardMask[1];
                keyboardDataOut[2] |= ~keyboardMask[2];
                keyboardDataOut[3] |= ~keyboardMask[3];
                keyboardDataOut[4] |= ~keyboardMask[4];
                keyboardDataOut[5] |= ~keyboardMask[5];
                keyboardDataOut[6] |= ~keyboardMask[6];
                keyboardDataOut[7] |= ~keyboardMask[7];
                break;

            case Event::JoystickMoved:
                switch (event.joystickMove.axis)
                {
                    case Joystick::X:
                    case Joystick::U:
                    case Joystick::PovX:
                        *joystickDataOut &= 0xFC;
                        if (event.joystickMove.position < -34.0)
                            *joystickDataOut |= 0x02;
                        else if (event.joystickMove.position > 34.0)
                            *joystickDataOut |= 0x01;
                        break;

                    case Joystick::Y:
                    case Joystick::V:
                    case Joystick::PovY:
                        *joystickDataOut &= 0xF3;
                        if (event.joystickMove.position < -34.0)
                            *joystickDataOut |= 0x08;
                        else if (event.joystickMove.position > 34.0)
                            *joystickDataOut |= 0x04;
                        break;

                    default:
                        break;
                }
                break;

            case Event::JoystickButtonPressed:
                *joystickDataOut |= 0x10;
                break;

            case Event::JoystickButtonReleased:
                *joystickDataOut &= 0xEF;
                break;

            default:
                break;
        }
    }
}

void Screen::scanKeys(Event const& event)
{
    keyboardMask[0] = 0xFF;
    keyboardMask[1] = 0xFF;
    keyboardMask[2] = 0xFF;
    keyboardMask[3] = 0xFF;
    keyboardMask[4] = 0xFF;
    keyboardMask[5] = 0xFF;
    keyboardMask[6] = 0xFF;
    keyboardMask[7] = 0xFF;

    switch (event.key.code)
    {
        case Keyboard::B:
            keyboardMask[0] = 0xEF;
            break;
        case Keyboard::N:
            keyboardMask[0] = 0xF7;
            break;
        case Keyboard::M:
            keyboardMask[0] = 0xFB;
            break;
        case Keyboard::LControl:    // Symbol Shift
        case Keyboard::RControl:
            keyboardMask[0] = 0xFD;
            break;
        case Keyboard::Space:
            keyboardMask[0] = 0xFE;
            break;

        case Keyboard::Comma:   // Symbol Shift + N
            keyboardMask[0] = 0xF5;
            break;
        case Keyboard::Period:  // Symbol Shift + M
            keyboardMask[0] = 0xF9;
            break;
        case Keyboard::LAlt:    // Extend Mode = Caps Shift + Symbol Shift
            keyboardMask[0] = 0xFD; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Escape:  // Break = Caps Shift + Space
            keyboardMask[0] = 0xFE; keyboardMask[7] = 0xFE;
            break;

        case Keyboard::H:
            keyboardMask[1] = 0xEF;
            break;
        case Keyboard::J:
            keyboardMask[1] = 0xF7;
            break;
        case Keyboard::K:
            keyboardMask[1] = 0xFB;
            break;
        case Keyboard::L:
            keyboardMask[1] = 0xFD;
            break;
        case Keyboard::Return:
            keyboardMask[1] = 0xFE;
            break;

        case Keyboard::Y:
            keyboardMask[2] = 0xEF;
            break;
        case Keyboard::U:
            keyboardMask[2] = 0xF7;
            break;
        case Keyboard::I:
            keyboardMask[2] = 0xFB;
            break;
        case Keyboard::O:
            keyboardMask[2] = 0xFD;
            break;
        case Keyboard::P:
            keyboardMask[2] = 0xFE;
            break;

        case Keyboard::Quote:   // Symbol Shift + P
            keyboardMask[0] = 0xFD; keyboardMask[2] = 0xFE;
            break;

        case Keyboard::Num6:
            keyboardMask[3] = 0xEF;
            break;
        case Keyboard::Num7:
            keyboardMask[3] = 0xF7;
            break;
        case Keyboard::Num8:
            keyboardMask[3] = 0xFB;
            break;
        case Keyboard::Num9:
            keyboardMask[3] = 0xFD;
            break;
        case Keyboard::Num0:
            keyboardMask[3] = 0xFE;
            break;

        case Keyboard::Down:        // Caps Shift + 6
            keyboardMask[3] = 0xEF; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Up:          // Caps Shift + 7
            keyboardMask[3] = 0xF7; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Right:       // Caps Shift + 8
            keyboardMask[3] = 0xFB; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Insert:      // Graph Mode: Caps Shift + 9
            keyboardMask[3] = 0xFD; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::BackSpace:   // Delete: Caps Shift + 0
            keyboardMask[3] = 0xFE; keyboardMask[7] = 0xFE;
            break;

        case Keyboard::Num5:
            keyboardMask[4] = 0xEF;
            break;
        case Keyboard::Num4:
            keyboardMask[4] = 0xF7;
            break;
        case Keyboard::Num3:
            keyboardMask[4] = 0xFB;
            break;
        case Keyboard::Num2:
            keyboardMask[4] = 0xFD;
            break;
        case Keyboard::Num1:
            keyboardMask[4] = 0xFE;
            break;

        case Keyboard::Left:        // Caps Shift + 5
            keyboardMask[4] = 0xEF; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Home:        // Inv Video: Caps Shift + 4
            keyboardMask[4] = 0xF7; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::End:         // True Video: Caps Shift + 3
            keyboardMask[4] = 0xFB; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Tab:      // Caps Lock: Caps Shift + 2
            keyboardMask[4] = 0xFD; keyboardMask[7] = 0xFE;
            break;
        case Keyboard::Delete:   // Edit: Caps Shift + 1
            keyboardMask[4] = 0xFE; keyboardMask[7] = 0xFE;
            break;

        case Keyboard::T:
            keyboardMask[5] = 0xEF;
            break;
        case Keyboard::R:
            keyboardMask[5] = 0xF7;
            break;
        case Keyboard::E:
            keyboardMask[5] = 0xFB;
            break;
        case Keyboard::W:
            keyboardMask[5] = 0xFD;
            break;
        case Keyboard::Q:
            keyboardMask[5] = 0xFE;
            break;

        case Keyboard::G:
            keyboardMask[6] = 0xEF;
            break;
        case Keyboard::F:
            keyboardMask[6] = 0xF7;
            break;
        case Keyboard::D:
            keyboardMask[6] = 0xFB;
            break;
        case Keyboard::S:
            keyboardMask[6] = 0xFD;
            break;
        case Keyboard::A:
            keyboardMask[6] = 0xFE;
            break;

        case Keyboard::V:
            keyboardMask[7] = 0xEF;
            break;
        case Keyboard::C:
            keyboardMask[7] = 0xF7;
            break;
        case Keyboard::X:
            keyboardMask[7] = 0xFB;
            break;
        case Keyboard::Z:
            keyboardMask[7] = 0xFD;
            break;
        case Keyboard::LShift:
        case Keyboard::RShift:
            keyboardMask[7] = 0xFE;
            break;

        default:
            break;
    }
}

// vim: et:sw=4:ts=4
