set(MODULE_NAME core)
set(SOURCES
    src/core.cpp
    src/databases.h
    src/databases.cpp
    src/shapes.h
    src/shapes.cpp
    src/heatmap.h
    src/heatmap.cpp
)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)


# Interface library for dependency inversion
set(READER_INTERFACE reader-interface)

add_library(${READER_INTERFACE} INTERFACE)
target_include_directories(${READER_INTERFACE} INTERFACE readers)
