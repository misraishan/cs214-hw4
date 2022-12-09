#include "mymalloc.h"
#include <string.h>

// Global memory manager
MemoryManager mm;

void *firstFit(size_t size);
void *nextFit(size_t size);
void *bestFit(size_t size);

static char heap[1024 * 1024];

void myinit(int algorithm) {
//    printf("heap address: %p\n", heap); // Prints out the address of the heap for testing purposes

    // Check if algorithm is not 0 1 or 2
    if (algorithm < 0 || algorithm > 2) {
        printf("Invalid algorithm. Please choose 0, 1, or 2.\n");
        exit(1);
    }

    // Create the 1 MB heap
    mm.size = 1024 * 1024;
    mm.head = (MemoryBlock *) heap;
    mm.allocAlg = algorithm;

    // Set the size of the first block
    mm.head->size = mm.size;
    mm.head->next = NULL;
}

void *firstFit(size_t size) {
    MemoryBlock *current = mm.head;

    // Find the first block that is large enough to satisfy the request
    // Return null if nothing found
    while (current != NULL && current->size < size + sizeof(MemoryBlock)) {
        current = current->next;
    }

    if (current == NULL) {
        return NULL;
    }

    // Allocate memory for the new block and cast it to a MemoryBlock
    // Prof said there would be a lot of type casting so I guess this is where he was referring to?
    MemoryBlock *newBlock = (MemoryBlock *)((char *)current + size + sizeof(MemoryBlock));

    // Update the stuff in the new block, and the stuff in the current block
    newBlock->next = current->next;
    newBlock->size = current->size - size - sizeof(MemoryBlock);
    current->size = size;
    current->next = newBlock;

    // Decrease the size of the heap
    mm.size -= sizeof(MemoryBlock);

    return current;
}

void *nextFit(size_t size) {
}

void *bestFit(size_t size) {

}


void* mymalloc(size_t size) {
    // Check if the memory manager has been initialized
    if (mm.head == NULL) {
        printf("Error: Memory manager is not initialized\n");
        return NULL;
    }

    // Check if the size is 0
    if (size == 0) {
        return NULL;
    }

    // Turn into 8 byte aligned
    if (size % 8 != 0) {
        size = size + 8 - (size % 8);
    }

//    printf("Memory manager size is: %d\n", mm.head->size);
    // Check if the size is too large
    if (size > mm.size - sizeof(MemoryBlock)) {
//        printf("Size is: %d\n", size);
        printf("Error: Requested size is too large\n");
        return NULL;
    }

    if (mm.allocAlg == 0) {
        return firstFit(size);
    } else if (mm.allocAlg == 1) {
        return nextFit(size);
    } else if (mm.allocAlg == 2) {
        return bestFit(size);
    } else {
        printf("Error: Invalid allocation algorithm\n");
        return NULL;
    }
}

// THIS IS ONLY FOR DEBUGGING PURPOSES
// TODO: REMOVE THIS FUNCTION BEFORE SUBMISSION
void printHeap() {
    MemoryBlock *current = mm.head;
    while (current != NULL) {
        printf("Block size: %zu\n", current->size);
        current = current->next;
    }
}

void coalesce(MemoryBlock *block) {
    // Coalesce the block with adjacent free blocks
    MemoryBlock *prev = NULL;
    MemoryBlock *next = mm.head;
    while (next != NULL && next < block) {
        prev = next;
        next = next->next;
    }

    // Coalesce with the previous block (if it exists and is free)
    if (prev != NULL && prev->next == NULL) {
        prev->next = next;
        prev->size += block->size + sizeof(MemoryBlock);
    } else {
        // Set the block as free
        block->next = NULL;

        // Insert the block into the free list
        if (prev == NULL) {
            // Insert at the beginning of the list
            block->next = mm.head;
            mm.head = block;
        } else {
            prev->next = block;
            block->next = next;
        }
    }
}

void *findBlock(void *ptr) {
    // Find the block that contains the pointer
    MemoryBlock *current = mm.head;
    printf("ptr: %p\n", ptr);
    while (current != ((char *) ptr)) {
        current = current->next;
    }

    // Check if the block was found
    if (current == NULL) {
        printf("Error: Block not found\n");
        return NULL;
    }

    return current;
}

void myfree(void *ptr) {
    // Check if the pointer is NULL
    if (ptr == NULL) {
        return;
    }

    MemoryBlock *foundBlock = findBlock(ptr);

    if (foundBlock == NULL) {
        return;
    }

    coalesce(foundBlock);
}


void* myrealloc(void *ptr, size_t size) {

}


void mycleanup() {
    mm.head = NULL;
    mm.lastSearched = NULL;
    mm.allocAlg = 0;
    mm.size = 0;
}
