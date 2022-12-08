cmake_minimum_required(VERSION 3.24)
project(hw4_lib C)

set(CMAKE_C_STANDARD 99)

add_library(hw4_lib mymalloc.c tests.c)
