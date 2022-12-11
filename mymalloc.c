#include "mymalloc.h"
#include <string.h>

void *firstFit(MemoryBlock *head, size_t size);

void *nextFit(size_t size);

void *bestFit(size_t size);

// Global memory manager
static MemoryManager mm;

static char heap[1024 * 1024];

// TODO: REMOVE. THIS IS ONLY FOR DEBUGGING PURPOSES.
unsigned int getSize() {
    return mm.size;
}

// Global memory manager
static MemoryManager mm;

// Create a 1 MB heap as a static variable
static char heap[1024 * 1024];

void myinit(int algorithm) {
    // Check if algorithm is not 0 1 or 2
    if (algorithm < 0 || algorithm > 2) {
        printf("Invalid algorithm. Please choose 0, 1, or 2.\n");
        exit(1);
    }

    // Set the head to point to the beginning of the heap
    mm.head = (MemoryBlock *) heap;

    // Set the allocation algorithm
    if (algorithm == 0) {
        mm.allocAlgo = FIRST_FIT;
    } else if (algorithm == 1) {
        mm.allocAlgo = NEXT_FIT;
    } else {
        mm.allocAlgo = BEST_FIT;
    }

    // Set the size of the first block
    mm.head->size = sizeof(heap);
    mm.head->next = NULL;
    mm.head->prev = NULL;
    mm.head->isFree = true;
}

void *findBlock(void *ptr) {
    MemoryBlock *current = mm.head;

    //search for ptr
    while (current != NULL) {
        if (((char *) current + sizeof(MemoryBlock)) == (char *) ptr) {
            // Check if the given pointer is within the bounds of this MemoryBlock
            if (ptr < (void *) ((char *) current + sizeof(MemoryBlock) + current->size)) {
                break;
            }
        }
        current = current->next;
    }
    return current;
}

void *firstFit(MemoryBlock *head, size_t size) {
    MemoryBlock *current = head;

    // Find the first block that is large enough to satisfy the request
    // Return null if nothing found
    while (current != NULL) { //while loop is not null
        if (current->size < size + sizeof(MemoryBlock) ||
            current->isFree == false) { //is current size < size or is current allocated?
            current = current->next;
            continue;
        }
        break;
    }

    if (current == NULL) {
        return NULL;
    }

    // Allocate memory for the new block and cast it to a MemoryBlock
    // Prof said there would be a lot of type casting, so I guess this is where he was referring to?
    MemoryBlock *newBlock = (MemoryBlock *) ((char *) current + sizeof(MemoryBlock) + size);

    // Update the stuff in the new block, and the stuff in the current block
    newBlock->prev = current;
    newBlock->next = current->next;

    if (current->next != NULL) {
        current->next->prev = newBlock;
    }

    newBlock->size = current->size - size - sizeof(MemoryBlock);
    current->next = newBlock; //added thing for checking
    newBlock->isFree = true; //free memory
    current->size = size;
    current->isFree = false; //allocated memory

    return (char *) current + sizeof(MemoryBlock);
}

