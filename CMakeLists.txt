cmake_minimum_required(VERSION 3.16)


# Set root directory, if needed
if (NOT EGCF_ROOT_DIR)
    set(EGCF_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()


# Set the name of the project
project(EGCFramework C CXX)


# Use C99 and C++11
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Include helper scripts
include(${EGCF_ROOT_DIR}/cmake/utils.cmake)
custom_set_build_type()


# Set path to detector scripts, then find and include the required packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${EGCF_ROOT_DIR}/cmake)


# Find required packages
find_package(OpenGL REQUIRED)


# Gather the source files
file(GLOB_RECURSE EGCF_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Source/*.c*
)

# Gather the header files
file(GLOB_RECURSE EGCF_HEADERS_PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Source/*.h*
)


# Gather the include directories
set(EGCF_INCLUDE_DIRS_PRIVATE
    ${EGCF_ROOT_DIR}/libs
    ${EGCF_ROOT_DIR}/libs/Engine
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
custom_add_executable(EGCFramework
    ${EGCF_SOURCES}
    ${EGCF_HEADERS_PRIVATE}
)


# Detect architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(__cmake_arch x64)
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(__cmake_arch x86)
endif()


# Link third-party libraries
target_link_libraries(EGCFramework PRIVATE
    ${OPENGL_LIBRARIES}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(EGCFramework PRIVATE
        ${EGCF_ROOT_DIR}/libs/GL/${__cmake_arch}/Release/glew32.lib
        ${EGCF_ROOT_DIR}/libs/GLFW/${__cmake_arch}/Release/glfw3dll.lib
        ${EGCF_ROOT_DIR}/libs/assimp/${__cmake_arch}/Release/assimp.lib
        ${EGCF_ROOT_DIR}/libs/Engine/${__cmake_arch}/Release/Components.lib
    )
else()
    target_link_libraries(EGCFramework PRIVATE
        GLEW
        glfw
        assimp
        ${EGCF_ROOT_DIR}/libs/Engine/${__cmake_arch}/Release/libcomponents.so
    )
endif()


# Set target properties
target_include_directories(EGCFramework PRIVATE ${EGCF_INCLUDE_DIRS_PRIVATE})


# Set source file properties
set_source_files_properties(${EGCF_HEADERS_PRIVATE}  PROPERTIES HEADER_FILE_ONLY TRUE)


# For Visual Studio, set the working directory and the startup project
if (MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT EGCFramework)
    set_property(TARGET EGCFramework PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()


# Group files under logical folders (mainly for IDEs)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES
    ${EGCF_SOURCES}
    ${EGCF_HEADERS_PRIVATE})


# Add definitions (specific to this project)
# Recommended reading: so@a/24470998/5922876, so@a/11437693/5922876
set(EGCF_CXX_DEFS       ENGINE_DLL_EXPORTS _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(EGCFramework PRIVATE ${EGCF_CXX_DEFS})


# Add compile options (specific to this project)
# Recommended reading: so@a/23995391/5922876
if (MSVC)
    set(EGCF_CXX_FLAGS  /W4 /WX-)
    # TODO fix these warnings
    set(EGCF_CXX_FLAGS  ${EGCF_CXX_FLAGS} /wd4100 /wd4201 /wd4324 /wd4458 /wd4189 /wd4244 /wd4456 /wd4702)
else()
    set(EGCF_CXX_FLAGS  -Wall -Wextra -pedantic -Wno-error)
    # TODO fix these warnings
    set(EGCF_CXX_FLAGS  ${EGCF_CXX_FLAGS}   -Wno-unused-parameter -Wno-unused-variable
                                            -Wno-pedantic -Wno-deprecated-copy -Wno-reorder
                                            -Wno-misleading-indentation -Wno-unused-but-set-variable
                                            -Wno-class-memaccess -Wno-shift-negative-value
                                            -Wno-implicit-fallthrough)
endif()
target_compile_options(EGCFramework PRIVATE ${EGCF_CXX_FLAGS})


# Add post-build events
get_target_property(OUTPUT_DIR EGCFramework RUNTIME_OUTPUT_DIRECTORY)
if (MSVC)
    string(REPLACE "/" "\\" OUTPUT_DIR_BS "${OUTPUT_DIR}")
    string(REPLACE "/" "\\" EGCF_ROOT_DIR_BS "${EGCF_ROOT_DIR}")

    # TODO make these more elegant
    add_custom_command(TARGET EGCFramework POST_BUILD
        COMMAND rmdir /S /Q "${OUTPUT_DIR_BS}\\Resources"
        COMMAND mklink /J "${OUTPUT_DIR_BS}\\Resources" "${EGCF_ROOT_DIR_BS}\\Resources"
        COMMAND rmdir /S /Q "${OUTPUT_DIR_BS}\\Source"
        COMMAND mklink /J "${OUTPUT_DIR_BS}\\Source" "${EGCF_ROOT_DIR_BS}\\Source"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGCF_ROOT_DIR}/libs/GL/${__cmake_arch}/Release/glew32.dll         ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGCF_ROOT_DIR}/libs/GLFW/${__cmake_arch}/Release/glfw3.dll        ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGCF_ROOT_DIR}/libs/assimp/${__cmake_arch}/Release/assimp.dll     ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGCF_ROOT_DIR}/libs/Engine/${__cmake_arch}/Release/Components.dll ${OUTPUT_DIR}
    )
else()
    add_custom_command(TARGET EGCFramework POST_BUILD
        COMMAND rm -rf ${OUTPUT_DIR}/Resources
        COMMAND ln -s ${EGCF_ROOT_DIR}/Resources ${OUTPUT_DIR}/Resources
        COMMAND rm -rf ${OUTPUT_DIR}/Source
        COMMAND ln -s ${EGCF_ROOT_DIR}/Source ${OUTPUT_DIR}/Source
    )
endif()
