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
        if (current->size >= size) {
            // Creates the new mem block and sets the new address by adding the size of the current block
            MemoryBlock *newBlock = (MemoryBlock *)((char *)current + size + sizeof(MemoryBlock));

            newBlock->size = current->size - size - sizeof(MemoryBlock);
            newBlock->next = current->next;
            current->size = size;
            current->next = newBlock;
            mm.size -= sizeof(MemoryBlock);

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
        if (current->size > 0) {
            printf("Allocated block of size %zu\n", current->size);
        } else {
            printf("Free region of size %zu\n", current->size);
        }
        current = current->next;
    }
}

void *findBlock(void *ptr) {
    MemoryBlock *current = mm.head;

    while (current != NULL) {
        if (current == ptr) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void coalesce(MemoryBlock *ptr) {
    if (ptr == NULL) {
        return;
    }

    MemoryBlock *current = mm.head;
    MemoryBlock *prev = NULL;

    while (current != NULL) {
        if (current == ptr) {
            if (prev != NULL && prev->size < 0) {
                prev->size += current->size + sizeof(MemoryBlock);
                prev->next = current->next;
                mm.size += sizeof(MemoryBlock);
            }
            if (current->next != NULL && current->next->size < 0) {
                current->size += current->next->size + sizeof(MemoryBlock);
                current->next = current->next->next;
                mm.size += sizeof(MemoryBlock);
            }
            return;
        }
        prev = current;
        current = current->next;
    }
}

void myfree(void *ptr) {
    if (mm.head == NULL) {
        printf("Error: Memory manager is not initialized\n");
        return;
    }

    if (ptr == NULL) {
        printf("Error: Pointer is NULL\n");
        return;
    }

    MemoryBlock *current = mm.head;
    MemoryBlock *prev = NULL;
    MemoryBlock *ptrBlock = findBlock(ptr);

//        printf("Freeing block at %p with size:%zu\n", ptrBlock, ptrBlock->size);

    while (current != NULL) {
        if (current == ptrBlock) {
//            printf("Freeing %zu bytes at %p\n", current->size, current);
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                mm.head = current->next;
            }
            current->next = NULL;
            current->size = 0;
            current = NULL;
            // printHeap();
            coalesce(ptrBlock);
            ptrBlock = NULL;
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Error: Pointer is not valid\n");
}

void* myrealloc(void *ptr, size_t size) {

}


void mycleanup() {
}