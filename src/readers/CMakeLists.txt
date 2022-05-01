set(MODULE_NAME readers)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/* include/*)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)
target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)

add_executable(reader-test tests/solver3d_test.cpp tests/solver2d_test.cpp src/common.h)

include(GoogleTest)
gtest_discover_tests(reader-test)
