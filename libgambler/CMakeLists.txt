cmake_minimum_required(VERSION 3.23)

project(
  gambler
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(gambler SHARED)

target_sources(
  gambler
  PRIVATE source/random.cpp )

target_include_directories(
  gambler
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


set_target_properties(
  gambler
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/library"
             POSITION_INDEPENDENT_CODE ON)
