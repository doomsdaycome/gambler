cmake_minimum_required(VERSION 3.23)

project(gambler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

option(BUILD_SHARED_LIBS "Build gambler as a shared library" ON)
option(BUILD_TESTS "Build gambler unit tests" ON)
option(BUILD_EXAMPLES "Build gambler example binaries" ON)

add_library(gambler)
target_sources(gambler PRIVATE src/random.cpp)

target_include_directories(
    gambler
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(BUILD_SHARED_LIBS)
    include(GenerateExportHeader)
    generate_export_header(gambler)
endif()

if(BUILD_EXAMPLES)
    add_executable(gambler_example examples/example.cpp)
    target_link_libraries(gambler_example PRIVATE gambler)
endif()

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL
            https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    add_executable(gambler_test tests/test.cpp)
    target_link_libraries(gambler_test PRIVATE gambler GTest::gtest_main)
    include(GoogleTest)

    gtest_discover_tests(gambler_test)
endif()
