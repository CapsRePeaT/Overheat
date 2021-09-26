cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Overheat VERSION 0.1)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable link libraries in folders where they are not building
cmake_policy(SET CMP0079 NEW)

add_subdirectory(src)

