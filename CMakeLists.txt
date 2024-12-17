# Setup
cmake_minimum_required(VERSION 3.10)
project(KioppMathTool C)

# Include directory
set(INCLUDE_DIR inc)

# Source files
set(SOURCES
    src/main.c
    src/kiopp_interface.c
    src/kiopp_math.c
)

# Create executable
add_executable(math_tool ${SOURCES})

# Specify include directories
target_include_directories(math_tool PUBLIC ${INCLUDE_DIR})