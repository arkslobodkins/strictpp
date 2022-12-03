add_library(strictpp INTERFACE)
add_library(strictpp::strictpp ALIAS strictpp)

##########################################################################################
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
   target_link_libraries(strictpp INTERFACE ${MATH_LIBRARY})
endif()

##########################################################################################
target_include_directories(
   strictpp INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
                      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

##########################################################################################
set(STRICT_DEBUG
    "SMART_DEFAULT"
    CACHE STRING "Available options are ON, OFF, or SMART_DEFAULT, which
      defaults to false in release build and defaults to true in all other builds.
      ON or OFF overrides these defaults.")
# Set the possible values for cmake-gui.
set_property(CACHE STRICT_DEBUG PROPERTY STRINGS "ON" "OFF" "SMART_DEFAULT")

if(STRICT_DEBUG STREQUAL "SMART_DEFAULT")
   target_compile_definitions(strictpp INTERFACE "$<$<CONFIG:Release>:STRICT_DEBUG_OFF>")
elseif(STRICT_DEBUG STREQUAL "ON")
   # Nothing to be done.
elseif(STRICT_DEBUG STREQUAL "OFF")
   target_compile_definitions(strictpp INTERFACE STRICT_DEBUG_OFF)
else()
   message(
      FATAL_ERROR
         "Invalid debug configuration. Available options are ON, OFF, or SMART_DEFAULT.")
endif()

##########################################################################################
option(STRICT_ERROR_EXCEPTIONS "Enable use of exceptions rather than assertions for
      error handling throughout strictpp." OFF)
if(STRICT_ERROR_EXCEPTIONS)
   target_compile_definitions(strictpp INTERFACE STRICT_ERROR_EXCEPTIONS)
endif()

##########################################################################################
option(STRICT_STACKTRACE "Enable strictpp stacktrace support." OFF)
if(STRICT_ERROR_EXCEPTIONS AND STRICT_STACKTRACE)
   message(WARNING "Both exceptions and stacktrace are enabled, but stacktrace
         is only useful with assertions.")
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

##########################################################################################
option(STRICT_NODISCARD "Enable extensive support of [[nodiscard]] throughout strictpp."
       ON)
if(NOT STRICT_NODISCARD)
   target_compile_definitions(strictpp INTERFACE STRICT_REMOVE_NODISCARD)
endif()
