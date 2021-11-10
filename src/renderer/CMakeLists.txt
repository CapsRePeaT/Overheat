set(MODULE_NAME renderer)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)
target_include_directories(${MODULE_NAME} PRIVATE src)

add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

add_executable(renderer-test tests/test_main.cpp)
target_link_libraries(renderer-test gtest_main ${MODULE_NAME})

include(GoogleTest)
gtest_discover_tests(renderer-test)
