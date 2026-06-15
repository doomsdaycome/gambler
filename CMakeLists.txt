cmake_minimum_required(VERSION 3.23)
project(gambler LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(GTest REQUIRED)

add_executable(test)

target_sources(test PRIVATE test/random.cpp)

target_include_directories(
  test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libgambler/include")

target_link_libraries(
  test
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libgambler/library/libgambler.so"
          GTest::gtest GTest::gtest_main)
