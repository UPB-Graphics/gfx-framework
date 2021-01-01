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


# Gather the source files
file(GLOB_RECURSE EGXF_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Source/*.c*
)

# Gather the header files
file(GLOB_RECURSE EGXF_HEADERS_PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Source/*.h*
)


# Gather the include directories
set(EGXF_INCLUDE_DIRS_PRIVATE
    ${EGXF_ROOT_DIR}/deps/api
    ${EGXF_ROOT_DIR}/deps/api/libEGXComponents/internal
    #
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/Source/Component
    ${CMAKE_CURRENT_LIST_DIR}/Source/Core
    ${CMAKE_CURRENT_LIST_DIR}/Source/Core/GPU
    ${CMAKE_CURRENT_LIST_DIR}/Source/Core/Managers
    ${CMAKE_CURRENT_LIST_DIR}/Source/Core/Window
    ${CMAKE_CURRENT_LIST_DIR}/Source/include
    ${CMAKE_CURRENT_LIST_DIR}/Source/Laboratoare
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
        ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComponents/${__cmake_arch}/Release/libEGXComponents.lib
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${target_name} PRIVATE
        GLEW
        glfw
        assimp
        ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComponents/${__cmake_arch}/Release/libEGXComponents.so
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
        ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComponents/${__cmake_arch}/Release/libEGXComponents.dylib
    )
endif()


# Set target properties
target_include_directories(${target_name} PRIVATE ${EGXF_INCLUDE_DIRS_PRIVATE})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${target_name})


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


# Add post-build events
get_target_property(OUTPUT_DIR ${target_name} RUNTIME_OUTPUT_DIRECTORY)
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    string(REPLACE "/" "\\" OUTPUT_DIR_BS "${OUTPUT_DIR}")
    string(REPLACE "/" "\\" EGXF_ROOT_DIR_BS "${EGXF_ROOT_DIR}")

    # TODO make these more elegant
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND rmdir /S /Q "${OUTPUT_DIR_BS}\\Resources"
        COMMAND mklink /J "${OUTPUT_DIR_BS}\\Resources" "${EGXF_ROOT_DIR_BS}\\Resources"
        COMMAND rmdir /S /Q "${OUTPUT_DIR_BS}\\Source"
        COMMAND mklink /J "${OUTPUT_DIR_BS}\\Source" "${EGXF_ROOT_DIR_BS}\\Source"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/GL/${__cmake_arch}/Release/glew32.dll       ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/GLFW/${__cmake_arch}/Release/glfw3.dll      ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/assimp/${__cmake_arch}/Release/assimp.dll   ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGXF_ROOT_DIR}/deps/prebuilt/libEGXComponents/${__cmake_arch}/Release/libEGXComponents.dll ${OUTPUT_DIR}
    )
else()
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND rm -rf ${OUTPUT_DIR}/Resources
        COMMAND ln -s ${EGXF_ROOT_DIR}/Resources ${OUTPUT_DIR}/Resources
        COMMAND rm -rf ${OUTPUT_DIR}/Source
        COMMAND ln -s ${EGXF_ROOT_DIR}/Source ${OUTPUT_DIR}/Source
    )
endif()
