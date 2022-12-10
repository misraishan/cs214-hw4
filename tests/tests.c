#include <stdio.h>
#include "../mymalloc.h"

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

    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);
    printf("ptr4 = %p\n", ptr4);
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
    myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);

    printf("My cleanup\n");
    mycleanup();
    printHeap(); // Should print nothing!
    printf("If you 'my cleanup' above this then that function works\n");
}

void testBestFit() {
    printf("Tests for best fit algo:\n");

    myinit(2); // Use best-fit algorithm

    void *ptr1 = mymalloc(100);
    void *ptr2 = mymalloc(200);
    void *ptr3 = mymalloc(300);

    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);

    printHeap();

    // Free the blocks
    myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);

    printf("My cleanup\n");
    mycleanup();
    printHeap(); // Should print nothing!
    printf("If you 'my cleanup' above this then that function works\n");
}

void testNextFit() {
    printf("Tests for next fit algo:\n");

    myinit(1); // Use next-fit algorithm

    void *ptr1 = mymalloc(100);
    void *ptr2 = mymalloc(200);
    void *ptr3 = mymalloc(300);

    printf("ptr1 = %p\n", ptr1);
    printf("ptr2 = %p\n", ptr2);
    printf("ptr3 = %p\n", ptr3);

    printHeap();

    // Free the blocks
    myfree(ptr1);
    myfree(ptr2);
    myfree(ptr3);

    printf("My cleanup\n");
    mycleanup();
    printHeap(); // Should print nothing!
    printf("If you 'my cleanup' above this then that function works\n");
}


int main() {
    printf("\t\tTest first fit.\n\n");
    testFirstFit();
    printf("\n\n\t\tTest Next fit.\n\n");
    testNextFit();
    printf("\n\n\t\tTest Best fit.\n\n");
    testBestFit();
    exit(0);
}
