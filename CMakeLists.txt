cmake_minimum_required(VERSION 3.16)


# Set root directory, if needed
if (NOT EGXF_ROOT_DIR)
    set(EGXF_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()


# Set the name of the project
set(target_name EGXFramework)
project(${target_name} C CXX)


# Use C99 and C++11
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Include helper scripts
include(${EGXF_ROOT_DIR}/infra/utils.cmake)
custom_set_build_type()


# Fix to hide a bug in CMake. Hopefully able to be removed in the future.
set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)


# Find required packages
find_package(OpenGL REQUIRED)
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(GLEW REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    find_package(ASSIMP REQUIRED)
    find_package(spdlog REQUIRED)
    find_package(Freetype REQUIRED)
endif()


# Set options
option(WITH_LAB_M1 "With module 1 labs" ON)
option(WITH_LAB_M2 "With module 2 labs" ON)
option(WITH_LAB_EXTRA "With extra labs" ON)
option(USE_DEV_COMPONENTS "Use dev components" OFF)


# Set RPATH to avoid using LD_LIBRARY_PATH
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_INSTALL_RPATH "@executable_path")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()


# Set the suffixes for directories, depending on how we want to include
# sources and headers for different courses. If the suffix variables remain
# empty, the GLOB_RECURSE will look for files in the directory called `lab_`
# which does not actually exist, but will not fail.
if (WITH_LAB_M1)
    set(SUFFIX_LAB_M1 "m1")
endif()
if (WITH_LAB_M2)
    set(SUFFIX_LAB_M2 "m2")
endif()
if (WITH_LAB_EXTRA)
    set(SUFFIX_LAB_EXTRA "extra")
endif()


# Gather the source files
file(GLOB_RECURSE EGXF_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/components/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/core/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c*
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M1}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M2}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.c*
)


