cmake_minimum_required(VERSION 3.3)
project(mtftable)

set(CMAKE_C_FLAGS "-std=c99")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES dlist.c dlist.h table.c table.h table_test.c)
add_executable(table_test ${SOURCE_FILES} table_test.c)
#set_property(TARGET mtftable PROPERTY C_STANDARD 99)