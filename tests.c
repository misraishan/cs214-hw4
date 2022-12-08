#include <stdio.h>
#include "mymalloc.h"

void testFirstFit() {
    printf("Tests for first fit algo:\n");

    myinit(0); // Use first-fit algorithm

    void *ptr1 = mymalloc(100);
    void *ptr2 = mymalloc(200);
    void *ptr3 = mymalloc(300);

    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);

//    printHeap();
    myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);

//    printHeap();


/*
    mycleanup();

    // Reallocate some memory
    void *ptr4 = myrealloc(ptr1, 400);
    void *ptr5 = myrealloc(ptr4, 500);

    // Print the addresses of the reallocated blocks
    printf("ptr4 = %p\n", ptr4);
    printf("ptr5 = %p\n", ptr5);

    // Clean up the memory manager
    mycleanup();*/

}


int main() {
    testFirstFit();
    exit(0);
}
