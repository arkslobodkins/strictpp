add_library(strictpp INTERFACE)
add_library(strictpp::strictpp ALIAS strictpp)
target_compile_features(strictpp INTERFACE cxx_std_20)
target_include_directories(strictpp INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
add_compile_definitions("$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")

if(NOT DEFINED strictpp_FIND_QUIETLY)
   message(STATUS "Found strictpp: ${CMAKE_CURRENT_LIST_DIR}")
endif()
