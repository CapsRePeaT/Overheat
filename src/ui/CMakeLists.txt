set(MODULE_NAME core)
set(SOURCES 
    src/main.cpp
)
add_executable(${MODULE_NAME} ${SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC include)