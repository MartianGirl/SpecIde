cmake_minimum_required(VERSION 3.0)

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)
elseif(WIN32)
    find_package(SFML COMPONENTS audio graphics system window)
    # set(Boost_NO_BOOST_CMAKE TRUE)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# This is for all platforms
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})

# Add the test executable
add_executable(GraphicWindowTest
    GraphicWindowTest.cc
    ${PROJECT_SOURCE_DIR}/src/GraphicWindow.cc)
target_link_libraries(GraphicWindowTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(ScreenTest
    ScreenTest.cc
    ${PROJECT_SOURCE_DIR}/src/GraphicWindow.cc
    ${PROJECT_SOURCE_DIR}/src/Screen.cc)
target_link_libraries(ScreenTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(ComputerTest
    ComputerTest.cc
    ${PROJECT_SOURCE_DIR}/src/Computer.cc)
target_link_libraries(ComputerTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(SpectrumTest
    SpectrumTest.cc
    ${PROJECT_SOURCE_DIR}/src/CPU.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Decoder.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Register.cc
    ${PROJECT_SOURCE_DIR}/src/Z80RegisterSet.cc
    ${PROJECT_SOURCE_DIR}/src/Computer.cc
    ${PROJECT_SOURCE_DIR}/src/Spectrum.cc)
target_link_libraries(SpectrumTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(CPUTest
    CPUTest.cc
    ${PROJECT_SOURCE_DIR}/src/CPU.cc)
target_link_libraries(CPUTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80Test
    Z80Test.cc
    ${PROJECT_SOURCE_DIR}/src/CPU.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Decoder.cc
    ${PROJECT_SOURCE_DIR}/src/Z80RegisterSet.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Register.cc)
target_link_libraries(Z80Test
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80AluTest
    Z80AluTest.cc
    ${PROJECT_SOURCE_DIR}/src/CPU.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Decoder.cc
    ${PROJECT_SOURCE_DIR}/src/Z80RegisterSet.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Register.cc)
target_link_libraries(Z80AluTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(MemoryTest
    MemoryTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc)
target_link_libraries(MemoryTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80RegisterTest
    Z80RegisterTest.cc
    ${PROJECT_SOURCE_DIR}/src/Z80Register.cc)
target_link_libraries(Z80RegisterTest
    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

install(TARGETS GraphicWindowTest ScreenTest
    ComputerTest SpectrumTest
    CPUTest Z80Test Z80AluTest
    MemoryTest Z80RegisterTest
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR}/tst)
