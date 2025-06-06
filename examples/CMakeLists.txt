if(CMAKE_CXX_COMPILER_ID NOT STREQUAL "GNU")
   message(FATAL_ERROR "GCC compiler is required to compile examples")
endif()

file(GLOB_RECURSE CPPFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(CPPFILE ${CPPFILES})
    get_filename_component(CPPNAME ${CPPFILE} NAME_WE)
    add_executable(${CPPNAME} ${CPPFILE})
    target_link_libraries(${CPPNAME} PRIVATE strictpp quadmath)
    target_compile_definitions(${CPPNAME} PRIVATE STRICT_QUAD_PRECISION)

    set_target_properties(${CPPNAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON)
endforeach()
