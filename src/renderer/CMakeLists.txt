set(MODULE_NAME renderer)
set(SOURCES
    src/OverheatGLRenderer.cpp
)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)


add_executable(renderer-test tests/test_main.cpp)
target_link_libraries(renderer-test gtest_main ${MODULE_NAME})

include(GoogleTest)
gtest_discover_tests(renderer-test)