# Gather the header files
file(GLOB_RECURSE EGXF_HEADERS_PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/components/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/core/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.h*
    #
    ${CMAKE_CURRENT_LIST_DIR}/res/*.glsl
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M1}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M2}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.h*
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M1}/*.glsl
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_M2}/*.glsl
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.glsl
)


# Gather the include directories
set(EGXF_INCLUDE_DIRS_PRIVATE
    ${EGXF_ROOT_DIR}/deps/api
    ${CMAKE_CURRENT_LIST_DIR}/src
)


# Add the executable
custom_add_executable(${target_name}
    ${EGXF_SOURCES}
    ${EGXF_SOURCES_HIDDEN}
    ${EGXF_HEADERS_PRIVATE}
)


# Detect architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch")
        set(__cmake_arch arm64)
    else()
        set(__cmake_arch x86_64)
    endif()
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch")
        set(__cmake_arch armv7)
    else()
        set(__cmake_arch i686)
    endif()
endif()


# Set library suffic
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(__cmake_shared_suffix dll)
    set(__cmake_import_suffix lib)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(__cmake_shared_suffix so)
    set(__cmake_import_suffix so)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(__cmake_shared_suffix dylib)
    set(__cmake_import_suffix dylib)
endif()


# Link third-party libraries
target_link_libraries(${target_name} PRIVATE
    ${OPENGL_LIBRARIES}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${target_name} PRIVATE
        ${EGXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/glew32.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/glfw3dll.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/assimp.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/spdlog/${__cmake_arch}/spdlog.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/freetype/${__cmake_arch}/freetype.lib
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${target_name} PRIVATE
        GLEW
        glfw
        assimp
        spdlog
        freetype
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    target_link_directories(${target_name} PRIVATE
        /usr/local/lib
    )
    target_link_libraries(${target_name} PRIVATE
        GLEW
        glfw
        assimp
        spdlog
        freetype
    )
endif()

if (USE_DEV_COMPONENTS)
    if (NOT TARGET EGXComponents)
        add_subdirectory(src/components_hidden)
        target_link_libraries(${target_name} PRIVATE EGXComponents)
    endif()
else()
    target_link_libraries(${target_name} PRIVATE
        ${EGXF_ROOT_DIR}/deps/prebuilt/EGXComponents/${__cmake_arch}/EGXComponents.${__cmake_import_suffix}
    )
endif()


# Set target properties
target_include_directories(${target_name} PRIVATE ${EGXF_INCLUDE_DIRS_PRIVATE})


# For Visual Studio, set the working directory and the startup project
if (MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${target_name})
    set_property(TARGET ${target_name} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()


# Set source file properties
set_source_files_properties(${EGXF_HEADERS_PRIVATE}  PROPERTIES HEADER_FILE_ONLY TRUE)


# Group files under logical folders (mainly for IDEs)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES
    ${EGXF_SOURCES}
    ${EGXF_SOURCES_HIDDEN}
    ${EGXF_HEADERS_PRIVATE}
)


# Add definitions (specific to this project)
# Recommended reading: so@a/24470998/5922876, so@a/11437693/5922876
set(EGXF_CXX_DEFS       LIBEGXC_EXPORTS GLM_FORCE_SILENT_WARNINGS _CRT_SECURE_NO_WARNINGS SOLVED $<$<CONFIG:Debug>:DEBUG>)
if (WITH_LAB_M1)
    set(EGXF_CXX_DEFS   ${EGXF_CXX_DEFS} WITH_LAB_M1)
endif()
if (WITH_LAB_M2)
    set(EGXF_CXX_DEFS   ${EGXF_CXX_DEFS} WITH_LAB_M2)
endif()
if (WITH_LAB_EXTRA)
    set(EGXF_CXX_DEFS   ${EGXF_CXX_DEFS} WITH_LAB_EXTRA)
endif()
target_compile_definitions(${target_name} PRIVATE ${EGXF_CXX_DEFS})


# Add compile options (specific to this project)
# Recommended reading: so@a/23995391/5922876
if (MSVC)
    set(EGXF_CXX_FLAGS  /W4 /WX-)
    # TODO(developer): Maybe address these warnings, see discussion below.
    set(EGXF_CXX_FLAGS  ${EGXF_CXX_FLAGS} /wd4100 /wd4458 /wd4189)
else()
    set(EGXF_CXX_FLAGS  -Wall -Wextra -pedantic -Wno-error)
    # TODO(developer): Addressing these warnings might make the source code
    # more convoluted, since they are generated by missing code that needs
    # to be implemented. Some of the warnings are caused by dependencies.
    if (CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(EGXF_CXX_FLAGS  ${EGXF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                                -Wno-unused-but-set-variable
                                                -Wno-missing-field-initializers -Wno-sign-compare)
    elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(EGXF_CXX_FLAGS  ${EGXF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                                -Wno-missing-field-initializers -Wno-sign-compare
                                                # Apple clang
                                                -Wno-unknown-warning-option
                                                # Windows clang
                                                -Wno-microsoft-enum-value -Wno-language-extension-token)
    endif()
endif()
target_compile_options(${target_name} PRIVATE ${EGXF_CXX_FLAGS})


# Post-build events. First, we get the directory where the target was
# just built. We will then copy several files and create several symlinks
# into the target's parent directory.
get_target_property(__target_dir ${target_name} RUNTIME_OUTPUT_DIRECTORY)

# On Windows, we copy required binary files next to the target.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${EGXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/glew32.dll"
            "${EGXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/glfw3.dll"
            "${EGXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/assimp.dll"
            "${EGXF_ROOT_DIR}/deps/prebuilt/spdlog/${__cmake_arch}/spdlog.dll"
            "${EGXF_ROOT_DIR}/deps/prebuilt/freetype/${__cmake_arch}/freetype.dll"

            # Destination directory
            "${__target_dir}"
    )
endif()

# This bit is necessary to allow building via Clang on Windows
if (NOT MSVC)
    set(__error_sink || rem)
endif()

# We create symbolic links of several directories next to the target.
# Note for developers: a much more elegant solution would be to use
# `cmake -E rm` and `cmake -E create_symlink` and avoid using OS-specific
# commands. Unfortunately, `create_symlink` will fail if the user is not
# an administrator of the device, as it uses `mklink /D` under the hood.
foreach (dir IN ITEMS "src" "assets")
    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        string(REPLACE "/" "\\" __target_dir "${__target_dir}")
        string(REPLACE "/" "\\" EGXF_ROOT_DIR "${EGXF_ROOT_DIR}")

        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND rmdir /S /Q "${__target_dir}\\${dir}" ${__error_sink}
            COMMAND mklink /J "${__target_dir}\\${dir}" "${EGXF_ROOT_DIR}\\${dir}" ${__error_sink}
        )
    else()
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND rm -rf "${__target_dir}/${dir}"
            COMMAND ln -s "${EGXF_ROOT_DIR}/${dir}" "${__target_dir}/${dir}"
        )
    endif()
endforeach()

# For builds that are supplied with closed-source modules,
# we copy those modules next to the target. If you already
# have access to the original source code, there's no need
# for this step.
if (NOT USE_DEV_COMPONENTS)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EGXF_ROOT_DIR}/deps/prebuilt/EGXComponents/${__cmake_arch}/EGXComponents.${__cmake_shared_suffix}" "${__target_dir}"
    )
endif()
