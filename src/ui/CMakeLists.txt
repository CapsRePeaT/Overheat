set(MODULE_NAME ui)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

option(USE_QT "Try to find and link qt libraries to main module" ON)

if(USE_QT)

# Qt components to include in target
set(QT_MODULES
    Widgets Gui OpenGLWidgets
)

# Load Qt libs
#find_package(QT NAMES Qt6 COMPONENTS ${QT_MODULES} REQUIRED)
set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_MODULES} REQUIRED)

# Set list of qt modules to link in format QtX::MODULE_NAME
set(QT_MODULES_TO_LINK ${QT_MODULES})
list(TRANSFORM QT_MODULES_TO_LINK PREPEND Qt${QT_VERSION_MAJOR}::)

# Add target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Do not know why in 5th qt it's not used, but in qtc generated cmakelists it's done through if
    qt_add_executable(${MODULE_NAME}
        MANUAL_FINALIZATION
        ${SOURCES}
    )
else()
    add_executable(${MODULE_NAME} ${SOURCES})
endif()

# Rename result executable
set_target_properties(${MODULE_NAME}
    PROPERTIES
    OUTPUT_NAME overheat
# Set qt tools autorun
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Include publicly this module's include
target_include_directories(${MODULE_NAME} PUBLIC include)
target_include_directories(${MODULE_NAME} PRIVATE forms)


# Add interface library for link it to this target (because find_package intended to use only in this scope)
set(QT_LIBRARIES_TO_LINK qt-libraries)
set(QT_LIBRARIES_TO_LINK ${QT_LIBRARIES_TO_LINK} PARENT_SCOPE)
add_library(${QT_LIBRARIES_TO_LINK} INTERFACE)
target_link_libraries(${QT_LIBRARIES_TO_LINK} INTERFACE ${QT_MODULES_TO_LINK})

# Iterate through modules to get other variables built from list items
foreach(module ${QT_MODULES})
    message(STATUS "Iterate on ${module}")

    # Get INCLUDE DIRS variable of module
    set(MODULE_DIRS_TO_INCLUDE ${Qt6${module}_INCLUDE_DIRS})
    message(STATUS "Include ${MODULE_DIRS_TO_INCLUDE}")
    target_include_directories(${QT_LIBRARIES_TO_LINK} INTERFACE ${MODULE_DIRS_TO_INCLUDE})
endforeach()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${MODULE_NAME})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_program(TOOL_WINDEPLOYQT NAMES windeployqt.debug.bat)
else()
    find_program(TOOL_WINDEPLOYQT NAMES windeployqt)
endif()

add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${TOOL_WINDEPLOYQT} $<TARGET_FILE:${MODULE_NAME}>
    COMMENT "Running windeployqt..."
)

endif() # USE_QT
