# SpecIde

This is an attempt at writing a ZX Spectrum emulator using SFML for video, audio and user interface.

Currently, the emulator is functional and is quite accurate. Some of the supported features are:

- Emulation of ZX Spectrum 48K.
- Loading of tapes via .tap and .tzx tape images.
- Full screen video mode.
- Kempston joystick emulation from the PC joystick/gamepad.
- Works in GNU/Linux, Windows, and MacOS.

How to install it:

GNU/Linux:
1. Install libboost. At least chrono, system, thread and unit_test_framework are required.
2. Install libsfml. Audio, graphics, window and system components are required.
3. Install cmake.
4. Clone the repository, go into the 'source' directory.
5. Run: cmake -DCMAKE_BUILD_TYPE=Release .
6. Run: make clean && make install
7. The binaries are installed in 'source/bin'. The test binaries are installed in 'source/bin/tst'
8. To run it: Copy the file 48.rom from the spectrum-roms package (or find it online) to the 'source' directory.
9. Run: bin/SpecIde <TZXFile.tzx|TAPFile.tap>

MacOS: (Thanks to David Garijo for taking the time of making this work and for providing the information!)
1. Install brew: /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
2. Install cmake: brew install cmake
3. Install boost: brew install boost
4. Install sfml: brew install sfml
5. Install pkgconfig: brew install pkgconfig
6. Move /usr/local/Cellar/sfml/2.4.2/share/SFML/cmake/Modules/FindSFML.cmake 
   to /usr/local/Cellar/cmake/3.8.2/share/cmake/Modules
7. Run: cmake -DCMAKE_BUILD_TYPE=Release .
8. Run: make clean && make install
9. To run it: Copy the file 48.rom from the spectrum-roms package (or find it online) to the 'source' directory.
10. Run: bin/SpecIde <TZXFile.tzx|TAPFile.tap>

Windows:
I've successfully compiled SpecIde with MinGW32 and Visual Studio 2015. There is a script RunCMake.bat that helps
in the build process.

1. Install boost.
2. Install sfml.
3. Install cmake.
3b. (Optional) Install ninja-builds. It really helps building SpecIde.

-8<-
4. Edit the RunCMake.bat script. You need to change the following lines:
  a. set BOOST_ROOT=<Path to Boost root directory>
  b. set SFML_ROOT=<Path to SFML binaries>
5. Run: RunCMake <GNU|MS|NINJAGNU|NINJAMS> RELEASE
-8<-
or
-8<-
4. Set some environment variables:
  a. set BOOST_ROOT=<Path to Boost root directory>
  b. set Boost_NO_BOOST_CMAKE=TRUE
  c. set SFML_ROOT=<Path to SFML binaries>
  d. If using MinGW, set MINGW_SYSROOT=<Path where MinGW is installed>
5. Run CMake:
  a. For MinGW: cmake -DCMAKE_BUILD_TYPE=RELEASE -G "MinGW Makefiles" .
  b. For VS2015: cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 14 2015 Win64" .
  c. For Ninja + MinGW: cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" .
  d. For Ninja + VS2015: cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -G "Ninja" .
-8<-
6. Use the chosen build system.
  a. MinGW: mingw32-make install
  b. VS2015: Use the IDE.
  c. Ninja: ninja install
7. Run it. You'll need to add the path to the libraries (SFML and Boost) to the system path.
