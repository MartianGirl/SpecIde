#include "Screen.h"

#include "SoundDefs.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace sf;

Screen::Screen(size_t scale, bool fullscreen) :
    GraphicWindow(344 * scale, 288 * scale, "SpecIde", fullscreen),
    skip(ULA_CLOCK_48 / SAMPLE_RATE),
    done(false),
    fullscreen(fullscreen), smooth(false),
    squareRootDac(true),
    scale(scale),
    xSize(352), ySize(304),
    stereo(0),
    pad(false)
{
    // Create a texture. It'll be 352x304, which holds the entire Spectrum
    // screen.
    texture(xSize, ySize);

    if (fullscreen)
    {
        // Use best mode available.
        float xScale = bestMode.width / static_cast<float>(xSize);
        float yScale = bestMode.height / static_cast<float>(suggestedScans);
        float sScale;

        // Adjust depending on the vertical scale.
        sScale = yScale;
        xOffset = (bestMode.width - ((xSize - 8) * sScale)) / 2;
        yOffset = 0;

        cout << "XScale " << xScale << " YScale " << yScale << endl;
        cout << "Using scale " << sScale << endl;

        size_t start = (312 - suggestedScans) / 2;
        size_t lines = bestMode.height;

        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(0, static_cast<uint_fast32_t>(start),
                    static_cast<uint_fast32_t>(xSize - 8), static_cast<uint_fast32_t>(lines)));
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(sScale, sScale));
    }
    else
    {
        // We select the windowed mode by default.
        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(0, 8, 344, 288));
        scrSprite.setScale(Vector2f(static_cast<float>(scale), static_cast<float>(scale)));
        scrSprite.setPosition(0, 0);
    }

    window.setJoystickThreshold(0.5);

    size_t vectorSize = xSize * (ySize + 8);    // Count blanking lines.
#if SPECIDE_BYTE_ORDER == 1
    pixels.assign(vectorSize, 0xFF000000);
#else
    pixels.assign(vectorSize, 0x000000FF);
#endif

    channel.open(2, SAMPLE_RATE);
    channel.play();
}

void Screen::clock()
{
    static size_t count = 0;

    ++count;

    spectrum.clock();

    if (tape.playing && (count & 0x01))
    {
        if (tape.sample-- == 0)
            spectrum.ula.tapeIn = tape.advance();
    }

    // Generate sound
    if ((count % skip) == 0)
    {
        spectrum.buzzer.sample();
        spectrum.psg.sample();

        switch (stereo)
        {
            case 1: // ACB
                samples[0] = spectrum.buzzer.signal
                    + spectrum.psg.channelA + spectrum.psg.channelC;
                samples[1] = spectrum.buzzer.signal
                    + spectrum.psg.channelB + spectrum.psg.channelC;
                break;

            case 2: // ABC
                samples[0] = spectrum.buzzer.signal
                    + spectrum.psg.channelA + spectrum.psg.channelB;
                samples[1] = spectrum.buzzer.signal
                    + spectrum.psg.channelC + spectrum.psg.channelB;
                break;

            default:
                samples[0] = samples[1] = spectrum.buzzer.signal + spectrum.psg.channelA
                    + spectrum.psg.channelB + spectrum.psg.channelC;
                break;
        }

        channel.push(samples);
    }
}

bool Screen::update()
{
    bool tick = false;

    static size_t xPos = 0;
    static size_t yPos = 0;

    // If not blanking, draw.
    if (spectrum.ula.blanking == false)
    {
        pixels[(yPos * xSize) + xPos] = spectrum.ula.rgba;
        ++xPos;
    }

    // Act on sync pulses falling edges:
    // VSYNC falling edge restores the beam to the top of the screen.
    if (spectrum.ula.vSyncEdge)
    {
        yPos = 0;

        scrTexture.update(reinterpret_cast<Uint8*>(&pixels[0]));
        window.clear(Color::Black);
        window.draw(scrSprite);
        window.display();

        if (tape.pulseData.size())
        {
            stringstream ss;
            double percent = 100.0 * tape.pointer / tape.pulseData.size();
            ss << "SpecIde [" << setfill('0') << setw(3);
            ss << static_cast<size_t>(percent) << "%]" ; 
            window.setTitle(ss.str());
        }

        tick = true;

        pollEvents();

        tape.is48K = (spectrum.paging & 0x20) ? true : false;
    }

    // HSYNC falling edge restores the beam to the beginning of the next line.
    if (spectrum.ula.hSyncEdge)
    {
        xPos = 0;
        if (spectrum.ula.retrace == false)
            ++yPos;
    }

    return tick;
}

