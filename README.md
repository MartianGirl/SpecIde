# SpecIde

[![](https://github.com/MartianGirl/SpecIde/actions/workflows/build.yml/badge.svg)](https://github.com/MartianGirl/SpecIde/actions/workflows/build.yml)

## What is it?
SpecIde is (yet another) ZX Spectrum (and, partially, Amstrad CPC) emulator.
Currently, the ZX Spectrum emulation is quite accurate. The Amstrad CPC emulation
is still a work in progress, but it is mostly enough for playing most games.
Some of the supported features are:

- Emulation of ZX Spectrum 48K (Issue 2/3), 128K, +2, +2A and +3.
- Emulation of Amstrad CPC 464/664/6128. No support for Plus models yet!
- FDC765 disk drive emulation. (Scan commands are missing yet)
- Emulation of Spanish 128K, +2, +2A and +3.
- Emulation of Pentagon timings (but no BetaDisk yet! Sorry for that!)
- PSG (AY-3-8912/YM-2149) sound emulation.
- Turbosound emulation. Supports two and four PSG modes.
- Covox/Soundrive emulation.
- Loading of tapes via .tap and .tzx tape images, and .csw files.
- Loading of disks via .dsk disk images.
- Flashloading of .tap files and .tzx that use the ROM routines.
- Flashsaving to .tap files using the ROM routines.
- Full screen video mode detection.
- Double scan interlaced modes. (Gigascreen modes)
- Kempston/Sinclair/Cursor/Fuller joystick emulation from the PC joystick/gamepad.
- Fuller box audio.
- Works in GNU/Linux, Windows, and MacOS.

## How to get it?
You can check the Releases page for Windows binaries.

Sometimes I keep here non-official releases:

- [SpecIde for Windows 8/10 (32-bit)](https://drive.google.com/file/d/1C5zRcY8itrvP9hKJsIniFWSGP1Zs-3NC/view?usp=sharing)
- [SpecIde for Windows 8/10 (64-bit)](https://drive.google.com/file/d/1X-eZrQ7BfX1SP4DVdLrDT94rwOLgmoQX/view?usp=sharing)
- [SpecIde for older Windows (XP/Vista/7) (32-bit)](https://drive.google.com/file/d/1jta-_Wdl9C7p0AbkiEEBOUJsI95JPsbc/view?usp=sharing)
- [SpecIde for older Windows (XP/Vista/7) (64-bit)](https://drive.google.com/file/d/1x0hd8CP3DUEAUI0stAzojeQLIJrlylPd/view?usp=sharing)

For GNU/Linux and MacOS I'm not providing binaries yet, but SpecIde can be compiled quite easily.

## How to compile it?
### Compiling for GNU/Linux:
1. Install libboost. At least chrono, system, thread and unit_test_framework are required.
1. Install libsfml 2.6.x. Audio, graphics, window and system components are required.
1. Install cmake.
1. Install zlib1g.
1. Clone the repository: `git clone https://github.com/MartianGirl/SpecIde.git`
1. Go into the 'source' directory.
1. Run: `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=_prefix_ -Bbuild -S.` with _prefix_ being the path where SpecIde will be installed. For instance, to install SpecIde into $HOME/bin, the order would be: `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~ -Bbuild -S.`
1. Run: `cmake --build build && cmake --install build`
1. The binaries will be installed in '_prefix_/bin'.
1. Copy the roms from the spectrum-roms package (or from the Windows binaries) to the $HOME/.SpecIde/roms directory.
1. Copy the fonts from the Windows binaries zip to the $HOME/.SpecIde/font directory.
1. Run: bin/SpecIde [options] \<TZXFile.tzx|TAPFile.tap|PZXFile.pzx|CSWFile.csw|DSKFile.dsk\>

### Compiling for MacOS
1. Install brew: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
1. Install cmake: `brew install cmake`
1. Install zlib: `brew install zlib`
1. Install pkgconfig: `brew install pkgconfig`
1. Since the homebrew version of SFML sfml@2 causes problems, compile and install SFML.
1. Create a folder for building and select it: `mkdir Projects && cd Projects`
1. Clone the SFML repository: `git clone --branch 2.6.2 https://github.com/SFML/SFML.git`
1. Run: `pushd SFML && cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release -Bbuild -S. && cmake --build build && sudo cmake --install build && popd`
1. Move the Frameworks into its own folder: `pushd /usr/local && sudo mkdir -p Frameworks && sudo mv lib/*.framework Frameworks && popd`
1. Clone the repository: `git clone https://github.com/MartianGirl/SpecIde.git`
1. Run: `pushd SpecIde/source && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=_prefix_ -DCMAKE_INSTALL_RPATH=/usr/local/lib -Bbuild -S. && cmake --build build && sudo cmake --install build && popd` with _prefix_ being the path where SpecIde will be installed. For instance, to install SpecIde into /usr/local/bin, the order would be: `pushd SpecIde/source && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_INSTALL_RPATH=/usr/local/lib -Bbuild -S. && cmake --build build && sudo cmake --install build && popd`
1. The binaries will be installed in '_prefix_/bin'.
1. Copy the roms from the spectrum-roms package (or from the Windows binaries) to the $HOME/Library/Application Support/SpecIde/roms directory.
1. Copy the fonts from the Windows binaries zip to the $HOME/Library/Application Support/SpecIde/font directory.
1. Run: bin/SpecIde [options] \<TZXFile.tzx|TAPFile.tap|PZXFile.pzx|CSWFile.csw|DSKFile.dsk\>

### Compiling for Windows
I've successfully compiled SpecIde with MinGW and Visual Studio 2015 & 2017.
I've included a script RunCMake.bat that helps in the build process.

1. Install and compile boost. Add the binaries to the PATH.
1. Install and compile sfml. Add the binaries to the PATH.
1. Install and compile zlib. Add the binaries to the PATH.
1. Install cmake.
1. (Optional) Install ninja-builds. It really helps building SpecIde.
1. Edit the RunCMake.bat script. You need to change the lines: `set BOOST_ROOT=\<Path_to_Boost_root_directory\>` and `set SFML_ROOT=\<Path_to_SFML_binaries\>`
1. Run: `RunCMake \[MS32|MS64\] RELEASE`
1. Build it!: `ninja clean & ninja install` or `mingw32-make clean & mingw32-make install` (or use Visual Studio IDE)
1. Copy the roms from the spectrum-roms package (or find them online) to the %APPDATA%/SpecIde/roms directory.
1. Download [this font](https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf "ZX Spectrum font by JFSebastian") to the %APPDATA%/SpecIde/font directory.
1. Run: bin/SpecIde [options] \<TZXFile.tzx|TAPFile.tap|PZXFile.pzx|CSWFile.csw|DSKFile.dsk\>

## How to use it?
SpecIde is invoked from the command line. To run SpecIde, type:
`SpecIde [options] [tapefiles|diskfiles]`

SpecIde supports the following file formats:
- For the Spectrum models: CSW, PZX, TAP, TZX, DSK.
- For the Amstrad CPC models: CSW, CDT, DSK.

### Command line options
The following command line options are available:

```
Model selection options:
--issue2               Spectrum 48K, issue 2.
--issue3|--48          Spectrum 48K, issue 3. (Default)
--48sp                 Spectrum + 48K. (Spanish ROM)
--128                  Spectrum 128K.
--128sp                Spectrum 128K. (Spanish ROM)
--plus2                Spectrum +2.
--plus2sp              Spectrum +2. (Spanish ROM)
--plus2a               Spectrum +2A.
--plus2asp             Spectrum +2A. (Spanish ROM)
--plus3                Spectrum +3.
--plus3sp              Spectrum +3. (Spanish ROM)
--pentagon             Pentagon 128.
--cpc464               Amstrad CPC 464. (BASIC v1)
--cpc464sp             Amstrad CPC 464. (Spanish ROM, BASIC s1)
--cpc464fr             Amstrad CPC 464. (French ROM, BASIC f1)
--cpc664               Amstrad CPC 664. (BASIC v2)
--cpc6128              Amstrad CPC 6128. (BASIC v3)
--cpc6128sp            Amstrad CPC 6128. (BASIC s3)
--cpc6128fr            Amstrad CPC 6128. (BASIC f3)

Joystick options:
--kempston             Map joystick 1 to Kempston (older behaviour). Map joystick 2 to Sinclair 1.
--kempstonnew          Map joystick 1 to Kempston (newer behaviour). Map joystick 2 to Sinclair 1.
--fuller               Map joystick 1 to Fuller and enable Fuller sound. Map joystick 2 to Sinclair 1.
--cursor               Map joystick 1 to cursor keys. Map joystick 2 to Kempston.
--sinclair             Map joystick 1 and 2 to Sinclair interface. (Default)
--pad|--nopad          Map pad extra buttons to keys.

PSG options:
--psg|--nopsg          Emulate AY chip in 48K Spectrum.
--abc|--acb|--mono     Select stereo mode.
--turbo                Select TurboSound with 2 PSGs. (mono)
--turboabc|--turboacb  Select TurboSound with 2 PSGs. (stereo ABC/ACB)
--turbonext            Select Next-style TurboSound with 4 PSGs.
--ay|--ym              Select PSG: AY-3-8912/YM-2149.

Covox options:
--covox                LPT Covox on port $FB. (Mono)
--covox2               Stereo Covox. (ports $FB and $4F)
--covox3               Czech Covox. (ports $1F, $3F, $5F)
--soundrive1           Soundrive on ports $0F, $1F, $4F, $5F.
--soundrive2           Soundrive on ports $F1, $F3, $F9, $FB.
--nocovox              No Covox present.

Misc hardware options:
--sd1                  Emulate Dinamic SD1 hardware protection.
--cmos|--nmos          Emulate CMOS/NMOS Z80. (Affects OUT(C),0 instruction)

Video options:
--fullscreen           Start SpecIde in full screen mode.
--window               Start SpecIde in windowed mode.
--scanlines            Render PAL double scan mode.
--average              Render PAL double scan mode, averaging scanlines.
--nodoublescan         Single scan mode. (Default)
--sync                 Sync emulation to PC video refresh rate.
                           (Use only with 50Hz video modes!)

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
| F3        | Save DSK file to disk. |
| Shift-F3  | Create blank DSK file. |
| F4        | Select next disk image. |
| Shift-F4  | Select previous disk image. |
| F5        | Reset the Spectrum. |
| F6        | Clear SAVE buffer. (Spectrum only) |
| Shift-F6  | Add FlashTAP to SAVE buffer. (Spectrum only) |
| F7        | Write SAVE buffer to disk. (Spectrum only) |
| Shift-F7  | Use SAVE buffer as FlashTAP. (Spectrum only) |
| F8        | Toggle FlashTAP on/off. (Spectrum only) |
| Shift-F8  | Toggle PSG: AY-3-8912/YM-2149. |
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
1. On GNU/Linux: $HOME/.SpecIde/SpecIde.cfg
1. On MacOS: $HOME/Library/Application Support/SpecIde/SpecIde.cfg
1. On Windows: %APPDATA%\SpecIde\SpecIde.cfg

A template SpecIde.cfg.template is provided in the source code, and a sample SpecIde.cfg is included in the binary distributions.

System ROMs can be placed also in the config directory:
- On GNU/Linux: $HOME/.SpecIde/roms
- On MacOS: $HOME/Library/Application Support/SpecIde/roms
- On Windows: %APPDATA%\SpecIde\roms

The following ROMs are included in the binary packages:

| File Name | Model | Size |
|:---:|:---:|:---:|
| 48.rom | Spectrum 48K | 16K |
| 128-0.rom | Spectrum 128K (ROM 0) | 16K |
| 128-1.rom | Spectrum 128K (ROM 1) | 16K |
| plus2-0.rom | Spectrum +2 (ROM 0) | 16K |
| plus2-1.rom | Spectrum +2 (ROM 1) | 16K |
| plus3-0.rom | Spectrum +2A/+3 (ROM 0) | 16K |
| plus3-1.rom | Spectrum +2A/+3 (ROM 1) | 16K |
| plus3-2.rom | Spectrum +2A/+3 (ROM 2) | 16K |
| plus3-3.rom | Spectrum +2A/+3 (ROM 3) | 16K |
| 128-spanish-0.rom | Spanish Spectrum 128K (ROM 0) | 16K |
| 128-spanish-1.rom | Spanish Spectrum 128K (ROM 1) | 16K |
| plus2-spanish-0.rom | Spanish Spectrum +2 (ROM 0) | 16K |
| plus2-spanish-1.rom | Spanish Spectrum +2 (ROM 1) | 16K |
| plus3-spanish-0.rom | Spanish Spectrum +2A/+3 (ROM 0) | 16K |
| plus3-spanish-1.rom | Spanish Spectrum +2A/+3 (ROM 1) | 16K |
| plus3-spanish-2.rom | Spanish Spectrum +2A/+3 (ROM 2) | 16K |
| plus3-spanish-3.rom | Spanish Spectrum +2A/+3 (ROM 3) | 16K |
| pentagon-0.rom | Pentagon 128 (ROM 0 - 128K + TR-DOS support) | 16K |
| pentagon-1.rom | Pentagon 128 (ROM 1 - 48K BASIC) | 16K |
| trdos.rom | TR-DOS ROM for BetaDisk 128 | 16K |
| amsdos.rom | AmsDOS ROM for CPC 664/6128 | 16K |
| cpc464.rom | BASIC ROM for CPC 464 | 32K |
| cpc664.rom | BASIC ROM for CPC 664 | 32K |
| cpc6128.rom | BASIC ROM for CPC 6128 | 32K |

Also, the fonts can be placed in here:
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
After I had the ZX Spectrum working, I thought I could try to emulate the Amstrad CPC series too, because many components are shared. So far, I've got the first three machines running, but there are still many details and problems to be fixed.

## Credits and acknowledgements:
+ David Garijo: For taking the time and helping with the MacOS build process.
+ Ast_A_Moore: For all his help with the +2A timings and port 0x0FFD.
+ César Hernández Bañó: For his help with the initial values for IR register, his comments, and his own emulator ZesarUX.
+ Miguel Mesa: For pointing out that the FLASH attribute was running at half speed.
+ Weiv (for his tests), ICEknight (for his videos), and all the people who helped describing the ULA Snow Effect on 48K/128K/+2 machines.
+ Patrik Rak for all his tests and for developing the PZX file format.
+ Manuel Sainz de Baranda Goñi (ZJoyKiller) for his repository of tests (https://github.com/redcode/ZXSpectrum/wiki/Tests), and for his help in keeping the MacOS version alive.
+ People from the #emulation discord for their ongoing testing efforts to discover every detail on the ZX Spectrum machines.

+ ZXSpectrum.ttf font by [Jorge Ferrer García (JFSebastian)](https://github.com/jfsebastian/zx-spectrum-unicode-font) (Public Domain - [http://unlicense.org](http://unlicense.org))
+ AmstradCPC.ttf font by [Damián Vila Río](https://github.com/damianvila/font-cpc464) ([CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/))
+ Older AmstradCPC.ttf font by [Wesley Clarke](http://fontstruct.fontshop.com) ([CC-BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/deed.en))
+ ZX Spectrum ROMs are (c) Amstrad PLC. Amstrad PLC has kindly given permission for the redistribution of the ZX Spectrum ROMs for their use with emulators, but retains the copyright for them.
+ Amstrad CPC ROMs are (c) Amstrad PLC. CPC BASIC is (c) LocoScript Software (Howard Fisher). Both parties have kindly given permission for the redistribution of their unmodified ROMs for their use with emulators, but retain the copyright for them.
+ [Cliff Lawson's statement](https://groups.google.com/g/comp.sys.amstrad.8bit/c/HtpBU2Bzv_U/m/HhNDSU3MksAJ)
+ [Howard Fisher's statement.](https://groups.google.com/g/comp.sys.amstrad.8bit/c/HtpBU2Bzv_U/m/ClxornAZTgcJ)
