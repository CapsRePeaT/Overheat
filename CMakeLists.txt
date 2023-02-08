cmake_minimum_required(VERSION 3.17)

# Don't allow to generate build in sources
include(cmake/PreventSourceBuilds.cmake)

# Init vcpkg in CMAKE_TOOLCHAIN_FILE (must be set before preject() call)
include(cmake/Vcpkg.cmake)

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

# Setup conan
#include(cmake/Conan.cmake)
#run_conan()

# Set color mode for gcc and clang
include(cmake/ColorfullOutput.cmake)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable link libraries in folders where they are not building
cmake_policy(SET CMP0079 NEW)
# Make timestamps from FetchContent to represent time when archive was extracted
cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(src)
