cmake_minimum_required(VERSION 3.10)
project(strictpp VERSION 1.0 LANGUAGES CXX)

add_library(strictpp INTERFACE)
add_library(strictpp::strictpp ALIAS strictpp)

target_compile_features(strictpp INTERFACE cxx_std_20)
target_include_directories(strictpp INTERFACE ./src)


