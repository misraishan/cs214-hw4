project(mymalloc)
add_library(hw4 mymalloc.h mymalloc.c) #link against pthreads and my_lib
add_executable(hw4test mymalloc.c main.c)
