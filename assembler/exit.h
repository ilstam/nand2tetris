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
     * Exit code 7 represents that file contains too many instructions to be translated.
     */
    EXIT_TOO_MANY_INSTRUCTIONS = 7,
    /*
     * Exit code 8 represents that there was an attempt to redefine a symbol.
     */
    EXIT_SYMBOL_ALREADY_EXISTS = 8,
    /*
     * Exit code 9 represents that an invalid label have been encountered.
     */
    EXIT_INVALID_LABEL = 9,
    /*
     * Exit code 10 represents that an invalid A-instruction have been encountered.
     */
    EXIT_INVALID_A_INST = 10,
    /*
     * Exit code 11 represents that the destination part of a C-instruction is invalid.
     */
    EXIT_INVALID_C_DEST = 11,
    /*
     * Exit code 12 represents that the compare part of a C-instruction is invalid.
     */
    EXIT_INVALID_C_COMP = 12,
    /*
     * Exit code 13 represents that the jump part of a C-instruction is invalid.
     */
    EXIT_INVALID_C_JUMP = 13,
    /*
     * Exit code 15 represents that the program run out of memory.
     */
    EXIT_OUT_OF_MEMORY = 15,
};


void exit_program(enum exitcode code, ...);
