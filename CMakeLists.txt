cmake_minimum_required(VERSION 3.13.0)
project(nly VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(nly INTERFACE)
target_include_directories(nly INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

option(ENABLE_NLY_UNIT_TEST "Enable nly unit test" OFF)
if(ENABLE_NLY_UNIT_TEST)
  add_subdirectory(tests)
endif()

# for use fmt: target_link_libraries(your_target PRIVATE fmt)
# or you can use fmt in header-only way
option(ADD_FMT_VIA_NLY "Whether to add fmt via nly" OFF)
if(ADD_FMT_VIA_NLY)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/upstream/fmt)
endif()

option(ADD_BOOST_VIA_NLY "Whether to add boost via nly" OFF)
if(ADD_BOOST_VIA_NLY)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/upstream/boost)
endif()
