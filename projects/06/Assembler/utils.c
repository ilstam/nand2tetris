#include <stdlib.h>

#include "exit.h"


void *assembler_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL) {
        exit_program(EXIT_OUT_OF_MEMORY);
    }

    return ptr;
}
