cmake_minimum_required(VERSION 3.0)
project(SpecIde)

include(TestBigEndian)

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)
    include_directories(${SFML_INCLUDE_DIRS})
    add_executable(SpecIde SpecIde.cc
            GraphicWindow.cc Screen.cc
            Computer.cc Spectrum.cc
            CPU.cc Z80.cc
            Memory.cc Z80Register.cc)
    target_link_libraries(SpecIde 
            ${SFML_LIBRARIES})
elseif(WIN32)
    set(SFML_ROOT C:/libs/MinGW/SFML-2.4.1)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML COMPONENTS audio graphics window system)
    include_directories(${SFML_INCLUDE_DIR})
    add_executable(SpecIde SpecIde.cc
            GraphicWindow.cc Screen.cc
            Computer.cc Spectrum.cc
            CPU.cc Z80.cc
            Memory.cc Z80Register.cc)
    target_link_libraries(SpecIde 
            ${SFML_LIBRARIES}
            ${SFML_DEPENDENCIES})
endif()


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
