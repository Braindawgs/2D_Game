cmake_minimum_required(VERSION 3.14)

# Project name
project(2DGameTemplate VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
set(SOURCE_FILES
    source/main.cpp

)

# Define header files
set(HEADER_FILES
    include/Game.hpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# SDL2 settings
find_package(SDL2 REQUIRED)

if (SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 library not found. Please install SDL2 and set SDL2_DIR correctly.")
endif()

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Organize source/header files in IDE
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})

# Include directories
include_directories(include)

# Enable verbose output for debugging (optional)
# set(CMAKE_VERBOSE_MAKEFILE ON)
