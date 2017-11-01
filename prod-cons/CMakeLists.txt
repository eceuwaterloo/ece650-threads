add_executable (t t.c)
target_link_libraries(t helper ${CMAKE_THREAD_LIBS_INIT})

add_executable (t-unsafe t-unsafe.c)
target_link_libraries(t-unsafe helper ${CMAKE_THREAD_LIBS_INIT})
