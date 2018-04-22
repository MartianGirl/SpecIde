#include "Screen.h"

#include "SoundDefs.h"

#include "config.h"

#ifdef USE_BOOST_THREADS
#include <boost/chrono/include.hpp>
#include <boost/thread.hpp>
using namespace boost::this_thread;
using namespace boost::chrono;
#else
#include <chrono>
#include <thread>
using namespace std::this_thread;
using namespace std::chrono;
#endif

#include <cfenv>
#include <cmath>

using namespace std;
using namespace sf;

Screen::Screen(size_t scale, bool fullscreen) :
    GraphicWindow(344 * scale, 288 * scale, "SpecIde", fullscreen),
    skip(ULA_CLOCK_48 / SAMPLE_RATE),
    fullscreen(fullscreen), smooth(false),
    squareRootDac(true),
    scale(scale),
    xSize(352), ySize(304),
    stereo(0),
    pad(false),
    flashTap(false)
{
    // Create a texture. It'll be 352x304, which holds the entire Spectrum
    // screen.
    texture(xSize, ySize);

    // Load the ZX font, for the menu.
    vector<string> fontPaths;
    string fontName("ZXSpectrum.ttf");
    char* pHome = getenv(SPECIDE_HOME_ENV);

    fontPaths.push_back("");
    if (pHome != nullptr)
    {
#if (SPECIDE_ON_UNIX==1)
        string home(pHome);
        home += string("/") + string(SPECIDE_CONF_DIR) + string("/font/");
        fontPaths.push_back(home);

        string fontHome(pHome);
        fontHome += string("/.fonts/truetype/");
        fontPaths.push_back(fontHome);
#else
        string home(pHome);
        home += string("\\") + string(SPECIDE_CONF_DIR) + string("\\font\\");
        fontPaths.push_back(home);
#endif
    }
#if (SPECIDE_ON_UNIX==1)
    fontPaths.push_back("/usr/local/share/fonts/truetype/");
    fontPaths.push_back("/usr/share/fonts/truetype/");
#endif

    size_t j = 0;
    bool success = false;
    do
    {
        string font = fontPaths[j] + fontName;
        printf("Trying font: %s\n", font.c_str());
        success = zxFont.loadFromFile(font);
        ++j;
    } while (!success && j < fontPaths.size());

    if (!success)
    {
        printf("Could not load menu font.\n");
        assert(false);
    }

    setFullScreen(fullscreen);

    size_t vectorSize = xSize * (ySize + 8);    // Count blanking lines.
    spectrum.ula.xSize = xSize;
    spectrum.ula.ySize = ySize;
#if SPECIDE_BYTE_ORDER == 1
    spectrum.ula.pixels.assign(vectorSize, 0xFF000000);
#else
    spectrum.ula.pixels.assign(vectorSize, 0x000000FF);
#endif

    channel.open(2, SAMPLE_RATE);
}

void Screen::run()
{
    steady_clock::time_point tick = steady_clock::now();

    for (;;)
    {
        channel.play();
        for (;;)
        {
            // Now this chunk is for instant loading of TAPs.
            // Check tape trap
            if (flashTap == true && spectrum.rom48 && cpuInRefresh())
                checkTapeTraps();

            // Update Spectrum hardware.
            clock();

            if (spectrum.ula.vSync)
            {
                // Update the screen.
                update();
#ifdef USE_BOOST_THREADS
                sleep_until(tick + boost::chrono::milliseconds(20));
#else
                sleep_until(tick + std::chrono::milliseconds(20));
#endif
                tick = steady_clock::now();

                if (done) return;
                if (menu) break;
            }
        }
        channel.stop();

        for (;;)
        {
            // Menu thingy.
            updateMenu();
#ifdef USE_BOOST_THREADS
                sleep_until(tick + boost::chrono::milliseconds(20));
#else
                sleep_until(tick + std::chrono::milliseconds(20));
#endif
                tick = steady_clock::now();
            if (done) return;
            if (!menu) break;
        }
    }
}

