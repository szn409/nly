cmake_minimum_required(VERSION 3.13.0)
project(nly_unit_test VERSION 0.1.0)

add_executable(nly_unit_test 
  ${CMAKE_CURRENT_SOURCE_DIR}/time/time_count_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/time/date_time_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/encoding_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/string_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/memory_pool_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/random_test.cpp
  )

if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(googletest)
target_link_libraries(nly_unit_test 
  gtest_main 
  nly 
  boost_date_time boost_locale boost_signals2 boost_pool)