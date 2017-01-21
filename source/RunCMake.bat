@echo off
set BOOST_ROOT=C:\libs\boost_1_62_0
set Boost_NO_BOOST_CMAKE=TRUE

if .%1.==.. goto USAGE
if .%1.==.GNU. goto GNU
if .%1.==.MS. goto MS
if .%1.==.NINJAMS. goto NINJAMS
if .%1.==.NINJAGNU. goto NINJAGNU

:USAGE
echo Usage:
echo.
echo RunCMake TOOLCHAIN
echo.
echo Toolchains:
echo     GNU: MinGW (32-bit)
echo     MS:  Microsoft Visual Studio 2014 (64-bit)
echo     NINJAGNU: MinGW compiler, Ninja build script (32-bit)
echo     NINJAMS: Visual Studio compiler, Ninja build script (64-bit)
echo.
goto END

:GNU
set MINGW_SYSROOT=C:\MinGW
set SFML_ROOT=C:\libs\MinGW\SFML-2.4.1
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" .
goto END

:MS
set SFML_ROOT=C:\libs\MSVC\SFML-2.4.1
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 14 2015 Win64" .
goto END

:NINJAMS
set SFML_ROOT=C:\libs\MSVC\SFML-2.4.1
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -G "Ninja" .
goto END

:NINJAGNU
set MINGW_SYSROOT=C:\MinGW
set SFML_ROOT=C:\libs\MinGW\SFML-2.4.1
cmake -DCMAKE_BUILD_TYPE=Debug -G "Ninja" .
goto END

:END
