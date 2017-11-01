add_executable (tinout tinout.c)
target_link_libraries(tinout helper ${CMAKE_THREAD_LIBS_INIT})

add_executable (tinout-blk tinout-blk.c)
target_link_libraries(tinout-blk helper ${CMAKE_THREAD_LIBS_INIT})

