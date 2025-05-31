#!/bin/sh

DEPS="xorg-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev"

sudo apt install ${DEPS}

OLDDIR=$(pwd)

cd /tmp

git clone https://github.com/SFML/SFML.git -b 2.6.2
cd SFML
cmake -DCMAKE_BUILD_TYPE=Release -Bbuild -S.
cmake --build build
sudo cmake --install build
cd ..

git clone https://github.com/MartianGirl/SpecIde.git
cd SpecIde
cmake -DCMAKE_BUILD_TYPE=Release -Bbuild -Ssource
cmake --build build
sudo cmake --install build
cd ..

rm -rf SFML SpecIde

cd $OLDDIR

# g++ -O3 -fomit-frame-pointer -funroll-loops -Wall -Wextra -pedantic -std=c++17 -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu SpecIde.cc Utils.cc Screen.cc KeyBinding.cc SpeccyScreen.cc Spectrum.cc ULA.cc CpcScreen.cc CPC.cc GateArray.cc CRTC.cc Z80.cc FDC765.cc Tape.cc CSWFile.cc PZXFile.cc TAPFile.cc TZXFile.cc DSKFile.cc SNAFile.cc Z80File.cc -lz -lGL -lGLU -lsfml-system -lsfml-graphics -lsfml-window -lsfml-system -lfreetype -lpng16 -lbrotlidec -lbrotlicommon -lsfml-audio -lsfml-system -lopenal -lvorbisenc -lvorbis -logg -lvorbisfile -lFLAC -lm -lsfml-network -lsfml-system -o SpecIde
