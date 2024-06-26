cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(SpecIde)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build. Options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

# Set an output directory for our binaries
set(PROJECT_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
message("Setting PROJECT_INSTALL_DIR to ${CMAKE_INSTALL_PREFIX}/bin")

# Bump up warning levels appropriately for clang, gcc & msvc.
# Also set debug/optimization flags depending on the build type. IDE users
# choose this when selecting the build mode in their IDE.
if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS} -g -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
        "${CMAKE_CXX_FLAGS} -g -O1 -DDEBUG")
    if(WIN32)
        set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "X86")
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" 
                CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    if(CMAKE_EXE_LINKER_FLAGS_DEBUG MATCHES "/INCREMENTAL(:YES)?")
        string(REGEX REPLACE "/INCREMENTAL(:YES)?" "/INCREMENTAL:NO"
                CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
    else()
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO")
    endif()
    if(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO MATCHES "/INCREMENTAL(:YES)?")
        string(REGEX REPLACE "/INCREMENTAL(:YES)?" "/INCREMENTAL:NO"
                CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
    else()
        set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /INCREMENTAL:NO")
    endif()
endif()

# Look in the src subdirectory
add_subdirectory(src)

# Look in the tst subdirectory
if(SPECIDE_BUILD_TESTS)
    add_subdirectory(tst)
endif()
