cmake_minimum_required(VERSION 3.21)
project(strictpp VERSION 1.0 LANGUAGES CXX)

##########################################################################################
# From https://cliutils.gitlab.io/modern-cmake/modern-cmake.pdf, page 59.
set(DEFAULT_BUILD_TYPE "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.
↪")
   set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build."
                                                      FORCE)
   # Set the possible values of build type for cmake-gui.
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel"
                                                "RelWithDebInfo")
endif()

##########################################################################################
add_subdirectory(strictpp)

option(STRICT_BUILD_EXAMPLES "Enable the compilation of the example files." OFF)
if(STRICT_BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()

option(STRICT_BUILD_BENCHMARKS "Enable the compilation of benchmarks." OFF)
if(STRICT_BUILD_BENCHMARKS)
   add_subdirectory(benchmarks)
endif()

option(STRICT_BUILD_TESTS "Enable the compilation of tests." OFF)
if(STRICT_BUILD_TESTS)
   enable_testing()
   add_subdirectory(tests)
endif()

##########################################################################################
include(GNUInstallDirs)

set(STRICTPP_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/strictpp-${PROJECT_VERSION})

# Install strictpp interface library.
if(STRICT_LINEAR_ALGEBRA)
install(TARGETS strictpp_core strictpp eigen EXPORT strictppTargets)
else()
install(TARGETS strictpp_core strictpp EXPORT strictppTargets)
endif()

install(EXPORT strictppTargets FILE strictppTargets.cmake
        DESTINATION ${STRICTPP_INSTALL_CONFIGDIR} NAMESPACE strictpp::)

# Install headers.
install(DIRECTORY ${PROJECT_SOURCE_DIR}/strictpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT headers PATTERN "CMakeLists.txt" EXCLUDE)

##########################################################################################
include(CMakePackageConfigHelpers)

configure_package_config_file(
   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
   INSTALL_DESTINATION ${STRICTPP_INSTALL_CONFIGDIR})

write_basic_package_version_file(
   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
   VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${STRICTPP_INSTALL_CONFIGDIR})
