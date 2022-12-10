#ifndef HW4_LIB_MYMALLOC_H
#define HW4_LIB_MYMALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Struct for a memory block
typedef struct MemoryBlock {
    size_t size; // Size of the block, including the block header
    struct MemoryBlock *next; // Pointer to the next block in the list
    struct MemoryBlock *prev; // Pointer to the previous block in the list
    struct MemoryBlock *nextFree; // Pointer to the next free block in the list
    bool isFree; // Whether the block is free or not
} MemoryBlock;

enum allocAlgo {
    FIRST_FIT,
    BEST_FIT,
    NEXT_FIT
};

// Struct for the memory manager
typedef struct MemoryManager {
    MemoryBlock *head; // Pointer to the first block in the list
    MemoryBlock *lastSearched; // Pointer to the last block searched (for next-fit algorithm)
    size_t size; // Size of the memory manager
    enum allocAlgo allocAlgo; // Allocation algorithm
    MemoryBlock *freeList; // Pointer to the first free block in the list
} MemoryManager;

// Function declarations
void myinit(int allocAlg);
void *mymalloc(size_t size);
void myfree(void* ptr);
void *myrealloc(void* ptr, size_t size);
void mycleanup();
void printHeap(); // TODO: REMOVE. THIS IS ONLY FOR DEBUGGING PURPOSES.
unsigned int getSize(); // TODO: REMOVE. THIS IS ONLY FOR DEBUGGING PURPOSES.

#endif