void *nextFit(size_t size) {
/*
 * Store the location of the last placed memory block in mm
 * When a new block is requested, start searching from the last placed block
 * If the last placed block is NULL, start searching from the beginning of the heap
 * */
    MemoryBlock *current = mm.lastSearched;

    if (mm.lastSearched == NULL) {
        current = mm.head;
    } else {
        current = mm.lastSearched;
    }

    while (current != NULL) { //while loop is not null
        if (current->size < size + sizeof(MemoryBlock) ||
            current->isFree == false) { //is current size < size or is current allocated?
            current = current->next;
            continue;
        }
        break;
    }

    if (current == NULL) {
        return NULL;
    }

    // Allocate memory for the new block and cast it to a MemoryBlock
    // Prof said there would be a lot of type casting, so I guess this is where he was referring to?
    MemoryBlock *newBlock = (MemoryBlock *) ((char *) current + sizeof(MemoryBlock) + size);

    // Update the stuff in the new block, and the stuff in the current block
    newBlock->prev = current;
    newBlock->next = current->next;

    if (current->next != NULL) {
        current->next->prev = newBlock;
    }

    newBlock->size = current->size - size - sizeof(MemoryBlock);
    current->next = newBlock; //added thing for checking
    newBlock->isFree = true; //free memory
    current->size = size;
    current->isFree = false; //allocated memory

    return (char *) current + sizeof(MemoryBlock);
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
        if (current->size >= size + sizeof(MemoryBlock) && (current->isFree == true || (void *) current->isFree == NULL)) {
            if (bestFit == NULL || current->size < bestFit->size) {
                bestFit = current;
            } else if (current->size == bestFit->size) {
                bestFit = current;
                break;
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
    //  newBlock->size = bestFit->size - newBlock->size + sizeof(MemoryBlock);
    bestFit->size = size;
    bestFit->next = newBlock;
    bestFit->isFree = false;

    mm.size -= sizeof(MemoryBlock);

    return (char *) bestFit + sizeof(MemoryBlock);
}


void *mymalloc(size_t size) {
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

    if (size > mm.size - sizeof(MemoryBlock)) {
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

// TODO: Remove
void printHeap() {
    MemoryBlock *current = mm.head;
    while (current != NULL) {
        printf("Location: %p\tBlock size: %zu\tIs free?: %d\n", current, current->size, current->isFree);
        current = current->next;
    }
}

/*void myfree(void *ptr) {
    // If ptr is NULL, myfree does nothing
    if (ptr == NULL) {
        return;
    }

    // Get the block header for the given pointer
    MemoryBlock *block = (MemoryBlock *) ((char *) ptr - sizeof(MemoryBlock));

    // Mark the block as free
    block->isFree = true;

    // Add the block to the free list
    block->nextFree = mm.freeList;
    mm.freeList = block;

    // Try to coalesce with the previous block, if it exists and is free
    if (block->prev != NULL && block->prev->isFree) {
        MemoryBlock *prev = block->prev;
        prev->size += block->size + sizeof(MemoryBlock);
        prev->next = block->next;
        if (block->next != NULL) {
            block->next->prev = prev;
        }
        block = prev;
    }

    // Try to coalesce with the next block, if it exists and is free
    if (block->next != NULL && block->next->isFree) {
        MemoryBlock *next = block->next;
        block->size += next->size + sizeof(MemoryBlock);
        block->next = next->next;
        if (next->next != NULL) {
            next->next->prev = block;
        }
    }
}*/

void coalesce(MemoryBlock *foundBlock) {
    int wasPrevFree = -1; // a simple check to see if we got through the first if statement, used when checking next

    if (foundBlock->prev != NULL && foundBlock->prev->isFree == true) {
        MemoryBlock *prev = foundBlock->prev;
        prev->size += foundBlock->size + sizeof(MemoryBlock);
        prev->next = foundBlock->next;
        if (foundBlock->next != NULL) {
            foundBlock->next->prev = prev;
        }
        foundBlock = prev;
        wasPrevFree = 1;
    }

    if (foundBlock->next != NULL && foundBlock->next->isFree == true) {
        MemoryBlock *next = foundBlock->next;
        foundBlock->size += next->size + sizeof(MemoryBlock);
        foundBlock->next = next->next;
        if (next->next != NULL) {
            next->next->prev = foundBlock;
        }
    }

    if (wasPrevFree == -1) {
        foundBlock->isFree = true;
    }

    return;
}


void myfree(void* ptr) {
    // If ptr is NULL, myfree does nothing
    if (ptr == NULL) {
        return;
    }

    // Get the block header for the given pointer
    MemoryBlock *block = (MemoryBlock *) ((char *) ptr - sizeof(MemoryBlock));

    // Mark the block as free
    block->isFree = true;

    // Add the block to the free list
    block->nextFree = mm.freeList;
    mm.freeList = block;

    // Try to coalesce with the previous block, if it exists and is free
    coalesce(block);

    return;
}

void *myrealloc(void *ptr, size_t size) {
/*
* Grabs the data from ptr
* Frees it then creates a new block with size
* Copies the data from the old block to the new block
* returns new block
* */
    if (ptr == NULL || size == 0) {
        printf("Wrong pointer or size is 0");
        return NULL;
    }

    if (size % 8 != 0) {
        size = size + 8 - (size % 8);
    }

    MemoryBlock *block = (MemoryBlock *) ((char *) ptr - sizeof(MemoryBlock));
    size_t newSize = size + sizeof(MemoryBlock);

    if (newSize == block->size || newSize < block->size) {
        return ptr; // Keep size the same due to being 8 byte aligned
    }

    void *newPtr = mymalloc(size);
    if (newPtr == NULL) {
        return NULL;
    }

    memcpy(newPtr, ptr, block->size);
    myfree(ptr);

    return newPtr;
}


void mycleanup() {
    mm.head = NULL;
    mm.lastSearched = NULL;
    mm.allocAlgo = 0;
    mm.size = 0;

    free(mm.head);
}
