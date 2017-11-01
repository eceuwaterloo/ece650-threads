cmake_minimum_required(VERSION 2.8)
project(ece650-threads)

find_package (Threads)
## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(helper)
add_subdirectory(prod-cons)
add_subdirectory(tim)
add_subdirectory(lk)
add_subdirectory(tinout)
add_subdirectory(kill-ex)
