cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(SpecIde)

include(TestBigEndian)

if(SPECIDE_MEDIA_LIB MATCHES "SDL2")
    message("SpecIde uses SDL2")
    if(APPLE)
        find_package(SDL2 REQUIRED)
    elseif(UNIX)
        find_package(SDL2 REQUIRED)
    elseif(WIN32)
        find_package(SDL2 REQUIRED)
        set(Boost_NO_BOOST_CMAKE TRUE)
    endif()
    set(MEDIA_LIBRARIES ${SDL2_LIBRARIES})
    set(MEDIA_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
    set(SpecIde_SDL2 1)
else()
    message("SpecIde uses SFML")
    if(APPLE)
        find_package(SFML COMPONENTS audio graphics window system)
    elseif(UNIX)
        find_package(SFML COMPONENTS audio graphics window system)
    elseif(WIN32)
        find_package(SFML COMPONENTS audio graphics window system)
        set(Boost_NO_BOOST_CMAKE TRUE)
    endif()
    set(MEDIA_LIBRARIES sfml-audio sfml-graphics sfml-window sfml-system)
    set(SpecIde_SDL2 0)
endif()
message("Media libraries found: " ${MEDIA_LIBRARIES})
message("Media include dirs found: " ${MEDIA_INCLUDE_DIRS})

find_package(Boost COMPONENTS chrono system thread unit_test_framework REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${MEDIA_INCLUDE_DIRS})

add_executable(SpecIde SpecIde.cc
    Screen.cc KeyBinding.cc
    SpeccyScreen.cc Spectrum.cc ULA.cc
    CpcScreen.cc CPC.cc GateArray.cc CRTC.cc
    Z80.cc FDC765.cc
    Tape.cc TAPFile.cc TZXFile.cc CSWFile.cc
    DSKFile.cc)
target_link_libraries(SpecIde ${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${MEDIA_LIBRARIES})

# Set build date information in config.h.
string(TIMESTAMP SpecIde_BUILD_DATE "%Y%m%d" UTC)

# Set timestamp in config.h.
string(TIMESTAMP SpecIde_BUILD_TIME "%Y-%m-%d %H:%M:%S UTC" UTC)

find_package(Git)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    RESULT_VARIABLE result
    OUTPUT_VARIABLE SpecIde_BUILD_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if(result)
    set(SpecIde_BUILD_COMMIT "snapcraft")
endif()

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
