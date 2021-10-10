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

target_link_libraries(ui PRIVATE renderer core common ${QT_LIBRARIES_TO_LINK})
target_link_libraries(core PRIVATE reader-interface readers common)
target_link_libraries(renderer PRIVATE core common)
target_link_libraries(readers PRIVATE reader-interface common)
