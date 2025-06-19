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

   if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                                    "Intel")
      target_compile_options(${CPPNAME} PRIVATE "-fp-model=precise")
   endif()
endforeach()
