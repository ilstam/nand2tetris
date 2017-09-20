#pragma once

#include <stdarg.h>

enum exitcode {
    /*
     * Exit code 1 represents that input file does not exist.
     */
    EXIT_FILE_DOES_NOT_EXIST = 1,
    /*
     * Exit code 2 represents that input file is not a regular file nor a directory.
     */
    EXIT_NOT_FILE_OR_DIR = 2,
    /*
     * Exit code 3 represents that input file couldn't be opened due to unknown reasons.
     */
    EXIT_CANNOT_OPEN_FILE = 3,
    /*
     * Exit code 4 represents that output file couldn't be opened for writing due to unknown reasons.
     */
    EXIT_CANNOT_OPEN_FILE_OUT = 4,
    /*
     * Exit code 5 represents that more than 1 arguments have been provided to the program.
     */
    EXIT_MANY_ARGS = 5,
    /*
     * Exit code 6 represents that no VM files found in given directory.
     */
    EXIT_NO_FILES_FOUND = 6,
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
