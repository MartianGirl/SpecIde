@echo off
set BOOST_ROOT=C:\libs\boost_1_66_0
set Boost_NO_BOOST_CMAKE=TRUE

if .%1.==.. goto USAGE

if .%2.==.RELEASE. goto RELEASE
set BUILD=Debug
goto TOOLCHAIN

:RELEASE
set BUILD=Release

:TOOLCHAIN
if .%1.==.GNU. goto GNU
if .%1.==.MS. goto MS
if .%1.==.NINJAMS. goto NINJAMS
if .%1.==.NINJAGNU. goto NINJAGNU

:USAGE
echo Usage:
echo.
echo RunCMake TOOLCHAIN [RELEASE]
echo.
echo Toolchains:
echo     GNU: MinGW (32-bit)
echo     MS:  Microsoft Visual Studio 2014 (64-bit)
echo     NINJAGNU: MinGW compiler, Ninja build script (32-bit)
echo     NINJAMS: Visual Studio compiler, Ninja build script (64-bit)
echo.
echo RELEASE: Generate Release builds, instead of Debug builds (default)
echo.
goto END

:GNU
set MINGW_SYSROOT=C:\MinGW
set SFML_ROOT=C:\libs\MinGW\SFML-2.4.2
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "MinGW Makefiles" .
goto END

:MS
set SFML_ROOT=C:\libs\MSVC\SFML-2.4.2
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "Visual Studio 15 2017 Win64" .
goto END

:NINJAMS
set SFML_ROOT=C:\libs\MSVC\SFML-2.4.2
cmake -DCMAKE_BUILD_TYPE=%BUILD% -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -G "Ninja" .
goto END

:NINJAGNU
set MINGW_SYSROOT=C:\MinGW
set SFML_ROOT=C:\libs\MinGW\SFML-2.4.2
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "Ninja" .
goto END

:END
