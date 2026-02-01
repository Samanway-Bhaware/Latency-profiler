cmake_minimum_required(VERSION 3.20)
project(macos_profiler CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization and Warning Flags
if(MSVC)
    add_compile_options(/O2 /W4)
else()
    add_compile_options(-O3 -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

# Source Files
set(SOURCES
    src/main.cpp
    src/platform/macos/timer_macos.cpp
    src/platform/macos/cpu_affinity_macos.cpp
)

# Include Directories
include_directories(src)

# Executable
add_executable(profiler ${SOURCES})

# Links
if(APPLE)
    add_subdirectory(src/platform/macos)
elseif(UNIX AND NOT APPLE)
    add_subdirectory(src/platform/linux)
elseif(WIN32)
    add_subdirectory(src/platform/windows)
endif()