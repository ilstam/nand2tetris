#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <errno.h>


#define MAX_LINE_LEN  200
#define MAX_INSTRUCTIONS UINT_MAX


enum exitcode {
    /*
     * Error code 1 represents that given file does not exist.
     */
	EXIT_FILE_DOES_NOT_EXIST = 1,
    /*
     * Error code 2 represents that given file couldn't be opened due to unknown reasons.
     */
	EXIT_CANNOT_OPEN_FILE = 2,
    /*
     * Error code 3 represents that more than 1 input files have been provided.
     */
    EXIT_MANY_FILES = 3,
    /*
     * Error code 4 represents that file contains too many instructions to be translated.
     */
	EXIT_TOO_MANY_INSTRUCTIONS = 4
};

const char *error_messages[] =
{
    [EXIT_CANNOT_OPEN_FILE] = "Can't open file %s",
    [EXIT_FILE_DOES_NOT_EXIST] = "File %s does not exist",
    [EXIT_MANY_FILES] = "One and only one file operand is expected",
    [EXIT_TOO_MANY_INSTRUCTIONS] = "File contians too many instructions. "
                                   "Only a maximum of %u instructions can be translated."
};


void exit_program(enum exitcode code, ...)
{
    va_list arguments;

    va_start(arguments, code);

    printf("%s: Error: ", __FILE__);
    vfprintf(stdout, error_messages[code], arguments);
    printf("\n");

    va_end(arguments);
    exit(code);
}

/*
 * Try opening a file using fopen and quit the program if this fails. Just a
 * wrapper for the fopen function.
 */
FILE *file_open_or_bail(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);

    if (fp == NULL) {
        if (errno == 2) {
            exit_program(EXIT_FILE_DOES_NOT_EXIST, filename);
        } else {
            exit_program(EXIT_CANNOT_OPEN_FILE, filename);
        }
    }

    return fp;
}

/*
 * Strip a line from comments and remove *all* whitespace.
 * Comments are indicated by two adjacent / characters.
 *
 * E.g. "  A  = M  // comment " becomes "A=M"
 */
char *strip_comments_and_whitespace(char *s) {
    // sanity checks
    if (s == NULL) {
        return NULL;
    } else if (!*s) {
        return s;
    }

    char s_new[strlen(s) + 1];
    int i = 0;

    for (char *s2 = s; *s2; s2++) {
        if (*s2 == '/' && *(s2+1) == '/') {
            break; // a comment starts from this point on; skip rest of the line
        } else if (!isspace(*s2)) {
            s_new[i++] = *s2;
        }
    }
    s_new[i] = '\0';

    strcpy(s, s_new); // this is safe because s_new is smaller than s

    return s;
}



int main(int argc, const char *argv[])
{
    if (argc != 2) {
        exit_program(EXIT_MANY_FILES);
    }

    const char *filename = argv[1];
    char line[MAX_LINE_LEN + 1];
    unsigned instruction_num = 1;

    FILE *fp = file_open_or_bail(filename, "r");

    while (fgets(line, sizeof(line), fp)) {
        strip_comments_and_whitespace(line);

        if (!*line) {
            // skip empty lines
            continue;
        }

        puts(line);

        instruction_num++;
        if (instruction_num == MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS);
        }
    }

    fclose(fp);

    return 0;
}
