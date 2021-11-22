set(MODULE_NAME readers)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

set(PRIVATE_INCLUDES src)
set(PUBLIC_INCLUDES include)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC ${PUBLIC_INCLUDES})
target_include_directories(${MODULE_NAME} PRIVATE ${PRIVATE_INCLUDES})

target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)

# add_executable(reader-test tests/test.cpp)
# target_include_directories(reader-test PRIVATE ${PUBLIC_INCLUDES} ${PRIVATE_INCLUDES})

# target_link_libraries(reader-test gtest_main ${MODULE_NAME})

# include(GoogleTest)
# gtest_discover_tests(reader-test)
