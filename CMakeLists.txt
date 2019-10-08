cmake_minimum_required (VERSION 2.6)

project(EGC-Framework C CXX)

set(EGC_LIBS ${PROJECT_SOURCE_DIR}/libs)
set(ENGINE ${EGC_LIBS}/Engine)

set(COMPONENT_LIB ${ENGINE}/x64/Release)

set(EGC_SOURCE ${PROJECT_SOURCE_DIR}/Source)
set(COMPONENT ${EGC_SOURCE}/Component)


set(CORE ${EGC_SOURCE}/Core)
set(CORE_GPU ${CORE}/GPU)
set(CORE_MANAGERS ${CORE}/Managers)
set(CORE_WINDOWS ${CORE}/Window)

set(INCLUDE ${EGC_SOURCE}/include)
set(LABS ${EGC_SOURCE}/Laboratoare)

set(SOURCE_SUBDIRS
    ${EGC_LIBS}
    ${ENGINE}
    ${EGC_SOURCE}
    "."
    ${COMPONENT} 
    ${CORE} 
    ${CORE_GPU} 
    ${CORE_MANAGERS} 
    ${CORE_WINDOWS}
    ${INCLUDE}
    ${LABS})


link_directories(${COMPONENT_LIB})

include_directories(${SOURCE_SUBDIRS})

file(GLOB_RECURSE CPPSources *.cpp)

# Remove strings matching given regular expression from a list.
# @param(in,out) aItems Reference of a list variable to filter.
# @param aRegEx Value of regular expression to match.
function (filter_items aItems aRegEx)
    # For each item in our list
    foreach (item ${${aItems}})
        # Check if our items matches our regular expression
        if ("${item}" MATCHES ${aRegEx})
            # Remove current item from our list
            list (REMOVE_ITEM ${aItems} ${item})
        endif ("${item}" MATCHES ${aRegEx})
    endforeach(item)
    # Provide output parameter
    set(${aItems} ${${aItems}} PARENT_SCOPE)
endfunction (filter_items)

filter_items(CPPSources ".*build.*")

message(WARNING ${SOURCE_SUBDIRS})
message(STATUS ${CPPSources})


add_executable(EGC ${CPPSources})
target_link_libraries(EGC m GL GLEW glfw assimp components)


add_custom_target(link_target ALL
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Resources)