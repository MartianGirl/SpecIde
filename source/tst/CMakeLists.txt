cmake_minimum_required(VERSION 3.0)

if(APPLE)
    find_package(SFML COMPONENTS audio graphics system window)
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)
elseif(WIN32)
    find_package(SFML COMPONENTS audio graphics system window)
    set(Boost_NO_BOOST_CMAKE TRUE)
endif()

find_package(Boost COMPONENTS thread unit_test_framework REQUIRED)

# This is for all platforms
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})

# Add the test executable
add_executable(TZXFileTest
    TZXFileTest.cc
    ${PROJECT_SOURCE_DIR}/src/TZXFile.cc)
target_link_libraries(TZXFileTest
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(DSKFileTest
    DSKFileTest.cc
    ${PROJECT_SOURCE_DIR}/src/DSKFile.cc)
target_link_libraries(DSKFileTest
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80Test
    Z80Test.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80Test
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80AluTest
    Z80AluTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80AluTest
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80InterruptTest
    Z80InterruptTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80InterruptTest
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80JumpTest
    Z80JumpTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80JumpTest
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

add_executable(Z80BitTest
    Z80BitTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80BitTest
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES})

install(TARGETS
    Z80Test Z80AluTest Z80InterruptTest Z80JumpTest Z80BitTest
    TZXFileTest DSKFileTest
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR}/tst)