void Screen::clock()
{
    static uint_fast32_t count = skip;
    static bool tapeTick = false;

    spectrum.clock();

    if (tape.playing)
    {
        tapeTick = !tapeTick;
        if (tapeTick == true && tape.sample-- == 0)
            spectrum.ula.tapeIn = tape.advance();
    }

    // Generate sound
    if (--count == 0)
    {
        count = skip;
        spectrum.buzzer.sample();
        spectrum.psg.sample();

        switch (stereo)
        {
            case 1: // ACB
                channel.push(
                        spectrum.buzzer.signal
                        + spectrum.psg.channelA + spectrum.psg.channelC,
                        spectrum.buzzer.signal
                        + spectrum.psg.channelB + spectrum.psg.channelC);
                break;

            case 2: // ABC
                channel.push(
                        spectrum.buzzer.signal
                        + spectrum.psg.channelA + spectrum.psg.channelB,
                        spectrum.buzzer.signal
                        + spectrum.psg.channelC + spectrum.psg.channelB);
                break;

            default:
                channel.push(
                        spectrum.buzzer.signal + spectrum.psg.channelA
                        + spectrum.psg.channelB + spectrum.psg.channelC,
                        spectrum.buzzer.signal + spectrum.psg.channelA
                        + spectrum.psg.channelB + spectrum.psg.channelC);
                break;
        }
    }
}

void Screen::update()
{
    // These conditions cannot happen at the same time:
    // - HSYNC and VSYNC only happen during the blanking interval.
    // - VSYNC happens at the end of blanking interval. (0x140)
    // - HSYNC happens at the beginning of HSYNC interval. (0x170-0x178)
    // If not blanking, draw.
    spectrum.ula.vSync = false;

    scrTexture.update(reinterpret_cast<Uint8*>(&spectrum.ula.pixels[0]));
    window.clear(Color::Black);
    window.draw(scrSprite);
    window.display();

    if (tape.pulseData.size())
    {
        char str[64];
        size_t percent = 100 * tape.pointer / tape.pulseData.size();
        snprintf(str, 64, "SpecIde [%03zu%%]", percent);
        window.setTitle(str);
    }

    pollEvents();

    tape.is48K = spectrum.set48;
}

void Screen::updateMenu()
{
    RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setFillColor(Color::White - Color(0, 0, 0, 64));
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(Color::Black);
    rectangle.setSize(Vector2f(296, 232));
    if (fullscreen)
    {
        rectangle.setPosition(xOffset + 24, yOffset + 24);
        rectangle.setScale(Vector2f(sScale, sScale));
    }
    else
    {
        rectangle.setPosition(24, 24);
        rectangle.setScale(Vector2f(scale, scale));
    }

    Text text;
    text.setFont(zxFont);
    text.setFillColor(Color::Black);
    text.setString("SpecIDE Menu");

    if (fullscreen)
    {
        text.setPosition(xOffset + 36, yOffset + 36);
        text.setCharacterSize(8 * sScale);
    }
    else
    {
        text.setPosition(36, 36);
        text.setCharacterSize(8 * scale);
    }

    window.clear(Color::Black);
    window.draw(scrSprite);
    window.draw(rectangle);
    window.draw(text);
    window.display();

    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::KeyPressed)
            if (event.key.code == Keyboard::F1)
                menu = false;
    }
}

void Screen::reopenWindow(bool fs)
{
    window.close();

    if (fs)
        window.create(bestMode, "SpecIDE", sf::Style::Fullscreen);
    else
        window.create(
                sf::VideoMode(static_cast<sf::Uint32>(w), static_cast<sf::Uint32>(h)),
                "SpecIDE", sf::Style::Close | sf::Style::Titlebar);
}

