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

include_directories(${Boost_INCLUDE_DIR})

add_executable(SpecIde SpecIde.cc Screen.cc
        Z80.cc ULA.cc
        Spectrum.cc
        Memory.cc
        Tape.cc TAPFile.cc TZXFile.cc)
target_link_libraries(SpecIde ${Boost_LIBRARIES}
        sfml-audio sfml-graphics sfml-window sfml-system)

# Set version information in a config.h file.
set(SpecIde_VERSION_MAJOR 0)
set(SpecIde_VERSION_MINOR 5)
set(SpecIde_VERSION_TWEAK 6)

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
