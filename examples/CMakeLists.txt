# Avoid inheriting flags specified by the user with CMAKE_CXX_FLAGS.
set(CMAKE_CXX_FLAGS "")

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   message(FATAL_ERROR "GCC compiler is required to compile examples.")
endif()

file(GLOB_RECURSE CPPFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

find_package(OpenMP QUIET)

foreach(CPPFILE ${CPPFILES})
   get_filename_component(CPPNAME ${CPPFILE} NAME_WE)
   add_executable(${CPPNAME} ${CPPFILE})
   set_target_properties(
      ${CPPNAME}
      PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON
                 RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/strictpp/examples)
   target_link_libraries(${CPPNAME} PRIVATE quadmath)
   target_compile_definitions(${CPPNAME} PRIVATE STRICT_QUAD_PRECISION
                                                 "$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")
   # Set warning flags.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      target_compile_options(${CPPNAME} PRIVATE "/W4")
   else()
      target_compile_options(${CPPNAME} PRIVATE -Wall -Wextra -Wdeprecated -Wconversion
                                                -Wsign-conversion)
   endif()

   # example5 uses OpenMP if available.
   if(OpenMP_CXX_FOUND AND CPPNAME STREQUAL "example5")
      target_link_libraries(${CPPNAME} PRIVATE OpenMP::OpenMP_CXX)
   endif()

   target_link_libraries(${CPPNAME} PRIVATE strictpp::strictpp)
endforeach()
