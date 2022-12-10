#include "mymalloc2.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//define memory block
typedef struct MemoryBlock {
    struct MemoryBlock *next;
    struct MemoryBlock *prev;
    unsigned int size;
    unsigned int FreeORAlloc; // if free 0 | if allocated 1
} MemoryBlock;

//global variables
static char heap[1024 * 1024];
static MemoryBlock *head;
static int algo;

void myinit(int algorithm) {
    
    // Check if algorithm is not 0 1 or 2
    if (algorithm < 0 || algorithm > 2) {
        printf("Invalid algorithm. Please choose 0, 1, or 2.\n");
        exit(1);
    }

    // Create the 1 MB heap
    head = (MemoryBlock *) heap;
    head->size = 1024 * 1024;
    head->next = NULL;
    head->prev = NULL;
    head->FreeORAlloc = 0;
    algo = algorithm;
}

//working? printing out the right values I'm pretty sure
void* firstFit(size_t size) {
    
    //create temp for looping
    MemoryBlock *current = head;

    //find first block that is large enough (and free) that can store the given size
    while(current != NULL) { //while loop is not null 
        if(current->size < size + sizeof(MemoryBlock ) || current->FreeORAlloc == 1) { //is current size < size or is current allocated?
            current = current->next;
        }
        else {
            break;
        }
    }

    //no spaces to allocate?
    if(current == NULL) {
        return NULL;
    }

    //found a spot for allocation, update main block
    MemoryBlock *newBlock = (MemoryBlock *) ((char*)current + sizeof(MemoryBlock) + size);
    printf("\ncheck1: newBlock->size: %d\ncurrent->size %d\nsizeof(MemoryBlock) %ld\n", newBlock->size, current->size, sizeof(MemoryBlock));
    
    newBlock->prev = current;
    newBlock->next = current->next;

    if(current->next != NULL) {
        current->next->prev = newBlock;
    }
    
    newBlock->size = current->size - newBlock->size - sizeof(MemoryBlock);
    current->next = newBlock; //added thing for checking
    newBlock->FreeORAlloc = 0; //free memory
    current->size = size;
    current->FreeORAlloc = 1; //allocated memory
    printf("\ncheck2: newBlock->size: %d\ncurrent->size %d\nsizeof(MemoryBlock) %ld\n\n", newBlock->size, current->size, sizeof(MemoryBlock));
    return (char*)current + sizeof(MemoryBlock);
}

void* nextFit(size_t size) {
    
}

void* bestFit(size_t size) {
    
}

void* mymalloc(size_t size) {

    //did it initialize correctly?
    if(head == NULL) {
        printf("Error: Memory manager is not initialized\n");
        return NULL;
    }

    //are we allocating 0 size?
    if(size == 0) {
        return NULL;
    }

    //are we 8-byte aligned?
    if(size % 8 != 0) {
        size += (8 - (size % 8));
    }

    //act depending on algo
    if(algo == 0) {
        return firstFit(size);
    }
    else if(algo == 1) {
        return nextFit(size);
    }
    else if(algo == 2) {
        return bestFit(size);
    }
    else {
        printf("Error: Invalid allocation algorithm\n");
        return NULL;
    }
}

void* findBlock(void* ptr) {
    MemoryBlock *current = head;

    //search for ptr
    while(current != NULL) {
        if(((char *) current + sizeof(MemoryBlock)) == (char *) ptr ) {
            //printf("I AM PETER GRIFFIN AND I AM PROUD\n");
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
    if(foundBlock->prev != NULL && foundBlock->prev->FreeORAlloc == 0) { //previous gets combines with foundBlock, making prev the new pointer
        wasPrevFree = 0;

        //increase size of block and combine
        foundBlock->prev->size += (foundBlock->size); //increase block size with size of block
        foundBlock->prev->next = foundBlock->next; //prev->next points to next
        
        if(foundBlock->next != NULL) {
            foundBlock->next->prev = foundBlock->prev; //next->prev points to prev, final bit of cutting out foundBlock from linked list
        }
    }

    //check if next exists and is free
    if(foundBlock->next != NULL && foundBlock->next->FreeORAlloc == 0) {
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
    if(foundBlock->FreeORAlloc == 0) {
        printf("Error: already free\n");
        return;
    }

    //otherwise free and coalesce
    foundBlock->FreeORAlloc = 0; //set block as free
    coalesce(foundBlock);
}

//don't forget to remove
void printHeap() {
    MemoryBlock *current = head;
    while (current != NULL) {
        printf("Location: %p\tBlock size: %zu\tValue is: %d\n", current, current->size, current[sizeof(MemoryBlock)]);
        current = current->next;
    }
}
