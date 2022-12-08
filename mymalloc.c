#include "mymalloc.h"
#include <string.h>

// Global memory manager
MemoryManager mm;

void *firstFit(size_t size);
void *nextFit(size_t size);
void *bestFit(size_t size);

void myinit(int algorithm) {
    // Check if algorithm is not 0 1 or 2
    if (algorithm < 0 || algorithm > 2) {
        printf("Invalid algorithm. Please choose 0, 1, or 2.\n");
        exit(1);
    }

    // Create the 1 MB heap
    mm.size = 1024 * 1024;
    mm.head = (MemoryBlock *) malloc(mm.size);
    mm.allocAlg = algorithm;

    // Set the size of the first block
    mm.head->size = mm.size;
    mm.head->next = NULL;
}

void *firstFit(size_t size) {
    MemoryBlock *current = mm.head;

    while (current != NULL) {
        if (current->size >= size + sizeof(MemoryBlock)) {
            // Creates the new mem block and sets the new address by adding the size of the current block
            MemoryBlock *newBlock = (MemoryBlock *)((char *)current + size + sizeof(MemoryBlock));

            newBlock->size = current->size - size - sizeof(MemoryBlock);
            newBlock->next = current->next;
            current->size = size;
            current->next = newBlock;

//            printf("Allocated %d bytes at %p using first-fit algorithm.\n", size, current);
            return current;
        }
        current = current->next;
    }

    return NULL;
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
//        printf("Adjusted size to %d bytes\n", size);
    }

//    printf("Memory manager size is: %d\n", mm.head->size);
    // Check if the size is too large
    if (size > mm.size - sizeof(MemoryBlock)) {
        printf("Size is: %d\n", size);
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

void myfree(void *ptr) {
}

void* myrealloc(void *ptr, size_t size) {

}


void mycleanup() {
}