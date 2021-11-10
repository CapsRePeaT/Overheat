set(MODULE_NAME common)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)

target_compile_features(${MODULE_NAME} PUBLIC cxx_std_20)
target_compile_options(${MODULE_NAME} PUBLIC -Wall -Wextra -Wpedantic -Wno-unused-parameter)
