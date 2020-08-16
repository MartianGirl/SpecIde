@echo off
set BOOST_ROOT=C:\libs\boost_1_74_0

if .%1.==.. goto USAGE

if .%2.==.DEBUG. goto DEBUG
set BUILD=Release
goto TOOLCHAIN

:DEBUG
set BUILD=Debug

:TOOLCHAIN
if .%1.==.GNU. goto GNU
if .%1.==.MS32. goto MS32
if .%1.==.MS64. goto MS64
if .%1.==.NINJAGNU. goto NINJAGNU
if .%1.==.NINJAMS32. goto NINJAMS32
if .%1.==.NINJAMS64. goto NINJAMS64

:USAGE
echo Usage:
echo.
echo RunCMake TOOLCHAIN [DEBUG]
echo.
echo Toolchains:
echo     GNU: MinGW (32-bit)
echo     MS32: Microsoft Visual Studio 2015 (32-bit)
echo     MS64: Microsoft Visual Studio 2015 (64-bit)
echo     NINJAGNU: MinGW compiler, Ninja build script (32-bit)
echo     NINJAMS32: Visual Studio compiler, Ninja build script (32-bit)
echo     NINJAMS64: Visual Studio compiler, Ninja build script (64-bit)
echo.
echo RELEASE: Generate Release builds, instead of Debug builds (default)
echo.
goto END

:GNU
set MINGW_SYSROOT=C:\MinGW
set CMAKE_PREFIX_PATH=C:\libs\MinGW\SFML-2.5.1
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "MinGW Makefiles" .
goto END

:MS32
set CMAKE_PREFIX_PATH=C:\libs\SFML\x86;C:\libs\zlib\x86
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "Visual Studio 16 2019" -A Win32 .
goto END

:MS64
set CMAKE_PREFIX_PATH=C:\libs\SFML\x64;C:\libs\zlib\x64
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "Visual Studio 16 2019" -A x64 .
goto END

:NINJAGNU
set MINGW_SYSROOT=C:\MinGW
set CMAKE_PREFIX_PATH=C:\libs\MinGW\SFML-2.5.1
cmake -DCMAKE_BUILD_TYPE=%BUILD% -G "Ninja" .
goto END

:NINJAMS32
set CMAKE_PREFIX_PATH=C:\libs\SFML\x86;C:\libs\zlib\x86
cmake -DCMAKE_BUILD_TYPE=%BUILD% -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -G "Ninja" .
goto END

:NINJAMS64
set CMAKE_PREFIX_PATH=C:\libs\SFML\x64;C:\libs\zlib\x64
cmake -DCMAKE_BUILD_TYPE=%BUILD% -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -G "Ninja" .
goto END

:END