void Screen::setFullScreen(bool fs)
{
    if (fs)
    {
        // Use best mode available.
        fesetround(FE_TONEAREST);
        xScale = bestMode.width / static_cast<float>(xSize);
        yScale = nearbyintf(bestMode.height / static_cast<float>(suggestedScans));

        // Adjust depending on the vertical scale.
        sScale = yScale;
        xOffset = (bestMode.width - ((xSize - 8) * sScale)) / 2;
        yOffset = 0;

        printf("XScale %.3f YScale %.3f\n", xScale, yScale);
        printf("Using scale %.3f\n", sScale);

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
        scrSprite.setTexture(scrTexture);
        scrSprite.setTextureRect(sf::IntRect(0, 8, 344, 288));
        scrSprite.setPosition(0, 0);
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
                    case Keyboard::F1:
                        menu = true;
                        break;
                    case Keyboard::F2:
                        if (event.key.shift)
                        {
                            smooth = !smooth;
                            setSmooth(smooth);
                        }
                        else
                        {
                            fullscreen = !fullscreen;
                            reopenWindow(fullscreen);
                            setFullScreen(fullscreen);
                        }
                        break;
                    case Keyboard::F7:
                        if (event.key.shift)
                        {
                            tape.clearSaveData();
                        }
                        else
                        {
                            tape.appendLoadData();
                        }
                        break;
                    case Keyboard::F8:
                        if (event.key.shift)
                        {
                            tape.useSaveData = !tape.useSaveData;
                            tape.selectTapData();
                        }
                        else
                        {
                            tape.writeSaveData();
                        }
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
    printf("Skipping %lu samples.\n", skip);
}

bool Screen::cpuInRefresh()
{
    return (spectrum.z80.state == Z80State::ST_OCF_T4L_RFSH2);
}

void Screen::checkTapeTraps()
{
    if (spectrum.z80.pc.w == 0x56D) // LD_START
    {
        if (tape.tapData.size())
            trapLdStart();
    }

    if (spectrum.z80.pc.w == 0x4D1) // SA_FLAG
    {
        trapSaBytes();
    }
}

void Screen::writeMemory(uint_fast16_t a, uint_fast8_t d)
{
    a &= 0xFFFF;
    if (a > 0x3FFF) // Don't write ROM.
        spectrum.map[a >> 14][a & 0x3FFF] = d;
}

uint_fast8_t Screen::readMemory(uint_fast16_t a)
{
    a &= 0xFFFF;
    return spectrum.map[a >> 14][a & 0x3FFF];
}

void Screen::trapLdStart()
{
    // Find first block that matches flag byte (Flag is in AF')
    while (tape.foundTapBlock(spectrum.z80.af_.h) == false)
        tape.nextTapBlock();

    // Get parameters from CPU registers
    uint16_t start = spectrum.z80.ix.w;
    uint16_t bytes = spectrum.z80.de.w;
    uint16_t block = tape.getBlockLength();

    if (block < bytes)
    {
        // Load error if not enough bytes.
        spectrum.z80.af.l &= 0xFE;
    }
    else
    {
        block = bytes;
        spectrum.z80.af.l |= 0x01;
    }
    spectrum.z80.ix.w += block; spectrum.z80.ix.w &= 0xFFFF;
    spectrum.z80.de.w -= block;

    // Dump block to memory.
    for (uint_fast16_t ii = 0; ii < block; ++ii)
        writeMemory(start + ii, tape.getBlockByte(ii + 3));

    // Advance tape
    tape.nextTapBlock();

    // Force RET
    spectrum.z80.decode(0xC9);
    spectrum.z80.startInstruction();

    if (tape.tapPointer == 0)
        tape.rewind();
}

void Screen::trapSaBytes()
{
    uint16_t start = spectrum.z80.ix.w;
    uint16_t bytes = spectrum.z80.de.w + 2;
    uint8_t checksum;

    tape.saveData.push_back(bytes & 0x00FF);
    tape.saveData.push_back((bytes & 0xFF00) >> 8);
    tape.saveData.push_back(spectrum.z80.af.h);
    bytes -= 2;

    spectrum.z80.ix.w += bytes; spectrum.z80.ix.w &= 0xFFFF;
    spectrum.z80.de.w -= bytes;

    checksum = spectrum.z80.af.h;
    for (uint_fast16_t ii = 0; ii < bytes; ++ii)
    {
        uint_fast8_t byte = readMemory(start + ii);
        tape.saveData.push_back(byte);
        checksum ^= byte;
    }
    
    tape.saveData.push_back(checksum);

    // Force RET
    spectrum.z80.decode(0xC9);
    spectrum.z80.startInstruction();
}

// vim: et:sw=4:ts=4
