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

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time beast locale serialization)
find_package(opencv REQUIRED)
find_package(fmt REQUIRED)

add_executable(nly_test 
  "${CMAKE_CURRENT_SOURCE_DIR}/source/stl_helper/time_count_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/stl_helper/random_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/boost_helper/date_time_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/boost_helper/base64_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/boost_helper/encoding_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/boost_helper/string_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/boost_helper/memory_pool_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/boost_helper/serialization_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/geometry_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/utility_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/bit_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/memory_stream_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/thread_pool_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/math_test.cpp"
  )
target_link_libraries(nly_test PUBLIC
 nly
 GTest::gtest_main
 Boost::date_time Boost::beast Boost::locale Boost::serialization
 fmt::fmt
 ${OpenCV_LIBS}
 )