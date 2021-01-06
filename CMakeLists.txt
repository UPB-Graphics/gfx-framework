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
include(${EGXF_ROOT_DIR}/cmake/utils.cmake)
custom_set_build_type()


# Fix to hide a bug in CMake. Hopefully able to be removed in the future.
set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)


# Set path to detector scripts, then find and include the required packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${EGXF_ROOT_DIR}/cmake)


# Find required packages
find_package(OpenGL REQUIRED)
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(GLEW REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    find_package(ASSIMP REQUIRED)
endif()


# Set options
option(WITH_LAB_EGC "With EGC labs" ON)
option(WITH_LAB_SPG "With SPG labs" ON)
option(WITH_LAB_EXTRA "With extra labs" ON)


# Set the suffixes for directories, depending on how we want to include
# sources and headers for different courses. If the suffix variables remain
# empty, the GLOB_RECURSE will look for files in the directory called `lab_`
# which does not actually exist, but will not fail.
if (WITH_LAB_EGC)
    set(SUFFIX_LAB_EGC "egc")
endif()
if (WITH_LAB_SPG)
    set(SUFFIX_LAB_SPG "spg")
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
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EGC}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_SPG}/*.c*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.c*
)


# Gather the header files
file(GLOB_RECURSE EGXF_HEADERS_PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/components/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/core/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.h*
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EGC}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_SPG}/*.h*
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}/*.h*
)


# Gather the include directories
set(EGXF_INCLUDE_DIRS_PRIVATE
    ${EGXF_ROOT_DIR}/deps/api
    ${EGXF_ROOT_DIR}/deps/api/libEGXComp/internal
    #
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/component
    ${CMAKE_CURRENT_LIST_DIR}/src/core
    ${CMAKE_CURRENT_LIST_DIR}/src/utils
    #
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EGC}
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_SPG}
    ${CMAKE_CURRENT_LIST_DIR}/src/lab_${SUFFIX_LAB_EXTRA}
)


# Add the executable
custom_add_executable(${target_name}
    ${EGXF_SOURCES}
    ${EGXF_HEADERS_PRIVATE}
)


# Detect architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(__cmake_arch x86_64)
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(__cmake_arch i686)
endif()


# Link third-party libraries
target_link_libraries(${target_name} PRIVATE
    ${OPENGL_LIBRARIES}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${target_name} PRIVATE
        ${EGXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/Release/glew32.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/Release/glfw3dll.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/Release/assimp.lib
        ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComp/${__cmake_arch}/Release/libEGXComp.lib
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${target_name} PRIVATE
        GLEW
        glfw
        assimp
        ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComp/${__cmake_arch}/Release/libEGXComp.so
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
        ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComp/${__cmake_arch}/Release/libEGXComp.dylib
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
    ${EGXF_HEADERS_PRIVATE}
)


# Add definitions (specific to this project)
# Recommended reading: so@a/24470998/5922876, so@a/11437693/5922876
set(EGXF_CXX_DEFS       LIBEGXC_EXPORTS GLM_FORCE_SILENT_WARNINGS _CRT_SECURE_NO_WARNINGS)
if (WITH_LAB_EGC)
    set(EGXF_CXX_DEFS   ${EGXF_CXX_DEFS} WITH_LAB_EGC)
endif()
if (WITH_LAB_SPG)
    set(EGXF_CXX_DEFS   ${EGXF_CXX_DEFS} WITH_LAB_SPG)
endif()
if (WITH_LAB_EXTRA)
    set(EGXF_CXX_DEFS   ${EGXF_CXX_DEFS} WITH_LAB_EXTRA)
endif()
target_compile_definitions(${target_name} PRIVATE ${EGXF_CXX_DEFS})


# Add compile options (specific to this project)
# Recommended reading: so@a/23995391/5922876
if (MSVC)
    set(EGXF_CXX_FLAGS  /W4 /WX-)
    # TODO fix these warnings
    set(EGXF_CXX_FLAGS  ${EGXF_CXX_FLAGS} /wd4100 /wd4458 /wd4189 /wd4244)
else()
    set(EGXF_CXX_FLAGS  -Wall -Wextra -pedantic -Wno-error)
    # TODO fix these warnings
    if (CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(EGXF_CXX_FLAGS  ${EGXF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                                -Wno-deprecated-copy
                                                -Wno-unused-but-set-variable
                                                -Wno-missing-field-initializers -Wno-sign-compare)
    elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(EGXF_CXX_FLAGS  ${EGXF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                                -Wno-deprecated-copy -Wno-constant-logical-operand
                                                -Wno-address-of-packed-member -Wno-unused-private-field
                                                -Wno-missing-field-initializers -Wno-sign-compare
                                                -Wno-unknown-warning-option)
    endif()
endif()
target_compile_options(${target_name} PRIVATE ${EGXF_CXX_FLAGS})


# Define post-build events
function(post_build)
    get_target_property(output_dir ${target_name} RUNTIME_OUTPUT_DIRECTORY)
    set(res_dir "res")
    set(src_dir "src")

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        string(REPLACE "/" "\\" OUTPUT_DIR_BS "${output_dir}")
        string(REPLACE "/" "\\" EGXF_ROOT_DIR_BS "${EGXF_ROOT_DIR}")

        # TODO make these more elegant
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND rmdir /S /Q "${OUTPUT_DIR_BS}\\${res_dir}"
            COMMAND mklink /J "${OUTPUT_DIR_BS}\\${res_dir}" "${EGXF_ROOT_DIR_BS}\\${res_dir}"
            COMMAND rmdir /S /Q "${OUTPUT_DIR_BS}\\${src_dir}"
            COMMAND mklink /J "${OUTPUT_DIR_BS}\\${src_dir}" "${EGXF_ROOT_DIR_BS}\\${src_dir}"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/Release/glew32.dll       ${output_dir}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/Release/glfw3.dll      ${output_dir}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/Release/assimp.dll   ${output_dir}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComp/${__cmake_arch}/Release/libEGXComp.dll ${output_dir}
        )
    else()
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND rm -rf ${output_dir}/${res_dir}
            COMMAND ln -s ${EGXF_ROOT_DIR}/${res_dir} ${output_dir}/${res_dir}
            COMMAND rm -rf ${output_dir}/${src_dir}
            COMMAND ln -s ${EGXF_ROOT_DIR}/${src_dir} ${output_dir}/${src_dir}
        )
    endif()
endfunction()


# Call post-build events
post_build()
