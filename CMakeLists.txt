cmake_minimum_required(VERSION 3.12)
project(strictpp VERSION 1.0 LANGUAGES CXX)

add_library(strictpp INTERFACE)
add_library(strictpp::strictpp ALIAS strictpp)
target_compile_features(strictpp INTERFACE cxx_std_20)
target_include_directories(strictpp INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
add_compile_definitions("$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")
