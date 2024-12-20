cmake_minimum_required(VERSION 3.7)

# Project name
project(2DGameTemplate VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

# Define source files
set(SOURCE_FILES
    source/main.cpp
    source/Snek.cpp
    source/Movement.cpp
    source/Apples.cpp
    source/Renderer.cpp
    source/Utils.cpp

)

# Define header files
include_directories(include)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

find_package(SDL2 REQUIRED COMPONENTS SDL2_image SDL2_mixer SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_mixer SDL2_image SDL2_ttf)

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
