set(MODULE_NAME readers)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/* include/*)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES} src/type2/type2_TRM.cpp)

target_include_directories(${MODULE_NAME} PUBLIC include)
target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)

add_executable(reader-test tests/test.cpp tests/type2_test.cpp src/common.h)
target_link_libraries(reader-test gtest_main ${MODULE_NAME})

include(GoogleTest)
gtest_discover_tests(reader-test)
