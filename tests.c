#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

void testFirstFit() {
    printf("Tests for first fit algo:\n");

    myinit(0); // Use first-fit algorithm

    /*void *ptr1 = mymalloc(100);
    void *ptr2 = mymalloc(200);
    void *ptr3 = mymalloc(300);

    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);

    printHeap();*/

    int *ptr1 = (int *) mymalloc(sizeof(int));
    int *ptr2 = (int *) mymalloc(sizeof(int));
    char *ptr3 = (char *) mymalloc(sizeof(char));
    char *ptr4 = (char *) mymalloc(sizeof(char));
    /*
    printHeap();
printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    *ptr1 = 1;
    *ptr2 = 2;
    printf("ptr1 = %d\n", *ptr1);
    printf("ptr2 = %d\n", *ptr2);

    printf("ptr1 = %p has value of %d\n", ptr1, *ptr1);
    printf("ptr2 = %p and value of %d\n", ptr2, *ptr2);*/
//    printHeap();

    myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);
    myfree(ptr4);
    printHeap();



    // TODO: Fix Free & make realloc
    // Free the blocks
    /*myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);*/

    printf("My cleanup\n");
    mycleanup();
    printHeap(); // Should print nothing!
    printf("If you 'my cleanup' above this then that function works\n");
}


int main() {
    testFirstFit();
    exit(0);
}
