# Avoid inheriting flags specified by the user with CMAKE_CXX_FLAGS.
set(CMAKE_CXX_FLAGS "")

file(GLOB_RECURSE CPPFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(CPPFILE ${CPPFILES})
   get_filename_component(CPPNAME ${CPPFILE} NAME_WE)
   add_executable(${CPPNAME} ${CPPFILE})
   add_test(NAME ${CPPNAME} COMMAND ${CPPNAME})
   set_tests_properties(${CPPNAME} PROPERTIES PASS 0)

   # Enable testing quadruple precision on GCC.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set_target_properties(${CPPNAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON
                                                  CXX_EXTENSIONS ON)
      target_compile_definitions(${CPPNAME} PRIVATE "STRICT_QUAD_PRECISION")
      target_link_libraries(${CPPNAME} PRIVATE quadmath)
   else()
      set_target_properties(${CPPNAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON
                                                  CXX_EXTENSIONS OFF)
   endif()

   set_target_properties(${CPPNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                               ${CMAKE_BINARY_DIR}/bin/strictpp/tests)

   # Enable precise floating-point mode on Intel.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                                    "Intel")
      target_compile_options(${CPPNAME} PRIVATE "-fp-model=precise")
   endif()

   # Set warning flags.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      target_compile_options(${CPPNAME} PRIVATE "/W4")
   else()
      target_compile_options(${CPPNAME} PRIVATE -Wall -Wextra -Wdeprecated -Wconversion
                                                -Wsign-conversion)
   endif()

   # -pedantic flag is not used for GNU since it is tested with extensions.
   if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL
                                                    "GNU")
      target_compile_options(${CPPNAME} PRIVATE -pedantic)
   endif()

   # Add -march=native in release mode if available.
   include(CheckCXXCompilerFlag)
   check_cxx_compiler_flag(-march=native HAVE_NATIVE)
   if(HAVE_NATIVE)
      target_compile_options(${CPPNAME} PRIVATE "$<$<CONFIG:Release>:-march=native>")
   endif()

   # Add address sanitizer in debug mode if available. From
   # https://github.com/autodiff/autodiff/blob/main/tests/CMakeLists.txt.
   set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
   check_cxx_compiler_flag(-fsanitize=address HAVE_SANITIZER)
   unset(CMAKE_REQUIRED_FLAGS)
   if(HAVE_SANITIZER)
      target_compile_options(${CPPNAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address>")
      target_link_options(${CPPNAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address>")
   endif()

   target_link_libraries(${CPPNAME} PRIVATE strictpp::strictpp)
endforeach()
