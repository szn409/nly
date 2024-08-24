cmake_minimum_required(VERSION 3.13.0)
project(thread_pool VERSION 0.1.0)

add_library(thread_pool INTERFACE)
target_include_directories(thread_pool INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/thread-pool/include")