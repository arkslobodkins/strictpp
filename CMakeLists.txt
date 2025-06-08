cmake_minimum_required(VERSION 3.12)
project(strictpp LANGUAGES CXX)

add_subdirectory(strictpp)

option(STRICT_BUILD_EXAMPLES "Enable the compilation of the example files." OFF)
if(STRICT_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()
