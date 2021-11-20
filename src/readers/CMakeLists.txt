set(MODULE_NAME readers)
set(SOURCES
        src/virtex_reader.cpp src/virtex_data_classes.h include/idata_provider.h src/virtex_data_provider.h)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)
target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)

add_executable(reader-test tests/test.cpp)
target_link_libraries(reader-test gtest_main ${MODULE_NAME})

include(GoogleTest)
gtest_discover_tests(reader-test)
