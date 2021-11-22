option(FORCE_COLORED_OUTPUT
       "Always produce ANSI-colored output (GNU/Clang only)." ON)

if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Setting color mode for gcc")
    add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Setting color mode for clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()
