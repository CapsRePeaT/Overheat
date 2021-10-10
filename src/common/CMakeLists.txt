set(MODULE_NAME common)
set(SOURCES
    src/dummy.cpp
)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)
