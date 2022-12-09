#include "mymalloc.h"
#include <string.h>

// Global memory manager
MemoryManager mm;

void *firstFit(MemoryBlock *head, size_t size);
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
    if (algorithm == 0) {
        mm.allocAlgo = FIRST_FIT;
    } else if (algorithm == 1) {
        mm.allocAlgo = NEXT_FIT;
    } else {
        mm.allocAlgo = BEST_FIT;
    }

    // Set the size of the first block
    mm.head->size = mm.size;
    mm.head->next = NULL;
}

void *firstFit(MemoryBlock *head, size_t size) {
    MemoryBlock *current = head;

    // Find the first block that is large enough to satisfy the request
    // Return null if nothing found
    while (current != NULL && current->size < size + sizeof(MemoryBlock)) {
        current = current->next;
    }

    if (current == NULL) {
        return NULL;
    }

    // Allocate memory for the new block and cast it to a MemoryBlock
    // Prof said there would be a lot of type casting, so I guess this is where he was referring to?
    MemoryBlock *newBlock = (MemoryBlock *)((char *)current + size + sizeof(MemoryBlock));

    // Update the stuff in the new block, and the stuff in the current block
    newBlock->next = current->next;
    newBlock->size = current->size - newBlock->size + sizeof(MemoryBlock);
    current->size = size;
    current->next = newBlock;

    // Decrease the size of the heap
    mm.size -= sizeof(MemoryBlock);

    return current + sizeof(MemoryBlock);
}

void *nextFit(size_t size) {
/*
 * Store the location of the last placed memory block in mm
 * When a new block is requested, start searching from the last placed block
 * If the last placed block is NULL, start searching from the beginning of the heap
 * */

    MemoryBlock *current = mm.head;

    if (mm.lastSearched == NULL) {
        mm.lastSearched = mm.head;
    }
    current = mm.lastSearched;

    MemoryBlock *newBlock = firstFit(current, size);
    mm.lastSearched = newBlock - sizeof(MemoryBlock);

    return newBlock;
}

void *bestFit(size_t size) {
/*
 * Finds the smallest amount of memory where it can take up then coalesces?
 * Loop through the entire heap and keep track of the smallest amount of memory it can take up
 * while being larger than or equal to the size passed in
 *
 * It's not time efficient but its space efficient
 */
    MemoryBlock *current = mm.head;
    MemoryBlock *bestFit = NULL;

    while (current != NULL) {
        if (current->size >= size + sizeof(MemoryBlock)) {
            if (bestFit == NULL) {
                bestFit = current;
            } else if (current->size < bestFit->size) {
                bestFit = current;
            }
        }
        current = current->next;
    }

    if (bestFit == NULL) {
        printf("No best fit found\n");
        return NULL;
    }

    MemoryBlock *newBlock = (MemoryBlock *)((char *)bestFit + size + sizeof(MemoryBlock));
    newBlock->next = bestFit->next;
    newBlock->size = bestFit->size - size - sizeof(MemoryBlock);
//    newBlock->size = bestFit->size - newBlock->size + sizeof(MemoryBlock);
    bestFit->size = size;
    bestFit->next = newBlock;

    mm.size -= sizeof(MemoryBlock);

    return bestFit + sizeof(MemoryBlock);
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

    if (mm.allocAlgo == FIRST_FIT) {
        MemoryBlock *head = mm.head;
        return firstFit(head, size);
    } else if (mm.allocAlgo == NEXT_FIT) {
        return nextFit(size);
    } else if (mm.allocAlgo == BEST_FIT) {
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
//        printf("Location: %p\tBlock size: %zu\tValue is: %d\n", current, current->size, current[sizeof(MemoryBlock)]);
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
////    printf("ptr: %p\n", ptr - sizeof(MemoryBlock));
//    printf("Current: %p\n", &current);
//    printf("char casted ptr: %p\n", (ptr - sizeof(MemoryBlock) - 0x108));
    while (current != NULL && current != (ptr - sizeof(MemoryBlock) - 0x108)) {
//        printf("current: %p\t ptr: %p\n", current, ptr - sizeof(MemoryBlock) - 0x108);
        current = current->next;
    }

    if (current == NULL) {
        printf("Error: Block not found\n");
        return NULL;
    }

    printf("Success\n");

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

//    coalesce(foundBlock);
}


void* myrealloc(void *ptr, size_t size) {
/*
 * Grabs the data from ptr
 * Frees it then creates a new block with size
 * Copies the data from the old block to the new block
 * returns new block
 * */
}


void mycleanup() {
    mm.head = NULL;
    mm.lastSearched = NULL;
    mm.allocAlgo = 0;
    mm.size = 0;
}
