add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(readers)
add_subdirectory(renderer)
add_subdirectory(ui)

#  Now dependency tree looks like this ((*) is executable)
#
#       readers
#         ∧ ∧
#         |  \
#         |   reader-interface
#         |  /
#         | ∨
#        core
#        ∧  ∧
#       /    \
#    *ui ---> renderer
#
# And all targets dependent to common
#

find_package(glm)
find_package(glad)
find_package(spdlog)
find_package(magic_enum)
find_package(Boost)

if(USE_QT)
target_link_libraries(ui PRIVATE renderer core common ${QT_LIBRARIES_TO_LINK})
endif()
target_link_libraries(readers PRIVATE reader-interface common core Boost::boost) 
target_link_libraries(reader-test readers gtest_main reader-interface common core)

target_link_libraries(core PRIVATE readers common)
target_link_libraries(core PUBLIC reader-interface)

target_link_libraries(renderer PRIVATE core common glm::glm glad::glad)
target_link_libraries(renderer-test gtest_main core common glm::glm glad::glad renderer)

target_link_libraries(common PUBLIC spdlog::spdlog)
target_link_libraries(common PUBLIC magic_enum::magic_enum)
target_link_libraries(reader-interface INTERFACE common)
