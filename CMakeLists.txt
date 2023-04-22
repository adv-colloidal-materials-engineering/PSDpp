cmake_minimum_required(VERSION 3.2)

project(psdpp)

# Define an option for the user to provide the path to the voropp library
option (USE_VOROPP "Use the voro++ library" ON)
option (VOROPP_DIR "voro++ install directory" OFF)

# If USE_VOROPP is ON, set the path to the voropp library
if(USE_VOROPP)
    set(VOROPP_DIR "${VOROPP_DIR}" CACHE PATH "Path to the voropp library")
endif()

# Find the voropp library
find_library(VOROPP_LIBRARY
             NAMES libvoro++.a
             PATHS ${VOROPP_DIR}/lib)

# Set the source files for the hello_world target
set(SOURCES
    src/main.cpp)

# Add the executable target for hello_world
add_executable(hello_world ${SOURCES})

# Link the hello_world target with the voropp library
target_link_libraries(hello_world ${VOROPP_LIBRARY})
