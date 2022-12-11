//
// Created by Ishan Misra on 12/10/22.
//

#include "../mymalloc.h"

void testFirstFit() {
    myinit(0); // Use first-fit algorithm

    printf("Total heap size set to %d\n", 1024 * 1024);

    char *single = mymalloc(1);
    printf("Allocated 1 byte at %p\n", single);
    printHeap();
    printf("\n");

    int *intArray = (int *) mymalloc(5*sizeof(int)); // 24 bytes
    int *singleInt = (int *) mymalloc(sizeof(int)); // 8 bytes

    printf("Allocated 5 ints and 1 int\n");
    printf("32 bytes allocated\n");
    printHeap();

    printf("Freeing both...\n");
    myfree(intArray);
    myfree(singleInt);
    printf("32 bytes freed\n");
    printHeap();

    printf("Allocating chars...\n");
    char *charArray = (char *) mymalloc(5*sizeof(char)); // 5 bytes
    charArray[0] = 'h';
    charArray[1] = 'e';
    charArray[2] = 'l';
    charArray[3] = 'l';
    charArray[4] = 'o';

    printf("Allocated 5 chars, with value of %s\n", charArray);
    printf("5 bytes allocated\n");
    printHeap();

    printf("Freeing char array...\n");
    myfree(charArray);
    printf("5 bytes freed\n");
    printHeap();

    single = myrealloc(single, 16);
    printf("Realloc test, reallocating single to 16 bytes...\n");
    printHeap();

    printf("Freeing original char...\n");
    myfree(single);
    printHeap();

    mycleanup();
    printf("Heap cleaned up and has size of %d\n", getSize());
    printHeap();
}