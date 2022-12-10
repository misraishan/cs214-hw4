#include <stdio.h>
#include <string.h>
#include "mymalloc2.h"

void testFirstFit() {
    myinit(0); // Use first-fit algorithm

    int *intArray;
    int *intJoe;
    printf("Without adding ANYTHING, here is the heap\n");
    printHeap();

    printf("=========================\n\n");
    printf("Running any checks by using mymalloc\n");
    intArray = (int *) mymalloc(5*sizeof(int)); //should allocate 24 bits, meaning remaining size in heap is 1048552
    //intJoe = (int *) mymalloc(1000*sizeof(int));
    
    printf("intArray = %p", intArray);
    printf(": intArray has no values...\n");
    printf("printHeap:\n");
    printHeap();

    printf("===============================\n\n");

    intArray[0] = 1;
    intArray[1] = 2;
    intArray[2] = 3;
    intArray[3] = 4;
    intArray[4] = 5;

    printf("Does adding values to the intArray do anything?\n");
    printf("intArray = %p", intArray);
    printf(": intArray has values...\n");
    printf("printHeap:\n\n");
    printf("In position 0  and 4 of the array we have the values: %d and %d\n", intArray[0], intArray[4]);
    printHeap();

    printf("===============================\n\n");
    printf("What does freeing do?\n");
    myfree(intArray);
    printf("intArray = %p", intArray);
    printf(": intArray has hopefully been freed...\n");
    printf("printHeap:\n");
    printHeap();

    intJoe = (int *) mymalloc(5*sizeof(int));
    printf("intJoe = %p", intJoe);
    printf(": intJoe before free...\n");
    printf("printHeap:\n");
    printHeap();
    printf("===============================\n\n");
    myfree(intJoe);
    printf("intJoe = %p", intJoe);
    printf(": intJoe after free...\n");
    printf("printHeap:\n");
    printHeap();


    //checking to see if we ever lose memory from adding and freeing a SHIT ton
    //DOES NOT WORK WHYWHYWHYWHYWHY
    //for(int i = 0; i < 100; i++) {
        //intArray = (int *) mymalloc(5*sizeof(int));
        //myfree(intArray);
        //printf("\nprintHeap:\n");
        //printHeap();
    //}

    //should print not able to find block or something??????
    myfree(intArray);

    printf("printHeap:\n");
    printHeap();
}

int main() {
    printf("\t\tTest first fit.\n\n");
    testFirstFit();
    exit(0);
}
