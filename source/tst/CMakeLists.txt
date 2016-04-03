cmake_minimum_required(VERSION 3.0)

if(UNIX)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SFML REQUIRED sfml-all)

    # Add the test executable
    add_executable(GraphicWindowTest
        GraphicWindowTest.cc
        ${PROJECT_SOURCE_DIR}/src/GraphicWindow.cc)
    target_link_libraries(GraphicWindowTest
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${SFML_LIBRARIES})

    add_executable(ScreenTest
        ScreenTest.cc
        ${PROJECT_SOURCE_DIR}/src/GraphicWindow.cc
        ${PROJECT_SOURCE_DIR}/src/Screen.cc)
    target_link_libraries(ScreenTest
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${SFML_LIBRARIES})
endif()

# This is for all platforms
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIRS})

install(TARGETS GraphicWindowTest ScreenTest
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR}/tst)
