//
// Created by Ishan Misra on 12/12/22.
//

#include <string.h>
#include "../mymalloc.h"

void testRealloc() {
    myinit(0);

    printf("Total heap size set to %d & using first fit\n", 1024 * 1024);

    char *single = mymalloc(1);
    printf("Allocated 1 byte at %p\n", single);
    printHeap();
    printf("\n");

    int *intArray = (int *) mymalloc(5*sizeof(int)); // 24 bytes
    int *singleInt = (int *) mymalloc(sizeof(int)); // 8 bytes

    printf("Allocated 5 ints and 1 int\n");
    printf("32 bytes allocated\n");
    printHeap();

    printf("Reallocating single to 16 bytes... Should return new address\n");
    single = myrealloc(single, 16);
    printHeap();

    printf("Reallocing intarray in place... Should return same address\n");
    intArray = myrealloc(intArray, 5*sizeof(int));
    printHeap();

    printf("Freeing all and performing cleanup...\n");
    myfree(single);
    myfree(intArray);
    myfree(singleInt);
    printHeap();

    mycleanup();
}