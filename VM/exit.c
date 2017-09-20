#include <stdio.h>
#include <stdlib.h>

#include "exit.h"


const char *error_messages[] =
{
    [EXIT_FILE_DOES_NOT_EXIST] = "%s does not exist",
    [EXIT_NOT_FILE_OR_DIR] = "%s is neither a regular file nor a dir",
    [EXIT_CANNOT_OPEN_FILE] = "Can't open file %s",
    [EXIT_CANNOT_OPEN_FILE_OUT] = "Can't open file for writing %s",
    [EXIT_MANY_ARGS] = "One and only one file or dir operand is expected",
    [EXIT_NO_FILES_FOUND] = "No VM files found in given directory",
    [EXIT_INVALID_COMMAND] = "Line %u: %s: Invalid command",
    [EXIT_OUT_OF_MEMORY] = "CRITICAL: Unable to allocate memory!",
};


void exit_program(enum exitcode code, ...)
{
    va_list arguments;

    va_start(arguments, code);

    printf("VM Translator: ERROR: ");
    vfprintf(stdout, error_messages[code], arguments);
    printf("\n");

    va_end(arguments);
    exit(code);
}
