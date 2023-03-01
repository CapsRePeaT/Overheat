set(MODULE_NAME mesh_processor)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/* include/*)

set(CMAKE_CXX_STANDARD 23)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC ${CINOLIB_INCLUDE_DIRS})
target_include_directories(${MODULE_NAME} PUBLIC include)
target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)