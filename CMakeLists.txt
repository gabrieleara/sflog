cmake_minimum_required(VERSION 3.16)
project(sflog
    VERSION "0.1.0"
    HOMEPAGE_URL "https://github.com/gabrieleara/sflog"
    DESCRIPTION "A library for simple formatted logging to stdout"
    LANGUAGES CXX
)
set(CPACK_PACKAGE_VENDOR "Gabriele Ara")

include(GNUInstallDirs)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif()

# ==================== GLOBAL OPTIONS ==================== #

option(SFLOG_OPT_INSTALL "Generate and install sflog target" ${IS_TOPLEVEL_PROJECT})

include(cmake/options.cmake)

# Choice-based features
add_option_numbered_choice(SFLOG_LEVEL "error" "error;warning;info;debug" "Logger verbosity level")

# ===================== DEPENDENCIES ===================== #

include(cmake/CPM.cmake)

# CPM dependencies are specified in this file
CPMUsePackageLock(package-lock.cmake)

CPMGetPackage(magic_enum)
CPMGetPackage(fmt)

# ================== PROJECT VARIABLES =================== #

set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(EXPORT_NAMESPACE "${PROJECT_NAME}::")

# ================== CONFIGURATION FILE ================== #

# Configuration file, will be place in $CMAKE_CURRENT_BINARY_DIR, visible
# to all targets via forced include (see src/CMakeLists.txt)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sflog/config.h @ONLY)

# ===================== MAIN TARGET ====================== #

add_library(${PROJECT_NAME} INTERFACE)
add_library(${EXPORT_NAMESPACE}${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${INCLUDES}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(${PROJECT_NAME} INTERFACE
    magic_enum::magic_enum
    fmt::fmt
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# add_executable(sflog_test EXCLUDE_FROM_ALL
#     test/test0.cpp)

# target_link_libraries(sflog_test PUBLIC sflog fmt)

# set_target_properties(sflog PROPERTIES
#     LANGUAGE CXX
# )

# ================== INSTALL PROPERTIES ================== #

if(SFLOG_OPT_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    file(GLOB_RECURSE HEADERS "${INCLUDES}/*.h" "${INCLUDES}/*.hxx" "${INCLUDES}/*.hpp")
    string(REPLACE "/${CMAKE_LIBRARY_ARCHITECTURE}" "" CMAKE_INSTALL_LIBDIR_ARCHIND "${CMAKE_INSTALL_LIBDIR}")
    foreach(headerFile ${HEADERS})
        get_filename_component(headerFileParentDir "${headerFile}" DIRECTORY)
        file(RELATIVE_PATH headerFileRelParentDir "${INCLUDES}" "${headerFileParentDir}")

        install(FILES "${headerFile}"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${headerFileRelParentDir}"
        )
    endforeach()

    export(TARGETS "${PROJECT_NAME}"
        NAMESPACE "${EXPORT_NAMESPACE}"
        FILE "${CMAKE_EXPORT_FILE_BASENAME}"
        EXPORT_LINK_INTERFACE_LIBRARIES
    )

    install(EXPORT "${PROJECT_NAME}"
        FILE "${CMAKE_CONFIG_FILE_BASENAME}"
        NAMESPACE "${EXPORT_NAMESPACE}"
        DESTINATION "${CMAKE_INSTALL_LIBDIR_ARCHIND}/cmake/${PROJECT_NAME}"
    )

    write_basic_package_version_file(
        "${CMAKE_CONFIG_VERSION_FILE_NAME}"
        #VERSION "100500.100500.100500"  # any version of same bitness suits. CMake cannot compare to infinity, so use a large number we expect to be greater than any future version
        VERSION ${_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(FILES "${CMAKE_CONFIG_VERSION_FILE_NAME}"
        DESTINATION "${CMAKE_INSTALL_LIBDIR_ARCHIND}/cmake/${PROJECT_NAME}"
    )

    configure_pkg_config_file("${PROJECT_NAME}"
        NAME "${PROJECT_NAME}"
        VERSION "${PROJECT_VERSION}"
        DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}"
        URL "${CPACK_PACKAGE_HOMEPAGE_URL}"
        INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR_ARCHIND}"
        INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}"
    )


    # configure_package_config_file(
    #     "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    #     "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    #     INSTALL_DESTINATION
    #     ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    # install(EXPORT ${PROJECT_NAME}
    #     FILE ${PROJECT_NAME}Targets.cmake
    #     NAMESPACE ${EXPORT_NAMESPACE}
    #     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    # install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    #     "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    #     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    # install(DIRECTORY
    #     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)
    # install(DIRECTORY
    #     ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME} DESTINATION include)

endif()
