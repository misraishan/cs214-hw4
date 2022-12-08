#ifndef HW4_LIB_MYMALLOC_H
#define HW4_LIB_MYMALLOC_H

#include <stdio.h>
#include <stdlib.h>

// Struct for a memory block
typedef struct MemoryBlock {
    size_t size; // Size of the block, including the block header
    struct MemoryBlock *next; // Pointer to the next block in the list
} MemoryBlock;

// Struct for the memory manager
typedef struct MemoryManager {
    MemoryBlock *head; // Pointer to the first block in the list
    MemoryBlock *lastSearched; // Pointer to the last block searched (for next-fit algorithm)
    int allocAlg; // Allocation algorithm (0 = first fit, 1 = next fit, 2 = best fit)
    size_t size; // Size of the memory manager
} MemoryManager;

// Function declarations
void myinit(int allocAlg);
void *mymalloc(size_t size);
void myfree(void* ptr);
void *myrealloc(void* ptr, size_t size);
void mycleanup();
void printHeap(); // TODO: REMOVE. THIS IS ONLY FOR DEBUGGING PURPOSES.

#endif

