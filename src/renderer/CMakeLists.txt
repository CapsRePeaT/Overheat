set(MODULE_NAME renderer)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h include/*.cpp include/*.h)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)
find_path(STB_INCLUDE_DIRS "stb_truetype.h")
target_include_directories(${MODULE_NAME} PRIVATE src ${STB_INCLUDE_DIRS})

add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp test/*.h)
add_executable(renderer-test ${TEST_SOURCES})

include(GoogleTest)
gtest_discover_tests(renderer-test)
