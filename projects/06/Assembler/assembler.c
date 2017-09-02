#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "hack_standard.h"
#include "exit.h"

#define MAX_LINE_LEN  200

/*
 * We cannot support more than MAX_ADDRESS instructions because we are not
 * able to address them.
 */
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS


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
