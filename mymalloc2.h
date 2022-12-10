#ifndef HW4_LIB_MYMALLOC_H
#define HW4_LIB_MYMALLOC_H

#include <stdio.h>
#include <stdlib.h>

// Function declarations
void myinit(int allocAlg);
void *mymalloc(size_t size);
void myfree(void* ptr);
void *myrealloc(void* ptr, size_t size);
void mycleanup();
void printHeap(); // TODO: REMOVE. THIS IS ONLY FOR DEBUGGING PURPOSES.

#endif
