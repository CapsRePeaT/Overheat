set(MODULE_NAME core)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(${MODULE_NAME} STATIC ${SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC include)


# Interface library for dependency inversion
set(READER_INTERFACE reader-interface)
add_library(${READER_INTERFACE} INTERFACE)
target_include_directories(${READER_INTERFACE} INTERFACE readers)

# Interface library for dependency inversion
set(FEM_SOLVER_INTERFACE fem_solver-interface)
add_library(${FEM_SOLVER_INTERFACE} INTERFACE)
target_include_directories(${FEM_SOLVER_INTERFACE} INTERFACE fem_solver)