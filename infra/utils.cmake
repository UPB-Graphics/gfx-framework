# Use header guard
include_guard(GLOBAL)


# custom_add_executable
# ---------------------
# Add an application to a project.
#
function(custom_add_executable app_name)
    add_executable(${app_name} ${ARGN})

    # Turn on the -fPIC option
    set_target_properties(${app_name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON)

    # Set the output directories
    set_target_properties(${app_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
endfunction()


# custom_add_library
# ------------------
# Add a library to a project.
#
function(custom_add_library lib_name lib_type)
    add_library(${lib_name} ${lib_type} ${ARGN})

    # Turn on the -fPIC option
    set_target_properties(${lib_name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON)

    # Prevent adding the "lib" prefix to be consistent between MSVC and GNU
    set_target_properties(${lib_name} PROPERTIES
        PREFIX "")

    # Set the output directories
    set_target_properties(${lib_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
endfunction()


# custom_set_build_type
# ---------------------
# Check and set the build type for single-config generators.
#
# Global effects:
#   - in cache: sets the variable CMAKE_BUILD_TYPE
#   - in cache: sets the property CMAKE_BUILD_TYPE::STRINGS
#
function(custom_set_build_type)
    get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (NOT isMultiConfig)
        if (NOT CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE Debug CACHE INTERNAL "")
            message("--== Setting CMAKE_BUILD_TYPE to default value ${CMAKE_BUILD_TYPE}")
        endif()

        get_property(__choices CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS)
        if (NOT __choices)
            set(__choices "Debug;Release;RelWithDebInfo;MinSizeRel")
            set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${__choices})
            message("--== Setting CMAKE_BUILD_TYPE::STRINGS to default value ${__choices}")
        endif()
        if (NOT ${CMAKE_BUILD_TYPE} IN_LIST __choices)
            message(FATAL_ERROR "CMAKE_BUILD_TYPE not in ${__choices}")
        endif()
    endif()
endfunction()