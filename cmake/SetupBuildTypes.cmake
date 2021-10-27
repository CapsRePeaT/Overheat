# Setting up switchers
option(ADD_BUILD_DEBUG "Add Debug build config" ON)
option(ADD_BUILD_RELEASE "Add Release build config" ON)
option(ADD_BUILD_MINSIZEREL "Add size optimized Release build config" OFF)
option(ADD_BUILD_RELWITHDEBINFO "Add Release build config with debug info" OFF)

# Setting up lists for one-pass loop
set(CONFIG_TYPES_SWITCHERS
    ADD_BUILD_DEBUG
    ADD_BUILD_RELEASE
    ADD_BUILD_MINSIZEREL
    ADD_BUILD_RELWITHDEBINFO
)
set(POSSIBLE_TYPES
    Debug
    Release
    MinSizeRel
    RelWithDebInfo
)

unset(CMAKE_CONFIGURATION_TYPES CACHE)

# Loop over switchers and possible types and append the parameter
# by switched on types
foreach(switcher type IN ZIP_LISTS CONFIG_TYPES_SWITCHERS POSSIBLE_TYPES)
    if(${switcher})
        list(APPEND CMAKE_CONFIGURATION_TYPES ${type})
    endif()
endforeach()

# If all types were swithed off set Debug as default
if(NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Set configuration type to Debug as all was switched off.")
    list(APPEND CMAKE_CONFIGURATION_TYPES Debug)
endif()

# Cache the parameter
set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES}
    CACHE STRING "Possible types of build" FORCE)

# If CMAKE_BUILD_TYPE was not specified or not specified correctly,
# then set and constraint it
if(NOT CMAKE_BUILD_TYPE OR
    NOT CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    list(GET CMAKE_CONFIGURATION_TYPES 0 CMAKE_BUILD_TYPE)
endif()
# Cache CMAKE_BUILD_TYPE
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Type of build." FORCE)
set_property(CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
