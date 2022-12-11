//
// Created by Ishan Misra on 12/10/22.
//

void testBestFit() {
    myinit(2);

    int *intArray = (int *) mymalloc(5*sizeof(int)); // 24 bytes
    int *fiveInts = (int *) mymalloc(5 * sizeof(int)); // 24
    char *fiveChar = (char *) mymalloc(sizeof(char) * 5); // 8 byes
    int *threeInts = (int *) mymalloc(sizeof(int) * 3); // 16 bytes
    int *singleInt = (int *) mymalloc(sizeof(int)); // 8 bytes

    printf("Malloc'd 24, 24, 8, 16, and 8\n");
    printf("Will now free singleInt and moreBits\nPrinting heap...\n");
    printHeap();

    myfree(threeInts);
    myfree(fiveInts);

    printf("5Ints and 3Ints freed, 24b then 16b.\nNew malloc should take 16b.\n");
    int *newMallocByte = (int *) mymalloc(sizeof(int) * 3);

    printf("Printing heap...\n");
    printHeap();

    printf("Cleanup!");
    mycleanup();
}
