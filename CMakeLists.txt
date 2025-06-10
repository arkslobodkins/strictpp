cmake_minimum_required(VERSION 3.20)
project(strictpp LANGUAGES CXX)

add_subdirectory(strictpp)

option(STRICT_BUILD_EXAMPLES "Enable the compilation of the example files." OFF)
if(STRICT_BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()

option(STRICT_BUILD_BENCHMARKS "Enable the compilation of benchmarks." OFF)
if(STRICT_BUILD_BENCHMARKS)
   add_subdirectory(benchmarks)
endif()
