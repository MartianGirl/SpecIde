cmake_minimum_required(VERSION 3.0)
project(SpecIde)

include(TestBigEndian)

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)
elseif(WIN32)
    find_package(SFML COMPONENTS audio graphics window system)
endif()

include_directories(${SFML_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})

add_executable(SpecIde SpecIde.cc
        GraphicWindow.cc Screen.cc
        CPU.cc Z80.cc ULA.cc
        Computer.cc Spectrum.cc
        Memory.cc Z80Register.cc Z80RegisterSet.cc Z80Decoder.cc
        TZXFile.cc)
target_link_libraries(SpecIde ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

# Set version information in a config.h file.
set(SpecIde_VERSION_MAJOR 0)
set(SpecIde_VERSION_MINOR 0)

# Set endianness in config.h.
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
set(SpecIde_BIG_ENDIAN ${IS_BIG_ENDIAN})

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
)

install(TARGETS SpecIde
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR})
