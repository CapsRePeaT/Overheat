cmake_minimum_required(VERSION 3.17)

# Don't allow to generate build in sources
include(cmake/PreventSourceBuilds.cmake)

set(PROJECT_NAME Overheat VERSION 0.1)
project(${PROJECT_NAME} LANGUAGES CXX)

# Build result binaries in separate directory in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# C++ std20 without extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup configuration types with ability to choose wich ones to generate.
# Also considering multiconfig generators
include(cmake/SetupBuildTypes.cmake)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable link libraries in folders where they are not building
cmake_policy(SET CMP0079 NEW)

add_subdirectory(src)
