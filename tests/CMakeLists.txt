enable_testing()

# Avoid inheriting flags specified by the user with CMAKE_CXX_FLAGS.
set(CMAKE_CXX_FLAGS "")

file(GLOB_RECURSE CPPFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(CPPFILE ${CPPFILES})
   get_filename_component(CPPNAME ${CPPFILE} NAME_WE)
   add_executable(${CPPNAME} ${CPPFILE})
   add_test(NAME ${CPPNAME} COMMAND ${CPPNAME})

   # Include testing quadruple precision on GCC.
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
                                               ${CMAKE_BINARY_DIR}/bin/tests)

   # Enable precise floating-point mode on Intel.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                                    "Intel")
      target_compile_options(${CPPNAME} PRIVATE "-fp-model=precise")
   endif()

   # Set warning flags.
   if(MSVC)
      target_compile_options(${CPPNAME} PRIVATE "/W4")
   else()
      target_compile_options(${CPPNAME} PRIVATE -Wall -Wextra -Wdeprecated -Wconversion
                                                -Wsign-conversion)
   endif()

   # Add -march=native in release mode if it is available.
   include(CheckCXXCompilerFlag)
   check_cxx_compiler_flag(-march=native HAVE_NATIVE)
   if(HAVE_NATIVE)
      target_compile_options(${CPPNAME} PRIVATE "$<$<CONFIG:Release>:-march=native>")
   endif()

   # Add address sanitizer in debug mode if it is available.
   set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
   check_cxx_compiler_flag(-fsanitize=address HAVE_SANITIZER)
   unset(CMAKE_REQUIRED_FLAGS)
   if(HAVE_SANITIZER)
      target_compile_options(${CPPNAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address>")
      target_link_options(${CPPNAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address>")
   endif()

endforeach()
