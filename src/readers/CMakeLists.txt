set(MODULE_NAME readers)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/* include/*)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
target_include_directories(${MODULE_NAME} PUBLIC include)
target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)

add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests)

add_executable(reader-test tests/solver3d_test.cpp tests/solver2d_test.cpp src/common.h)

include(GoogleTest)
gtest_discover_tests(reader-test)
