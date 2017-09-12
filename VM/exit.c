#include <stdio.h>
#include <stdlib.h>

#include "exit.h"


const char *error_messages[] =
{
    [EXIT_FILE_DOES_NOT_EXIST] = "%s does not exist",
    [EXIT_NOT_REGULAR_FILE] = "%s is not a regular file",
    [EXIT_CANNOT_OPEN_FILE] = "Can't open file %s",
    [EXIT_MANY_FILES] = "One and only one file operand is expected",
    [EXIT_OUT_OF_MEMORY] = "CRITICAL: Unable to allocate memory!",
};


void exit_program(enum exitcode code, ...)
{
    va_list arguments;

    va_start(arguments, code);

    printf("Assembler: ERROR: ");
    vfprintf(stdout, error_messages[code], arguments);
    printf("\n");

    va_end(arguments);
    exit(code);
}
