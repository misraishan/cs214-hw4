//
// Created by Ishan Misra on 12/10/22.
//

#include <stdio.h>
#include "tests/testFirstFit.c"
#include "tests/testNextFit.c"
#include "tests/testBestFit.c"
#include "tests/testRealloc.c"

int main() {
    printf("\033[0;31m");
    printf("Running all tests...\n");
    printf("Testing first fit...\n\n");
    printf("\033[0m");
    testFirstFit();

    printf("\033[0;31m");
    printf("\n\nTesting next fit...\n\n");
    printf("\033[0m");
    testNextFit();

    printf("\n\nTesting best fit...\n\n");
    printf("\033[0m");
    testBestFit();

    printf("\033[0;31m");
    printf("\n\nTesting realloc...\n\n");
    printf("\033[0m");
    testRealloc();
}