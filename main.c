//
// Created by Ishan Misra on 12/10/22.
//

#include <stdio.h>
#include "tests/testFirstFit.c"
#include "tests/testNextFit.c"
#include "tests/testBestFit.c"

int main() {
    printf("Running all tests...\n");
    printf("Testing first fit...\n\n");
    testFirstFit();

    printf("\n\nTesting next fit...\n\n");
    testNextFit();

    printf("\n\nTesting best fit...\n\n");
    testBestFit();
}