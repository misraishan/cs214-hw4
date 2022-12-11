//
// Created by Ishan Misra on 12/10/22.
//

#include "../mymalloc.h"

void testNextFit() {
    myinit(1); // Use first-fit algorithm

    int *intArray = (int *) mymalloc(5*sizeof(int)); // 24 bytes
    int *singleInt = (int *) mymalloc(sizeof(int)); // 8 bytes
    char *fiveChar = (char *) mymalloc(sizeof(char) * 5); // 8 byes

    printf("Malloc'd 24, 8, and 8\n");
    printf("Will now free middle (singleInt)\n");

    myfree(singleInt);

    printf("Single Int freed. Allocating char (should be in address after fiveChar\n");
    int *newSingle = (int *) mymalloc(sizeof(int));

    printf("Printing heap...\n");
    printHeap();

    printf("Cleanup!");
    mycleanup();
}