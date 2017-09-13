#include <stdio.h>
#include <stdlib.h>

#include "exit.h"


const char *error_messages[] =
{
    [EXIT_FILE_DOES_NOT_EXIST] = "%s does not exist",
    [EXIT_NOT_REGULAR_FILE] = "%s is not a regular file",
    [EXIT_CANNOT_OPEN_FILE] = "Can't open file %s",
    [EXIT_MANY_FILES] = "One and only one file operand is expected",
    [EXIT_TOO_MANY_INSTRUCTIONS] = "File contains too many instructions. "
                                   "Only a maximum of %u instructions can be translated.",
    [EXIT_SYMBOL_ALREADY_EXISTS] = "Line %u: %s : Symbol is already defined",
    [EXIT_INVALID_LABEL] = "Line %u: %s : Invalid label name",
    [EXIT_INVALID_A_INST] = "Line %u: %s : Invalid A-instruction operand",
    [EXIT_INVALID_C_DEST] = "Line %u: %s : Invalid destination part of C-instruction",
    [EXIT_INVALID_C_COMP] = "Line %u: %s : Ivalid compare part of C-instruction",
    [EXIT_INVALID_C_JUMP] = "Line %u: %s : Invalid jump part of C-instruction",
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
