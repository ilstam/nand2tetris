#pragma once

#include <stdarg.h>

enum exitcode {
    /*
     * Exit code 1 represents that given file does not exist.
     */
    EXIT_FILE_DOES_NOT_EXIST = 1,
    /*
     * Exit code 2 represents that given file is not a regular file.
     */
    EXIT_NOT_REGULAR_FILE = 2,
    /*
     * Exit code 3 represents that given file couldn't be opened due to unknown reasons.
     */
    EXIT_CANNOT_OPEN_FILE = 3,
    /*
     * Exit code 4 represents that more than 1 input files have been provided.
     */
    EXIT_MANY_FILES = 4,
    /*
     * Exit code 7 represents that an invalid command has been encountered.
     */
    EXIT_INVALID_COMMAND = 7,
    /*
     * Exit code 15 represents that the program run out of memory.
     */
    EXIT_OUT_OF_MEMORY = 15,
};


void exit_program(enum exitcode code, ...);
