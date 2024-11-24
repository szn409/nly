cmake_minimum_required(VERSION 3.13.0)
project(nly VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BUILD_TESTS OFF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

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

option(ADD_REDIS_PLUS_PLUS_VIA_NLY "Whether to add redis_plus_plus via nly" OFF)
if(ADD_REDIS_PLUS_PLUS_VIA_NLY)

  # for build and install hiredis
  set(nly_hiredis_build_path "${CMAKE_BINARY_DIR}/hiredis_build")
  set(nly_hiredis_install "${CMAKE_BINARY_DIR}/hiredis_install")

  if(NOT EXISTS "${nly_hiredis_install}/include/hiredis/hiredis.h")
    message(STATUS "----------------------build and install hiredis.----------------------")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -B "${nly_hiredis_build_path}" -S "${CMAKE_CURRENT_SOURCE_DIR}/upstream/hiredis"
    )
    execute_process(
      COMMAND ${CMAKE_COMMAND} --build "${nly_hiredis_build_path}" -t hiredis --config "${CMAKE_BUILD_TYPE}"
    )
    execute_process(
      COMMAND ${CMAKE_COMMAND} --install "${nly_hiredis_build_path}" --prefix "${nly_hiredis_install}" --config "${CMAKE_BUILD_TYPE}"
    )
  endif()

  set(hiredis_DIR "${nly_hiredis_install}/share/hiredis")
  set(REDIS_PLUS_PLUS_BUILD_TEST OFF)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/redis-plus-plus")
  target_link_libraries(nly INTERFACE redis++)
  target_include_directories(nly INTERFACE ${redis++_BINARY_DIR}/src)

endif()

option(ADD_LIBZMQ_VIA_NLY "Whether to add libzmq via nly" OFF)
if(ADD_LIBZMQ_VIA_NLY)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/libzmq")
  target_link_libraries(nly INTERFACE libzmq)
endif()

option(ADD_CPPZMQ_VIA_NLY "Whether to add cppzmq via nly" OFF)
if(ADD_CPPZMQ_VIA_NLY)
  set(CPPZMQ_BUILD_TESTS OFF)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/upstream/cppzmq")
  target_link_libraries(nly INTERFACE cppzmq)
endif()