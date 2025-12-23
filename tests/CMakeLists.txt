cmake_minimum_required(VERSION 3.13.0)
project(nly_test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/googletest_install_path")
list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/abseil-cpp_install_path")
list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/protobuf_install_path")
list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/boost_install_path")
list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/opencv_install_path")
list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/fmt_install_path")
list(APPEND CMAKE_PREFIX_PATH "${NLY_UPSTREAM_INSTALL}/cppitertools_install_path")

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time beast locale serialization)
find_package(opencv REQUIRED)
find_package(fmt REQUIRED)
find_package(cppitertools REQUIRED)

add_executable(nly_test 
  "${CMAKE_CURRENT_SOURCE_DIR}/source/cppitertools_test.cpp"
  )

target_link_libraries(nly_test PUBLIC
 nly
 GTest::gtest_main
 Boost::date_time Boost::beast Boost::locale Boost::serialization
 fmt::fmt
 cppitertools::cppitertools
 ${OpenCV_LIBS}
 )

if(MSVC)
  target_compile_options(nly_test PUBLIC "/W4" "/bigobj")
endif()