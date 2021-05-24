cmake_minimum_required(VERSION 3.0)
project(SpecIde)

include(TestBigEndian)

if(APPLE)
    find_package(SFML COMPONENTS audio graphics window system)
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)
elseif(WIN32)
    find_package(SFML COMPONENTS audio graphics window system)
    set(Boost_NO_BOOST_CMAKE TRUE)
endif()

find_package(Boost COMPONENTS chrono system thread unit_test_framework REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

add_executable(SpecIde SpecIde.cc Console.cc
    Screen.cc
    SpeccyScreen.cc Spectrum.cc ULA.cc
    CpcScreen.cc CPC.cc GateArray.cc CRTC.cc
    Z80.cc FDC765.cc
    Tape.cc TAPFile.cc TZXFile.cc CSWFile.cc
    DSKFile.cc)
target_link_libraries(SpecIde ${Boost_LIBRARIES} ${ZLIB_LIBRARIES}
    sfml-audio sfml-graphics sfml-window sfml-system)

# Set version information in a config.h file.
set(SpecIde_VERSION_MAJOR 0)
set(SpecIde_VERSION_MINOR 6)
set(SpecIde_VERSION_TWEAK 0)

# Set endianness in config.h.
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
set(SpecIde_BIG_ENDIAN ${IS_BIG_ENDIAN})

if(WIN32)
    set(SpecIde_HOME_ENV "APPDATA")
    set(SpecIde_CONF_DIR "SpecIde")
    set(SpecIde_ON_UNIX 0)
elseif(APPLE)
    set(SpecIde_HOME_ENV "HOME")
    set(SpecIde_CONF_DIR "Library/Application Support/SpecIde")
    set(SpecIde_ON_UNIX 1)
elseif(UNIX)
    set(SpecIde_HOME_ENV "HOME")
    set(SpecIde_CONF_DIR ".SpecIde")
    set(SpecIde_ON_UNIX 1)
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
)

install(TARGETS SpecIde
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR})
