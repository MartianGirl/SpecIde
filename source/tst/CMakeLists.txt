cmake_minimum_required(VERSION 3.0)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Add the test executable
include_directories(${PROJECT_SOURCE_DIR}/src ${Boost_INCLUDE_DIR})
add_executable(GenericModuleTest
    GenericModuleTest.cc)
# ${PROJECT_SOURCE_DIR}/src/GenericModule.cc)

target_link_libraries(GenericModuleTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

install(TARGETS GenericModuleTest
    RUNTIME
    DESTINATION ${PROJECT_INSTALL_DIR}/tst)
