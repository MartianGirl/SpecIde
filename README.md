# SpecIde

## What is it?
SpecIde is (yet another) ZX Spectrum emulator.  Currently, the emulator is functional;
actually, it is *very* accurate. Some of the supported features are:

- Emulation of ZX Spectrum 48K (Issue 2/3), 128K, +2, +2A and +3.
- +3 disk drive emulation. (Read-only yet)
- Emulation of Spanish 128K, +2, +2A and +3.
- PSG (AY-3-8912/YM-2149) sound emulation.
- Turbosound emulation. Supports two and four PSG modes.
- Loading of tapes via .tap and .tzx tape images.
- Loading of disks via .dsk disk images.
- Flashloading of .tap files and .tzx that use the ROM routines.
- Saving to .tap files.
- Full screen video mode detection.
- Double scan interlaced modes. (Gigascreen modes)
- Kempston/Sinclair joystick emulation from the PC joystick/gamepad.
- Works in GNU/Linux, Windows, and MacOS.

## How to get it?
These are SpecIde binaries for Windows:

- [SpecIde for Windows 10 (32-bit)](https://drive.google.com/open?id=1C5zRcY8itrvP9hKJsIniFWSGP1Zs-3NC)
- [SpecIde for Windows 10 (64-bit)](https://drive.google.com/open?id=1X-eZrQ7BfX1SP4DVdLrDT94rwOLgmoQX)
- [SpecIde for older Windows (XP/Vista/7) (32-bit)](https://drive.google.com/open?id=1jta-_Wdl9C7p0AbkiEEBOUJsI95JPsbc)

For GNU/Linux and MacOS I'm not providing binaries, but SpecIde can be compiled quite easily.

## How to compile it?
### Compiling for GNU/Linux:
1. Install libboost. At least chrono, system, thread and unit_test_framework are required.
1. Install libsfml. Audio, graphics, window and system components are required.
1. Install cmake.
1. Clone the repository: `git clone https://github.com/MartianGirl/SpecIde.git`
1. Go into the 'source' directory.
1. Run: `cmake -DCMAKE_BUILD_TYPE=Release .`
1. Run: `make clean && make install`
1. The binaries will be installed in 'source/bin'. The test binaries will be installed in 'source/bin/tst'
1. Copy the roms from the spectrum-roms package (or find them online) to the $HOME/.SpecIde/roms directory.
1. Download [this font](https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf "ZX Spectrum font by JFSebastian") to the $HOME/.SpecIde/font directory.
1. Run: bin/SpecIde [options] \<TZXFile.tzx|TAPFile.tap|DSKFile.dsk\>

### Compiling for MacOS
1. Install brew: `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
1. Install cmake: `brew install cmake`
1. Install boost: `brew install boost`
1. Install sfml: `brew install sfml`
1. Install pkgconfig: `brew install pkgconfig`
1. Simlink FindSFML.cmake in cmake modules `ln -s $(brew --prefix sfml)/share/SFML/cmake/Modules/FindSFML.cmake $(brew --prefix cmake)/share/cmake/Modules/FindSFML.cmake`
1. Clone the repository: `git clone https://github.com/MartianGirl/SpecIde.git`
1. Go into the 'source' directory.
1. Run: `cmake -DCMAKE_BUILD_TYPE=Release .`
1. Build it!: `make clean && make install`
1. Copy the roms from the spectrum-roms package (or find them online) to the $HOME/Library/Application Support/SpecIde/roms directory.
1. Download [this font](https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf "ZX Spectrum font by JFSebastian") to the $HOME/Library/Application Support/SpecIde/font directory.
1. Run: bin/SpecIde [options] \<TZXFile.tzx|TAPFile.tap|DSKFile.dsk\>

### Compiling for Windows
I've successfully compiled SpecIde with MinGW32 and Visual Studio 2015 & 2017.  
I've included a script RunCMake.bat that helps in the build process.

1. Install and compile boost. Add the binaries to the PATH.
1. Install and compile sfml. Add the binaries to the PATH.
1. Install cmake.
1. (Optional) Install ninja-builds. It really helps building SpecIde.
1. Edit the RunCMake.bat script. You need to change the lines: `set BOOST_ROOT=\<Path_to_Boost_root_directory\>` and `set SFML_ROOT=\<Path_to_SFML_binaries\>`  
1. Run: `RunCMake \[GNU|MS|NINJAGNU|NINJAMS\] RELEASE`
1. Build it!: `ninja clean & ninja install` or `mingw32-make clean & mingw32-make install` (or use Visual Studio IDE)
1. Copy the roms from the spectrum-roms package (or find them online) to the %APPDATA%/SpecIde/roms directory.
1. Download [this font](https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf "ZX Spectrum font by JFSebastian") to the %APPDATA%/SpecIde/font directory.
1. Run it!: bin\SpecIde [options] \<TZXFile.tzx|TAPFile.tap|DSKFile.dsk\>

## How to use it?
SpecIde is invoked from the command line. To run SpecIde, type:
`SpecIde [options] [tapefiles|diskfiles]`

Supported formats are TAP, TZX and DSK.

Make sure that the libraries, ROM files and 

### Command line options
The following command line options are available:

```
Model selection options:
--issue2               Spectrum 48K, issue 2.
--issue3 | --48        Spectrum 48K, issue 3. (Default)
--128                  Spectrum 128K.
--128sp                Spectrum 128K. (Spanish ROM)
--plus2                Spectrum +2.
--plus2sp              Spectrum +2. (Spanish ROM)
--plus2a               Spectrum +2A.
--plus2asp             Spectrum +2A. (Spanish ROM)
--plus3                Spectrum +3.
--plus3sp              Spectrum +3. (Spanish ROM)

Joystick options:
--kempston             Map joystick to Kempston interface.
--sinclair             Map joystick to Sinclair interface. (Default)
--pad|--nopad          Map pad extra buttons to keys.

PSG options:
--psg|--nopsg          Emulate AY chip in 48K Spectrum.
--abc|--acb|--mono     Select stereo mode.
--ay|--ym              Select PSG: AY-3-8912/YM-2149.
--turbo                Select TurboSound with 2 PSGs. (mono)
--turboacb|--turboabc  Select TurboSound with 2 PSGs. (stereo ACB/ABC)
--turbonext            Select Next-style TurboSound with 4 PSGs.

Misc hardware options:
--sd1                  Emulate Dinamic SD1 hardware protection.

Video options:
--fullscreen           Start SpecIde in full screen mode.
--window               Start SpecIde in windowed mode.
--scanlines            Render PAL double scan mode.
--average              Render PAL double scan mode, averaging scanlines.
--nodoublescan         Single scan mode. (Default)
--sync                 Sync emulation to PC video refresh rate.

Sound options (add prefix 'no' to disable. Eg. --nosound):
--sound                Enable buzzer/PSG sound. (Default)
--tapesound            Enable tape sound.

Emulation options (add prefix 'no' to disable. Eg. --noflashtap):
--flashtap         Enable ROM traps for LOAD and SAVE.
```

### Function keys
When the emulator is running, pressing F1 displays help about the function keys.

| Key | Function |
| --- | -------- |
| F1        | Display help. |
| F2        | Switch between fullscreen and windowed mode. |
| Shift-F2  | Toggle antialiasing on/off. |
| F4        | Switch PSG between AY-3-8910 and YM-2149. |
| F5        | Reset the Spectrum. |
| F7        | Add FlashTAP to SAVE buffer. |
| Shift-F7  | Clear SAVE buffer. |
| F8        | Write SAVE buffer to a file named savetapeXX.tap. |
| Shift-F8  | Use SAVE buffer as LOAD buffer. |
| F9        | Turn sound on/off. |
| Shift-F9  | Turn tape sounds on/off. |
| F10       | Exit the emulator. |
| F11       | Play/Stop the tape. |
| Shift-F11 | Remember tape position. (Reset tape counter to zero). |
| F12       | Rewind tape to start. |
| Shift-F12 | Rewind tape to saved position. (Rewind to tape counter zero). |


### The config directories
Finally, SpecIde looks for a configuration file (SpecIde.cfg) in the following places:
1. Current directory: ./SpecIde.cfg
2. On GNU/Linux: $HOME/.SpecIde/SpecIde.cfg
3. On MacOS: $HOME/Library/Application Support/SpecIde/SpecIde.cfg
4. On Windows: %APPDATA%\SpecIde\SpecIde.cfg

A template SpecIde.cfg.template is provided in the source code.

System ROMs can be placed also in the config directory:
- On GNU/Linux: $HOME/.SpecIde/roms
- On MacOS: $HOME/Library/Application Support/SpecIde/roms
- On Windows: %APPDATA%\SpecIde\roms

The following ROMs are included in the binary packages:

| File Name | Model |
|:---:|:---:|
| 48.rom | Spectrum 48K |
| 128-0.rom | Spectrum 128K (ROM 0) |
| 128-1.rom | Spectrum 128K (ROM 1) |
| plus2-0.rom | Spectrum +2 (ROM 0) |
| plus2-1.rom | Spectrum +2 (ROM 1) |
| plus3-0.rom | Spectrum +2A/+3 (ROM 0) |
| plus3-1.rom | Spectrum +2A/+3 (ROM 1) |
| plus3-2.rom | Spectrum +2A/+3 (ROM 2) |
| plus3-3.rom | Spectrum +2A/+3 (ROM 3) |
| 128-spanish-0.rom | Spanish Spectrum 128K (ROM 0) |
| 128-spanish-1.rom | Spanish Spectrum 128K (ROM 1) |
| plus2-spanish-0.rom | Spanish Spectrum +2 (ROM 0) |
| plus2-spanish-1.rom | Spanish Spectrum +2 (ROM 1) |
| plus3-spanish-0.rom | Spanish Spectrum +2A/+3 (ROM 0) |
| plus3-spanish-1.rom | Spanish Spectrum +2A/+3 (ROM 1) |
| plus3-spanish-2.rom | Spanish Spectrum +2A/+3 (ROM 2) |
| plus3-spanish-3.rom | Spanish Spectrum +2A/+3 (ROM 3) |

Also, the font can be placed in here:
- On GNU/Linux: $HOME/.SpecIde/font
- On MacOS: $HOME/Library/Application Support/SpecIde/font
- On Windows: %APPDATA%\SpecIde\font

## And why?
This is an attempt at writing a ZX Spectrum emulator using SFML for video, audio and user interface. The goals I set for myself were:
- Accuracy. The emulator should work as much as possible like a real Spectrum. It should be easy to start in full screen mode, without having to select a video mode or care about difficult settings.
- Simplicity. The UI should be as simple as possible, and the emulator should operate only with the function keys as much as possible.
- Reliability. The emulator should never crash.
- Portability. The emulator should be easy to run in different platforms.
I've been writing SpecIde mostly for learning and for trying to do a big project. So far, I am happy with the outcome!

## Credits and acknowledgements:  
David Garijo: For taking the time and helping with the MacOS build process.
JFSebastian: For his wonderful ZXSpectrum.ttf font.
Ast_A_Moore: For all his help with the +2A timings and port 0x0FFD.
César Hernández Bañó: For his help with the initial values for IR register, his comments, and his own emulator ZesarUX.
Miguel Mesa: For pointing out that the FLASH attribute was running at half speed.
