cmake_minimum_required(VERSION 3.13.0)
project(nly VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(nly INTERFACE)
target_include_directories(nly INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

option(ENABLE_NLY_UNIT_TEST "Enable nly unit test" OFF)
if(ENABLE_NLY_UNIT_TEST)
  add_subdirectory(tests)
endif()