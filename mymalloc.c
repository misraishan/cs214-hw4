#include "mymalloc.h"
#include <string.h>

// Global memory manager
MemoryManager mm;

void *firstFit(MemoryBlock *head, size_t size);
void *nextFit(size_t size);
void *bestFit(size_t size);

static char heap[1024 * 1024];

void myinit(int algorithm) {

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
    mm.head->prev = NULL;
    mm.head->isFree = true;
}

void *firstFit(MemoryBlock *head, size_t size) {
    MemoryBlock *current = head;

    // Find the first block that is large enough to satisfy the request
    // Return null if nothing found
    while (current != NULL && current->size < size + sizeof(MemoryBlock)) {
        current = current->next;
    }

    while(current != NULL) { //while loop is not null
        if(current->size < size + sizeof(MemoryBlock ) || current->isFree == false) { //is current size < size or is current allocated?
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
    MemoryBlock *newBlock = (MemoryBlock *)((char *)current + size + sizeof(MemoryBlock));

    // Update the stuff in the new block, and the stuff in the current block
    newBlock->prev = current;
    newBlock->next = current->next;

    if(current->next != NULL) {
        current->next->prev = newBlock;
    }

    newBlock->size = current->size - newBlock->size - sizeof(MemoryBlock);
    current->next = newBlock; //added thing for checking
    newBlock->isFree = true; //free memory
    current->size = size;
    current->isFree = false; //allocated memory

    return (char *)current + sizeof(MemoryBlock);
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

// TODO: Remove
void printHeap() {
    MemoryBlock *current = mm.head;
    while (current != NULL) {
        printf("Location: %p\tBlock size: %zu\tValue is: %p\n", current, current->size, current[sizeof(MemoryBlock)]);
        current = current->next;
    }
}

void* findBlock(void* ptr) {
    MemoryBlock *current = mm.head;

    //search for ptr
    while(current != NULL) {
        if(((char *) current + sizeof(MemoryBlock)) == (char *) ptr ) {
            break;
        }
        current = current->next;
    }
    return current;
}

//working? printing out the right values when adding and removing one block from the heap
void coalesce(MemoryBlock *foundBlock) {

    int wasPrevFree = -1; // a simple check to see if we got through the first if statement, used when checking next

    printf("check - 1\n");
    //check if previous exists and is free
    if(foundBlock->prev != NULL && foundBlock->prev->isFree == true) { //previous gets combines with foundBlock, making prev the new pointer
        wasPrevFree = 0;

        //increase size of block and combine
        foundBlock->prev->size += (foundBlock->size); //increase block size with size of block
        foundBlock->prev->next = foundBlock->next; //prev->next points to next

        if(foundBlock->next != NULL) {
            foundBlock->next->prev = foundBlock->prev; //next->prev points to prev, final bit of cutting out foundBlock from linked list
        }
    }

    //check if next exists and is free
    if(foundBlock->next != NULL && foundBlock->next->isFree == true) {
        printf("check - 2\n");
        if(wasPrevFree == 0) { //foundBlock is no longer a node / pointer
            foundBlock->prev->size += (foundBlock->next->size); //increase block size with size of next
            foundBlock->prev->next = foundBlock->next->next; //prev->next points to next next

            if(foundBlock->next->next != NULL) {
                foundBlock->next->next->prev = foundBlock->prev; // final bit of cutting out next from linked list
            }
        }
        else { //foundBlock is a pointer that combines with next
            //printf("check - 3\n");
            foundBlock->size += (foundBlock->next->size); //increase block size with size of next
            //printf("check - 4\n");
            foundBlock->next = foundBlock->next->next;
            //printf("check - 5\n");

            if(foundBlock->next != NULL) {
                foundBlock->next->prev = foundBlock;
            }
        }
    }
}

//working? see above HOWEVER WE NEED TO ADD AN EXPLICIT FREE LIST OF ALL FREE POINTERS (shouldn't be too hard?)
void myfree(void* ptr) {

    MemoryBlock *head = mm.head;

    //did it initialize correctly?
    if(head == NULL) {
        printf("Error: Memory manager is not initialized\n");
        return;
    }

    //is ptr null?
    if(ptr == NULL) {
        return;
    }

    //find block
    MemoryBlock *foundBlock = findBlock(ptr);

    //did we actually find a block?
    if (foundBlock == NULL) {
        printf("Error: could not find ptr\n");
        return;
    }

    //is it already free?
    if(foundBlock->isFree == true) {
        printf("Error: already free\n");
        return;
    }

    //otherwise free and coalesce
    foundBlock->isFree = true; //set block as free
    coalesce(foundBlock);
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
