# Avoid inheriting flags specified by the user with CMAKE_CXX_FLAGS.
set(CMAKE_CXX_FLAGS "")
# Always run benchmarks in release mode, since we are primarily interested in benchmarking
# optimized versions of strictpp and Eigen.
set(CMAKE_BUILD_TYPE "Release")

##########################################################################################
include(FetchContent)

FetchContent_Declare(Eigen3 GIT_REPOSITORY https://gitlab.com/libeigen/eigen
                     GIT_TAG master)

set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE INTERNAL "")
# Disable assembly tests to suppress warnings related to "possibly broken assembly tests".
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF CACHE INTERNAL "")
FetchContent_Declare(benchmark GIT_REPOSITORY https://github.com/google/benchmark.git
                     GIT_TAG main)

FetchContent_MakeAvailable(Eigen3 benchmark)

##########################################################################################
file(GLOB_RECURSE CPPFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(CPPFILE ${CPPFILES})
   get_filename_component(CPPNAME ${CPPFILE} NAME_WE)
   add_executable(${CPPNAME} ${CPPFILE})
   set_target_properties(
      ${CPPNAME}
      PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF
                 RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/strictpp/benchmarks)
   target_link_libraries(${CPPNAME} PRIVATE Eigen3::Eigen benchmark::benchmark)

   # Disable assertions.
   target_compile_definitions(${CPPNAME} PRIVATE EIGEN_NO_DEBUG STRICT_DEBUG_OFF)

   # Add -march=native in release mode if available.
   include(CheckCXXCompilerFlag)
   check_cxx_compiler_flag(-march=native HAVE_NATIVE)
   if(HAVE_NATIVE)
      target_compile_options(${CPPNAME} PRIVATE "$<$<CONFIG:Release>:-march=native>")
   endif()

   # Enable precise floating-point mode on Intel.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                                    "Intel")
      target_compile_options(${CPPNAME} PRIVATE "-fp-model=precise")
   endif()

endforeach()
