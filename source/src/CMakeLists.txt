cmake_minimum_required(VERSION 3.0)
project(SpecIde)

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)
    # pkg_search_module(GLFW REQUIRED libglfw)
    # pkg_search_module(GL REQUIRED gl)
    # pkg_search_module(AL REQUIRED openal)
    add_executable(SpecIde SpecIde.cc
        GraphicWindow.cc Screen.cc
        Computer.cc Spectrum.cc
        CPU.cc Z80.cc
        Memory.cc)
    target_link_libraries(SpecIde 
        ${SFML_LIBRARIES})
        #         ${GLFW_LIBRARIES}
        #         ${GL_LIBRARIES}
        #         ${AL_LIBRARIES})

# elseif(WIN32)
# # TODO: Check if I can use the FindOpenAL/FindOpenGL modules.
# find_path(AL_INCLUDE_DIRS al.h
# PATH_SUFFIXES include/AL include/OpenAL include
# PATHS
# $ENV{PROGRAMFILES}/OpenAL\ 1.1\ SDK
# )
# 
# find_library(AL_LIBRARIES
# NAMES OpenAL al openal OpenAL32
# PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
# PATHS
# $ENV{PROGRAMFILES}/OpenAL\ 1.1\ SDK
# )
# 
# set (OPENGL_gl_LIBRARY opengl32 CACHE STRING "OpenGL library for win32")
# set (OPENGL_glu_LIBRARY glu32 CACHE STRING "GLU library for win32")
# set (GL_LIBRARIES ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
# add_executable(Spacecrash game0.cc sys_win.cc core.cc stdafx.cc sys.h core.h stdafx.h)
# target_link_libraries(Spacecrash
# ${GL_LIBRARIES}
# ${AL_LIBRARIES})
# # TODO: Set the properties for compiling a Windows application
# #       instead of a console one.
# set_target_properties(Spacecrash PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
# set_target_properties(Spacecrash PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
# set_target_properties(Spacecrash PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
# set_target_properties(Spacecrash PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
endif()

# Set version information in a config.h file.
set(SpecIde_VERSION_MAJOR 0)
set(SpecIde_VERSION_MINOR 0)
configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
    )

# This is for all platforms
include_directories(${SFML_INCLUDE_DIRS})
# include_directories(${GLFW_INCLUDE_DIRS})
# include_directories(${GL_INCLUDE_DIRS})
# include_directories(${AL_INCLUDE_DIRS})

install(TARGETS SpecIde
        RUNTIME
        DESTINATION ${PROJECT_INSTALL_DIR})
