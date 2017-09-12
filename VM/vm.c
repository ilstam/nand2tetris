#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "exit.h"

#define MAX_LINE_LEN 200

/*
 * Check whether the given path corresponds to a regular file and if that's
 * the case try to open the file using fopen.
 */
FILE *file_open_or_bail(const char *filename, const char *mode)
{
    struct stat path_stat;

    if (stat(filename, &path_stat) != 0) {
        exit_program(EXIT_FILE_DOES_NOT_EXIST, filename);
    }
    if(!S_ISREG(path_stat.st_mode)) {
        exit_program(EXIT_NOT_REGULAR_FILE, filename);
    }

    FILE *fp = fopen(filename, mode);

    if (fp == NULL) {
        exit_program(EXIT_CANNOT_OPEN_FILE, filename);
    }

    return fp;
}

/*
 * Strip a line from comments and removes new line character.
 */
char *strip_comments(char *s) {
    // sanity checks
    if (s == NULL) {
        return NULL;
    } else if (!*s) {
        return s;
    }

    char *s2 = s;

    for (; *s2; s2++) {
        if (*s2 == '/' && *(s2+1) == '/') {
            *s2 = '\0';
            break;
        }
    }
    if (s2 != s && *(s2-1) == '\n') {
        // remove new line char
        *(s2-1) = '\0';
    }

    return s;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        exit_program(EXIT_MANY_FILES);
    }

    /*
     * Holds current line read.
     */
    char line[MAX_LINE_LEN + 1];

    FILE *fp = file_open_or_bail(argv[1], "r");

    while (fgets(line, sizeof(line), fp)) {
        strip_comments(line);
        printf("%s\n", line);
    }

    fclose(fp);

    return 0;
}
