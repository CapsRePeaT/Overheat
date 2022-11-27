option(USE_SYSTEM_VCPKG "Use vcpkg from your system instead of skymp submodule" OFF)

if(WIN32)
  set(VCPKG_TARGET_TRIPLET "x64-windows")
else()
  set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

macro(set_from_environment VARIABLE)
  if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
    set(${VARIABLE} $ENV{${VARIABLE}})
  endif ()
endmacro()

set_from_environment(VCPKG_ROOT)

# Use installed VCPKG if exists
if (USE_SYSTEM_VCPKG AND DEFINED VCPKG_ROOT AND EXISTS ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else ()
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif ()

# File from vcpkg submodule. This indicates inability to find this file or checkout submodules.
if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
  set(msg "${CMAKE_TOOLCHAIN_FILE} doesn't exist. It seems that vcpkg submodule is not initialized.")
  set(msg "${msg}\nUse commands below to initialize:")
  set(msg "${msg}\n  git submodule init")
  set(msg "${msg}\n  git submodule update")
  message(FATAL_ERROR "${msg}")
endif()

