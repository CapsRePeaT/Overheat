set(MODULE_NAME renderer)
set(SOURCES 
    
)
add_library(${MODULE_NAME} ${SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC include)