cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)

if(WIN32)
    set(Boost_NO_BOOST_CMAKE TRUE)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(ZLIB REQUIRED)

# This is for all platforms
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

# Add the test executable
add_executable(TZXFileTest
    TZXFileTest.cc
    ${PROJECT_SOURCE_DIR}/src/TZXFile.cc
    ${PROJECT_SOURCE_DIR}/src/Utils.cc)
target_link_libraries(TZXFileTest
    ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})

add_executable(DSKFileTest
    DSKFileTest.cc
    ${PROJECT_SOURCE_DIR}/src/DSKFile.cc)
target_link_libraries(DSKFileTest
    ${Boost_LIBRARIES})

add_executable(Z80FileTest
    Z80FileTest.cc
    ${PROJECT_SOURCE_DIR}/src/Z80File.cc)
target_link_libraries(Z80FileTest
    ${Boost_LIBRARIES})

add_executable(Z80Test
    Z80Test.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80Test
    ${Boost_LIBRARIES})

add_executable(Z80AluTest
    Z80AluTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80AluTest
    ${Boost_LIBRARIES})

add_executable(Z80InterruptTest
    Z80InterruptTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80InterruptTest
    ${Boost_LIBRARIES})

add_executable(Z80JumpTest
    Z80JumpTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80JumpTest
    ${Boost_LIBRARIES})

add_executable(Z80BitTest
    Z80BitTest.cc
    ${PROJECT_SOURCE_DIR}/src/Memory.cc
    ${PROJECT_SOURCE_DIR}/src/Z80.cc)
target_link_libraries(Z80BitTest
    ${Boost_LIBRARIES})

add_executable(CRTCTest
    CRTCTest.cc
    ${PROJECT_SOURCE_DIR}/src/CRTC.cc)
target_link_libraries(CRTCTest
    ${Boost_LIBRARIES})

install(TARGETS
    Z80Test Z80AluTest Z80InterruptTest Z80JumpTest Z80BitTest
    TZXFileTest DSKFileTest CRTCTest
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR}/tst)
