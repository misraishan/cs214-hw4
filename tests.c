#include <stdio.h>
#include <string.h>
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

    printHeap();

    // TODO: Fix Free & make realloc
    // Free the blocks
    /*myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);*/
    mycleanup();

    printHeap(); // Should print nothing!
}


int main() {
    testFirstFit();
    exit(0);
}
