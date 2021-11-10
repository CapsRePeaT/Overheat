set(MODULE_NAME core)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)


# Interface library for dependency inversion
set(READER_INTERFACE reader-interface)

add_library(${READER_INTERFACE} INTERFACE)
target_include_directories(${READER_INTERFACE} INTERFACE readers)
