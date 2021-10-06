set(MODULE_NAME readers)
set(SOURCES
    
)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)
