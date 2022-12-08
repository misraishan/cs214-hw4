#include <stdio.h>
#include "mymalloc.h"

void testFirstFit() {
    // Initialize the memory manager
    myinit(0); // Use first-fit algorithm

    // Allocate some memory
    void *ptr1 = mymalloc(100);
    void *ptr2 = mymalloc(200);
    void *ptr3 = mymalloc(300);

    // Print the addresses of the allocated blocks
    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);

//    printHeap();
    // Free the memory
    myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);

    // Print the addresses of the allocated blocks
    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);


//    mycleanup();

    // Free some memory
    /*myfree(ptr2);
    myfree(ptr3);

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
