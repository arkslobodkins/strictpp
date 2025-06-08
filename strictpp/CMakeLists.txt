add_library(strictpp INTERFACE)
add_library(strictpp::strictpp ALIAS strictpp)
target_include_directories(strictpp INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
add_compile_definitions("$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")


option(STRICT_ERROR_EXCEPTIONS "Use exceptions rather than assertions for
      error handling throughout strictpp" OFF)
if(STRICT_ERROR_EXCEPTIONS)
   target_compile_definitions(strictpp INTERFACE STRICT_ERROR_EXCEPTIONS)
endif()


option(STRICT_STACKTRACE "strictpp stacktrace support" OFF)
if(STRICT_ERROR_EXCEPTIONS AND STRICT_STACKTRACE)
   message(WARNING "Both exceptions and stacktrace is enabled, but stacktrace is only useful with assertions")
endif()
if(STRICT_STACKTRACE)
   target_compile_features(strictpp INTERFACE cxx_std_23)
   target_compile_definitions(strictpp INTERFACE STRICT_STACKTRACE)

   # Enabling stacktrace on GCC requires linking against stdc++exp.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      target_link_libraries(strictpp INTERFACE stdc++exp)
   endif()
else()
   target_compile_features(strictpp INTERFACE cxx_std_20)
endif()


option(STRICT_NODISCARD "Extensive support of [[nodiscard]] throughout strictpp" ON)
if(NOT STRICT_NODISCARD)
   target_compile_definitions(strictpp INTERFACE STRICT_REMOVE_NODISCARD)
endif()


