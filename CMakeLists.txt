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

option(ADD_FMT_VIA_NLY "Whether to add fmt via nly" OFF)
if(ADD_FMT_VIA_NLY)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/fmt")
  target_link_libraries(nly INTERFACE fmt)
endif()

option(ADD_BOOST_VIA_NLY "Whether to add boost via nly" OFF)
if(ADD_BOOST_VIA_NLY)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/boost")
endif()

option(ADD_BS_THREAD_POOL_VIA_NLY "Whether to add BS_thread_pool via nly" OFF)
if(ADD_BS_THREAD_POOL_VIA_NLY)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/thread_pool")
  target_link_libraries(nly INTERFACE thread_pool)
endif()

option(ADD_NLOHMANN_JSON_VIA_NLY "Whether to add nlohmann json via nly" OFF)
if(ADD_NLOHMANN_JSON_VIA_NLY)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/json")
  target_link_libraries(nly INTERFACE nlohmann_json)
endif()

option(ADD_CPP_HTTP_LIB_VIA_NLY "Whether to add cpp-httplib via nly" OFF)
if(ADD_CPP_HTTP_LIB_VIA_NLY)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/cpp-httplib")
  target_link_libraries(nly INTERFACE httplib)
endif()