void Screen::setFullScreen(bool fs)
{
    window.close();
    if (fs)
    {
        // Use best mode available.
        window.create(bestMode, "SpecIDE", sf::Style::Fullscreen);
        float xScale = bestMode.width / static_cast<float>(xSize);
        float yScale = bestMode.height / static_cast<float>(suggestedScans);
        float sScale;

        // Adjust depending on the vertical scale.
        sScale = yScale;
        xOffset = (bestMode.width - ((xSize - 8) * sScale)) / 2;
        yOffset = 0;

        cout << "XScale " << xScale << " YScale " << yScale << endl;
        cout << "Using scale " << sScale << endl;

        size_t start = (312 - suggestedScans) / 2;
        size_t lines = bestMode.height;

        scrSprite.setTextureRect(sf::IntRect(0, static_cast<uint_fast32_t>(start),
                    static_cast<uint_fast32_t>(xSize - 8), static_cast<uint_fast32_t>(lines)));
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(sScale, sScale));
    }
    else
    {
        window.create(
                sf::VideoMode(static_cast<sf::Uint32>(w), static_cast<sf::Uint32>(h)),
                "SpecIDE", sf::Style::Close | sf::Style::Titlebar);
        xOffset = yOffset = 0;
        scrSprite.setTextureRect(sf::IntRect(0, 8, 344, 288));
        scrSprite.setPosition(xOffset, yOffset);
        scrSprite.setScale(Vector2f(static_cast<float>(scale), static_cast<float>(scale)));
    }

    // window.setFramerateLimit(50);
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
                channel.stop();
                done = true;
                break;

            case Event::KeyPressed:
                switch (event.key.code)
                {
                    // Scan function keys
                    case Keyboard::F2:
                        if (event.key.shift)
                        {
                            smooth = !smooth;
                            setSmooth(smooth);
                        }
                        else
                        {
                            fullscreen = !fullscreen;
                            setFullScreen(fullscreen);
                        }
                        break;
                    case Keyboard::F8:
                        squareRootDac = !squareRootDac;
                        spectrum.psg.setVolumeLevels(squareRootDac);
                        break;
                    case Keyboard::F9:
                        if (event.key.shift)
                        {
                            spectrum.buzzer.tapeSound = !spectrum.buzzer.tapeSound;
                        }
                        else
                        {
                            spectrum.buzzer.playSound = !spectrum.buzzer.playSound;
                            spectrum.psg.playSound = !spectrum.psg.playSound;
                        }
                        break;
                    case Keyboard::F5:
                        spectrum.reset();
                        break;
                    case Keyboard::F10:
                        done = true;
                        break;
                    case Keyboard::F11:
                        if (event.key.shift)
                            tape.resetCounter();
                        else
                            tape.play();
                        break;
                    case Keyboard::F12:
                        if (event.key.shift)
                            tape.rewind(tape.counter);
                        else
                            tape.rewind();
                        break;

                        // Scan Spectrum keyboard
                    default:
                        scanKeys(event);
                        spectrum.ula.keys[0] &= keyboardMask[0];
                        spectrum.ula.keys[1] &= keyboardMask[1];
                        spectrum.ula.keys[2] &= keyboardMask[2];
                        spectrum.ula.keys[3] &= keyboardMask[3];
                        spectrum.ula.keys[4] &= keyboardMask[4];
                        spectrum.ula.keys[5] &= keyboardMask[5];
                        spectrum.ula.keys[6] &= keyboardMask[6];
                        spectrum.ula.keys[7] &= keyboardMask[7];
                        break;
                }
                break;

            case Event::KeyReleased:
                scanKeys(event);
                spectrum.ula.keys[0] |= ~keyboardMask[0];
                spectrum.ula.keys[1] |= ~keyboardMask[1];
                spectrum.ula.keys[2] |= ~keyboardMask[2];
                spectrum.ula.keys[3] |= ~keyboardMask[3];
                spectrum.ula.keys[4] |= ~keyboardMask[4];
                spectrum.ula.keys[5] |= ~keyboardMask[5];
                spectrum.ula.keys[6] |= ~keyboardMask[6];
                spectrum.ula.keys[7] |= ~keyboardMask[7];
                break;

            case Event::JoystickMoved:
                switch (event.joystickMove.axis)
                {
                    case Joystick::X:
                    case Joystick::U:
                    case Joystick::PovX:
                        if (spectrum.kempston)
                        {
                            spectrum.joystick &= 0xFC;
                            if (event.joystickMove.position < -34.0)
                                spectrum.joystick |= 0x02;
                            else if (event.joystickMove.position > 34.0)
                                spectrum.joystick |= 0x01;
                        }
                        else
                        {
                            spectrum.ula.keys[3] |= 0x018;
                            if (event.joystickMove.position < -34.0)
                                spectrum.ula.keys[3] &= 0xEF;
                            else if (event.joystickMove.position > 34.0)
                                spectrum.ula.keys[3] &= 0xF7;
                        }
                        break;

                    case Joystick::Y:
                    case Joystick::V:
                    case Joystick::PovY:
                        if (spectrum.kempston)
                        {
                            spectrum.joystick &= 0xF3;
                            if (event.joystickMove.position < -34.0)
                                spectrum.joystick |= 0x08;
                            else if (event.joystickMove.position > 34.0)
                                spectrum.joystick |= 0x04;
                        }
                        else
                        {
                            spectrum.ula.keys[3] |= 0x06;
                            if (event.joystickMove.position < -34.0)
                                spectrum.ula.keys[3] &= 0xFD;
                            else if (event.joystickMove.position > 34.0)
                                spectrum.ula.keys[3] &= 0xFB;
                        }
                        break;

                    default:
                        break;
                }
                break;

            case Event::JoystickButtonPressed:
                if (pad)
                {
                    switch (event.joystickButton.button)
                    {
                        case 1: // Emulate press 'B'
                            spectrum.ula.keys[0] &= 0xEF; break;
                        case 2: // Emulate press 'N'
                            spectrum.ula.keys[0] &= 0xF7; break;
                        case 3: // Emulate press 'V'
                            spectrum.ula.keys[7] &= 0xEF; break;
                        case 4: // Emulate press 'C'
                            spectrum.ula.keys[7] &= 0xF7; break;
                        case 5: // Emulate press 'X'
                            spectrum.ula.keys[7] &= 0xFB; break;
                        case 6: // Emulate press 'G'
                            spectrum.ula.keys[6] &= 0xEF; break;
                        case 7: // Emulate press 'H'
                            spectrum.ula.keys[1] &= 0xEF; break;
                        default:    // Other buttons map to the joystick.
                            if (spectrum.kempston)
                                spectrum.joystick |= 0x10;
                            else
                                spectrum.ula.keys[3] &= 0xFE;   // '0'
                            break;
                    }

                }
                else
                {
                    if (spectrum.kempston)
                        spectrum.joystick |= 0x10;
                    else
                        spectrum.ula.keys[3] &= 0xFE;
                }
                break;

            case Event::JoystickButtonReleased:
                if (pad)
                {
                    switch (event.joystickButton.button)
                    {
                        case 1: // Emulate release 'B'
                            spectrum.ula.keys[0] |= 0x10; break;
                        case 2: // Emulate release 'N'
                            spectrum.ula.keys[0] |= 0x08; break;
                        case 3: // Emulate release 'V'
                            spectrum.ula.keys[7] |= 0x10; break;
                        case 4: // Emulate release 'C'
                            spectrum.ula.keys[7] |= 0x08; break;
                        case 5: // Emulate release 'X'
                            spectrum.ula.keys[7] |= 0x04; break;
                        case 6: // Emulate release 'G'
                            spectrum.ula.keys[6] |= 0x10; break;
                        case 7: // Emulate release 'H'
                            spectrum.ula.keys[1] |= 0x10; break;
                        default:    // Other buttons map to the joystick.
                            if (spectrum.kempston)
                                spectrum.joystick &= 0xEF;
                            else
                                spectrum.ula.keys[3] |= 0x01;
                            break;
                    }
                }
                else
                {
                    if (spectrum.kempston)
                        spectrum.joystick &= 0xEF;
                    else
                        spectrum.ula.keys[3] |= 0x01;
                }
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
            // case Keyboard::LAlt:    // Extend Mode = Caps Shift + Symbol Shift
            // keyboardMask[0] = 0xFD; keyboardMask[7] = 0xFE;
            // break;
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
            // case Keyboard::Home:        // Inv Video: Caps Shift + 4
            // keyboardMask[4] = 0xF7; keyboardMask[7] = 0xFE;
            // break;
            // case Keyboard::End:         // True Video: Caps Shift + 3
            // keyboardMask[4] = 0xFB; keyboardMask[7] = 0xFE;
            // break;
            // case Keyboard::Tab:      // Caps Lock: Caps Shift + 2
            // keyboardMask[4] = 0xFD; keyboardMask[7] = 0xFE;
            // break;
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

void Screen::texture(size_t x, size_t y)
{
    if (!scrTexture.create(static_cast<Uint32>(x), static_cast<Uint32>(y)))
        assert(false);
    scrTexture.setRepeated(false);
    scrTexture.setSmooth(false);
}

void Screen::set128K(bool is128K)
{
    skip = ((is128K) ? ULA_CLOCK_128 : ULA_CLOCK_48) / SAMPLE_RATE + 1;
    cout << "Skipping " << skip << " samples." << endl;
}

// vim: et:sw=4:ts=4
