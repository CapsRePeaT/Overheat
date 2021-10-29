set(MODULE_NAME readers)
set(SOURCES
    src/dummy.cpp
)

# This library
add_library(${MODULE_NAME} STATIC ${SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC include)

# Set boost components name
#set(BOOST_MODULES
#    json)

# Find installed boost
#find_package(Boost 1.77.0 REQUIRED COMPONENTS ${BOOST_MODULES})

# Prepend boost components name with Boost::
#set(BOOST_MODULES_TO_LINK ${BOOST_MODULES})
#list(TRANSFORM BOOST_MODULES_TO_LINK PREPEND Boost::)

# Labrary name
#set(BOOST_LIBRARY_TO_LINK boost-library)
#set(BOOST_LIBRARY_TO_LINK ${BOOST_LIBRARY_TO_LINK} PARENT_SCOPE)

# Add library with boost to link in outer scope. (Because find_package is intendent to use only in this scope)
#add_library(${BOOST_LIBRARY_TO_LINK} INTERFACE)
#target_include_directories(${BOOST_LIBRARY_TO_LINK} INTERFACE ${Boost_INCLUDE_DIRS})
#target_link_libraries(${BOOST_LIBRARY_TO_LINK} INTERFACE ${BOOST_MODULES_TO_LINK})
