cmake_minimum_required(VERSION 3.16)
project(sflog
    VERSION "0.1.0"
    HOMEPAGE_URL "https://github.com/gabrieleara/sflog"
    DESCRIPTION "A library for simple formatted logging to stdout"
    LANGUAGES CXX
)
set(CPACK_PACKAGE_VENDOR "Gabriele Ara")

include(GNUInstallDirs)

# ===================== DEPENDENCIES ===================== #

include(cmake/CPM.cmake)

# CPM dependencies are specified in this file
CPMUsePackageLock(package-lock.cmake)

CPMGetPackage(magic_enum)
CPMGetPackage(fmt)

# ==================== GLOBAL OPTIONS ==================== #

include(cmake/options.cmake)

# Choice-based features
add_option_numbered_choice(SFLOG_LEVEL "error" "error;warning;info;debug" "Logger verbosity level")

# ================== PROJECT VARIABLES =================== #

set(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)
set(EXPORT_NAMESPACE "${PROJECT_NAME}::")

# ================== CONFIGURATION FILE ================== #

# Configuration file, will be place in $CMAKE_CURRENT_BINARY_DIR, visible
# to all targets via forced include (see src/CMakeLists.txt)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/sflog/config.h @ONLY)

# ===================== MAIN TARGET ====================== #


add_library(${PROJECT_NAME} INTERFACE)
add_library(${EXPORT_NAMESPACE}${PROJECT_NAME}
    ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${INCLUDES}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(sflog INTERFACE magic_enum::magic_enum fmt::fmt)

target_compile_features(sflog INTERFACE cxx_std_20)
target_compile_options(sflog INTERFACE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wno-long-long
    -Wno-variadic-macros
    -Wfatal-errors
    -ffile-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/=
)

# add_executable(sflog_test EXCLUDE_FROM_ALL
#     test/test0.cpp)

# target_link_libraries(sflog_test PUBLIC sflog fmt)

# set_target_properties(sflog PROPERTIES
#     LANGUAGE CXX
# )

# # ================== INSTALL PROPERTIES ================== #
# include(GNUInstallDirs)

# # Export targets to install
# install(TARGETS rtdag
# RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
