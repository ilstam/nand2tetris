#include <stdio.h>
#include <stdlib.h>


void *assembler_malloc(size_t size)
{
    void *ptr = NULL;
    ptr = malloc(size);
    if (ptr == NULL) {
        printf("CRITICAL: Unable to allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
