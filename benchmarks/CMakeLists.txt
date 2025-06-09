file(GLOB_RECURSE CPPFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include(FetchContent)

FetchContent_Declare(
    Eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen
    GIT_TAG master)

set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE INTERNAL "")
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF CACHE INTERNAL "")
FetchContent_Declare(benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main)

FetchContent_MakeAvailable(Eigen3 benchmark)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
   if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Release")
   endif()
endif()

foreach(CPPFILE ${CPPFILES})
   get_filename_component(CPPNAME ${CPPFILE} NAME_WE)
   add_executable(${CPPNAME} ${CPPFILE})
   set_target_properties(${CPPNAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
   target_link_libraries(${CPPNAME} PRIVATE strictpp Eigen3::Eigen benchmark::benchmark)

   target_compile_definitions(${CPPNAME} PRIVATE EIGEN_NO_DEBUG STRICT_DEBUG_OFF)

   check_cxx_compiler_flag(-march=native HAVE_NATIVE)
   if(HAVE_NATIVE)
      target_compile_options(${CPPNAME} PRIVATE "$<$<CONFIG:Release>:-march=native>")
   endif()

   if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM" OR CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
      target_compile_options(${CPPNAME} PRIVATE "-fp-model=precise")
   endif()

endforeach()

