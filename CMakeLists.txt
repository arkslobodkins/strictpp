cmake_minimum_required(VERSION 3.12)
project(strictpp VERSION 1.0 LANGUAGES CXX)

add_subdirectory(strictpp)

option(STRICTPP_BUILD_EXAMPLES "Enable the compilation of the example files." OFF)
if(STRICTPP_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()
