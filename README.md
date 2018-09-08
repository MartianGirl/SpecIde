# SpecIde

This is an attempt at writing a ZX Spectrum emulator using SFML for video, audio and user interface. The goals I set for myself were:
- Accuracy. The emulator should work as much as possible like a real Spectrum. It should be easy to start in full screen mode, without having to select a video mode or care about difficult settings.
- Simplicity. The UI should be as simple as possible, and the emulator should operate only with the function keys as much as possible.
- Reliability. The emulator should never crash.
- Portability. The emulator should be easy to run in different platforms.


Currently, the emulator is functional and is quite accurate. Some of the supported features are:

- Emulation of ZX Spectrum 48K (Issue 2 and Issue 3), 128K, +2, and +2A.
- Emulation of Spanish 128K, +2, and +2A.
- AY-3-8912 sound emulation.
- Loading of tapes via .tap and .tzx tape images.
- Flashloading of .tap files and .tzx that use the ROM routines.
- Saving of .tap files.
- Full screen video mode detection.
- Double scan interlaced modes (a.k.a. Gigascreen).
- Kempston/Sinclair joystick emulation from the PC joystick/gamepad.
- Works in GNU/Linux, Windows, and MacOS.

Windows 32-bit binaries here: https://drive.google.com/open?id=1C5zRcY8itrvP9hKJsIniFWSGP1Zs-3NC

Credits and acknowledgements:
David Garijo for helping with the MacOS build process.
JFSebastian for his wonderful ZXSpectrum.ttf font.
Ast_A_Moore for his help with the +2A timings and port 0x0FFD.
chernandezba for his help with the initial values for IR register.

How to install it:

GNU/Linux:
1. Install libboost. At least chrono, system, thread and unit_test_framework are required.
2. Install libsfml. Audio, graphics, window and system components are required.
3. Install cmake.
4. Clone the repository, go into the 'source' directory.
5. Run: cmake -DCMAKE_BUILD_TYPE=Release .
6. Run: make clean && make install
7. The binaries are installed in 'source/bin'. The test binaries are installed in 'source/bin/tst'
8. To run it: Copy the roms from the spectrum-roms package (or find it online)
   to the ~/.SpecIde/roms directory. Download https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf to ./SpecIde/font directory.
9. Run: bin/SpecIde \<TZXFile.tzx|TAPFile.tap\>

MacOS: (Thanks to David Garijo for taking the time of making this work and for providing the information!)
1. Install brew: /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
2. Install cmake: brew install cmake
3. Install boost: brew install boost
4. Install sfml: brew install sfml
5. Install pkgconfig: brew install pkgconfig
6. Simlink FindSFML.cmake in cmake modules `ln -s $(brew --prefix sfml)/share/SFML/cmake/Modules/FindSFML.cmake $(brew --prefix cmake)/share/cmake/Modules/FindSFML.cmake`
7. Run: cmake -DCMAKE_BUILD_TYPE=Release .
8. Run: make clean && make install
8. To run it: Copy the roms from the spectrum-roms package (or find it online)
   to the ~/.SpecIde/roms directory. Download https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf to ./SpecIde/font directory.
10. Run: bin/SpecIde \<TZXFile.tzx|TAPFile.tap\>

Windows:
I've successfully compiled SpecIde with MinGW32 and Visual Studio 2015. There is a script RunCMake.bat that helps
in the build process.

1. Install boost.
2. Install sfml.
3. Install cmake. Optionally, install ninja-builds. It really helps building SpecIde.

4. Edit the RunCMake.bat script. You need to change the following lines:<br>
   4.1. set BOOST_ROOT=\<Path_to_Boost_root_directory\><br>
   4.2. set SFML_ROOT=\<Path_to_SFML_binaries\><br>
5. Run: RunCMake \[GNU|MS|NINJAGNU|NINJAMS\] RELEASE

-or-

4. Set some environment variables:<br>
   4.1. set BOOST_ROOT=\<Path to Boost root directory\><br>
   4.2. set Boost_NO_BOOST_CMAKE=TRUE<br>
   4.3. set SFML_ROOT=\<Path to SFML binaries\><br>
   4.4. If using MinGW, set MINGW_SYSROOT=\<Path where MinGW is installed\><br>
   
5. Run CMake: (Do not miss the dots at the end of the commands!!!)<br>
   5.1. For MinGW: cmake -DCMAKE_BUILD_TYPE=RELEASE -G "MinGW Makefiles" .<br>
   5.2. For VS2015: cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 14 2015 Win64" .<br>
   5.3. For Ninja + MinGW: cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" .<br>
   5.4. For Ninja + VS2015: cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -G "Ninja" .<br>
  
6. Use the chosen build system.<br>
   6.1. MinGW: mingw32-make install<br>
   6.2. VS2015: Use the IDE.<br>
   6.3. Ninja: ninja install<br>
  
7. Run it. You'll need to add the path to the libraries (SFML and Boost) to the system path. The font https://github.com/jfsebastian/zx-spectrum-unicode-font/blob/master/build/ZXSpectrum.ttf and Spectrum roms can be placed under %APPDATA%\SpecIde\roms and %APPDATA%\SpecIde\font.
